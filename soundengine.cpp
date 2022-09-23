/*
   gravel - live coding music system -
   Copyright (C) 2022 Malte Steiner

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "soundengine.h"
#include "QtCore/QDebug"
#include <QApplication>
#include <QFile>

typedef enum {
    NONE,
    INSTRUMENT,
    INSTRUMENTNUMBER,
    INSTRUMENTNAME,
    PARAMETER,
    PARAMETERNUMBER,
    PARAMETERNAME,
    PARAMETERVALUE
} InstrumentStates;

SoundEngine::SoundEngine(QObject *parent)
    : QObject{parent}
{

}

void SoundEngine::process(void)
{
    qDebug() << "worker process";       //This works

    // start Csound thread
    Csound *csound = new Csound();
    QString csd = "/Users/herrsteiner/Projects/gravel/csoundIntruments.csd";
    parseCsound(csd);
    int result = csound->CompileCsd(qPrintable(csd));

    this->csound = csound;
    csound->SetDebug(0);

    if (!result) {

        csound->Start();
        CsoundPerformanceThread* perfThread = new CsoundPerformanceThread(csound->GetCsound());
        this->perfThread = perfThread;
        perfThread->Play();

        //startTimer(rate,Qt::PreciseTimer);
        qDebug()<<"this thread "<<QThread::currentThread();

        // setup sequencer clock
        //60*4 / BPM = 1 Bar in s
        int rate =  54;
        QTimer *timer = new QTimer(this);
        timer->setTimerType(Qt::PreciseTimer);
        connect(timer,SIGNAL(timeout()),this,SLOT(seqStep()));
        timer->start(rate);

    }
    else {
        qDebug()<<"Csound error";
    }
}

/*
void SoundEngine::setPattern(QList<int> pattern){
    if (playPatternA) patternB = pattern;
    else patternA = pattern;
    swapPattern = true;
}*/

//void SoundEngine::timerEvent(QTimerEvent *event)
void SoundEngine::seqStep()
{
    //qDebug() << "Timer ID:" << event->timerId()<<" index "<<seqIndex;
    qDebug()<<"thread "<<QThread::currentThread()<< "main "<< QApplication::instance()->thread();
    QMapIterator<QString, Track> trackIterator(tracks);
    TracksType tickedTracks;
    while (trackIterator.hasNext()) {
        trackIterator.next();
        Track currentTrack = trackIterator.value();
        qDebug()<<"process track: "<<trackIterator.key();
        QList<PatternEvent> patternEvents = currentTrack.getNextEvents();
        QListIterator<PatternEvent> eventIterator(patternEvents);
        while (eventIterator.hasNext()){
            PatternEvent e = eventIterator.next();
            if (e.instrumentNumber > 0) {
                const double parameters[3] = {e.instrumentNumber,0,1.};
                this->perfThread->ScoreEvent( false,'i', 3, parameters);
                qDebug()<<"trigger instr: "<< e.instrumentNumber;
            }
        }
        tickedTracks[trackIterator.key()] = currentTrack;
    }

    tracks.swap(tickedTracks);

}

void SoundEngine::parseCsound(QString fileName){
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;
    QTextStream in(&file);
    QString line;
    InstrumentDefinition *currentInstrument;
    Parameter *currentParameter;
    QChar ch;
    QString currentToken;
    InstrumentStates state = NONE;

    while (!in.atEnd()){
        line = in.readLine().trimmed();
        // parse that line of csound code
        qsizetype lineLength = line.length();
        currentToken.clear();
        for(int i = 0; i < lineLength; i++)
        {
            ch = line.at(i);
            if (state == NONE){
                currentToken.append(ch);
                if (currentToken == "instr"){
                    state = INSTRUMENTNUMBER;
                    currentInstrument = new InstrumentDefinition();
                    currentToken.clear();
                }
                continue;
            }
            if (state == INSTRUMENTNUMBER){
                if (ch.isDigit()){
                    currentToken.append(ch);
                    continue;
                }
                if (ch == ';'){
                    state = INSTRUMENTNAME;

                    currentInstrument->instrNumber = currentToken.toInt();
                    currentToken.clear();
                }
                continue;
            }
            /*
            if (state == INSTRUMENT){
                if (ch == ';'){
                    state = INSTRUMENTNAME;
                }
                continue;
            }*/
            if (state == INSTRUMENTNAME){
                if (ch == ' ' || i == lineLength - 1){
                    currentInstrument->Name = currentToken;
                    currentToken.clear();
                    state = PARAMETER;
                    continue;
                }
                currentToken.append(ch);
                continue;
            }
            if (state == PARAMETER){
                if (ch == '='){
                    state = PARAMETERNUMBER;
                    currentToken.clear();
                    currentParameter = new Parameter();

                    continue;
                }

                currentToken.append(ch); // actually fishing here for endin statement
                if (currentToken == "endin"){
                    if (currentInstrument){
                        instruments.append(*currentInstrument);
                        currentToken.clear();
                        state = NONE;
                    }

                }
                continue;
            }
            if (state == PARAMETERNUMBER){
                if (ch == ';') {
                    currentParameter->pNumber = currentToken.toInt();
                    currentToken.clear();
                    state = PARAMETERNAME;
                    continue;
                }
                if (ch.isDigit()){
                    currentToken.append(ch);
                }
                continue;
            }
            if (state == PARAMETERNAME){
                if (ch == ' '){
                    currentParameter->Name = currentToken;
                    currentToken.clear();
                    state = PARAMETERVALUE;
                    continue;
                }
                currentToken.append(ch);
                continue;
            }
            if (state == PARAMETERVALUE){
                if (i == lineLength -2){
                    currentParameter->value = currentToken.toDouble();
                    currentToken.clear();
                    state = PARAMETER;
                    if (currentParameter && currentParameter->Name != nullptr){
                        currentInstrument->parameters[currentParameter->Name] = *currentParameter;
                    }

                    continue;
                }
                if (ch.isDigit() || ch == '.'){
                    currentToken.append(ch);
                }
                continue;
            }

        }
    }
    file.close();

    QString instrumentMessage;
    QListIterator<InstrumentDefinition> iterator(instruments);
    instrumentMessage.append("load Csound file with following instrument definitions:\n");

    while (iterator.hasNext()){
        InstrumentDefinition instrument = iterator.next();
        instrumentMessage.append("instrument #");

        instrumentMessage.append(QString::number(instrument.instrNumber));
        instrumentMessage.append(" ");
        instrumentMessage.append(instrument.Name);
        instrumentMessage.append(" parameters:\n");
        QMapIterator<QString,Parameter> parameterIterator(instrument.parameters);
        while (parameterIterator.hasNext()) {
            parameterIterator.next();
            instrumentMessage.append("parameter ");
            instrumentMessage.append(parameterIterator.key());
            instrumentMessage.append(" pNumber: ");
            Parameter parameter = parameterIterator.value();
            instrumentMessage.append(QString::number(parameter.pNumber));
            instrumentMessage.append(" defaultValue:");
            instrumentMessage.append(QString::number(parameter.value));
            instrumentMessage.append("\n");
        }
    }
    emit display(instrumentMessage);
}
