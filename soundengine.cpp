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
#include <QtGlobal>
#include <QFile>

#include <algorithm>

SoundEngine::SoundEngine(QObject *parent)
    : QObject{parent}
{

}

SoundEngine::~SoundEngine(){

    delete(csound);
    delete(csoundThread);

    delete(timer);
}

void SoundEngine::process(void)
{
    QSettings settings;
    int audioDeviceID = 0;// 0 is the default, but lets see if we have information in the preferences
    if (settings.contains("audioDevice")){
        audioDeviceID = settings.value("audioDevice").toInt();
    }

    // start Csound thread
    Csound *csound = new Csound();
    QString rtaudio;
#if (defined (Q_OS_MAC))
    rtaudio = "-+rtaudio=auhal";

#elif (defined (Q_OS_LINUX))
    rtaudio = "-+rtaudio=alsa";

#elif (defined (Q_OS_WIN)
    rtaudio = "-+rtaudio=portaudio";


#endif
    csound->SetOption(rtaudio.toStdString().c_str());

    QString dacOption = "-odac" + QString::number(audioDeviceID+1);
    csound->SetOption(dacOption.toStdString().c_str());

    QString csd;
#ifdef Q_OS_MAC
    csd = QCoreApplication::applicationDirPath() + "/../../../csoundIntruments.csd";
#else
    csd = "csoundIntruments.csd";
#endif

    emit parseCsound(csd);
    int result = csound->CompileCsd(qPrintable(csd));

    this->csound = csound;
    csound->SetDebug(0);

    if (!result) {

        csound->Start();
        CsoundThreaded* csoundThread = new CsoundThreaded(csound->GetCsound());
        this->csoundThread = csoundThread;
        csoundThread->Perform();
        qDebug()<<"this thread "<<QThread::currentThread();

        // setup sequencer clock
        currentBeat = 1;
        timer = new QTimer(this);
        timer->setTimerType(Qt::PreciseTimer);
        connect(timer,SIGNAL(timeout()),this,SLOT(seqStep()));
        setBPM(140);

    }
    else {
        emit status("Csound error");
    }
}

void SoundEngine::audioSet(){
    // shutting down former engine
    timer->stop();
    csoundThread->Stop();
    delete(csound);
    delete(csoundThread);
    delete(timer);

    // starting csound and the rest with the new audio setting
    process();
}

void SoundEngine::setBPM(int bpm){

    int rate = (int)(((60.f * 4.f) / (float)bpm) * 32.f); // clock resolution is 32 step
    if (rate == 0) rate = 1; // precautions
    timer->start(rate);
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
    //qDebug()<<"thread "<<QThread::currentThread()<< "main "<< QApplication::instance()->thread();
    QMapIterator<QString, Track> trackIterator(tracks);
    TracksType tickedTracks;
    while (trackIterator.hasNext()) {
        trackIterator.next();
        Track currentTrack = trackIterator.value();
        //qDebug()<<"process track: "<<trackIterator.key();
        QList<PatternEvent> patternEvents = currentTrack.getNextEvents();
        QMutableListIterator<PatternEvent> eventIterator(patternEvents);
        //QMap<QString,Parameter> updatedParameters;
        while (eventIterator.hasNext()){
            PatternEvent e = eventIterator.next();
            if (e.instrumentNumber > 0) {
                double duration = 1.;
                QMap<QString,Parameter*> parameters = e.parameters;
                QMutableMapIterator<QString,Parameter*> pIterator(parameters);
                int highestPNumber = 3;
                while (pIterator.hasNext()){
                    Parameter *p = pIterator.next().value();

                    if (p->pNumber == 3){ // always the duration
                        duration = p->getValue();
                        continue;
                    }
                    highestPNumber = qMax(highestPNumber,p->pNumber);
                }
                QList<double>pArray;
                pArray.append(e.instrumentNumber);
                pArray.append(0);
                pArray.append(duration);

                Parameter *p;
                for (int pIndex = 4; pIndex <= highestPNumber;pIndex++){
                    pIterator.toFront();
                    //QMap<QString,Parameter>::const_iterator citerator;
                    p = nullptr;

                    while (pIterator.hasNext()){
                        pIterator.next();
                        Parameter *pa = pIterator.value();
                        if (pa->pNumber == pIndex){
                            p = pa;
                            break;
                        }
                    }
                    if (p != nullptr){
                        pArray.append(p->getValue());
                        //pIterator.setValue(*p);
                        //*citerator->arrayIndex = *p->arrayIndex;
                        //updatedParameters[pIterator.key()] = *p;
                    }
                    else {
                        pArray.append(0.00000001);
                        qDebug()<<"found nullptr parameter for csound p "<<pIndex;
                    }
                }

                //const double parameters[3] = {e.instrumentNumber,0,1.};
                this->csoundThread->ScoreEvent('i', pArray.constData(), pArray.count());
                //qDebug()<<"trigger instr: "<< e.instrumentNumber;
            }

        }
        tickedTracks[trackIterator.key()] = currentTrack;
    }

    tracks.swap(tickedTracks);

    currentBeat++;
    if (currentBeat > 16) {
        currentBeat = 1;

        // check if we have schedulded changes
        if (hasParsedTracks) {
            tracks.swap(tempTracks);
            hasParsedTracks = false;
        }
    }
}

void SoundEngine::setInstrumentDefinitions(QMap<QString,InstrumentDefinition>instrumentDefinitions){
    instruments = instrumentDefinitions;
}

void SoundEngine::stop(){
    this->timer->stop();
    csoundThread->Stop();
    csoundThread->Join();
}
