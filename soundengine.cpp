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
    emit parseCsound(csd);
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
    //qDebug()<<"thread "<<QThread::currentThread()<< "main "<< QApplication::instance()->thread();
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
                double duration = 1.;
                QMap<QString,Parameter> parameters = e.parameters;
                QMapIterator<QString,Parameter> pIterator(parameters);
                int highestPNumber = 3;
                while (pIterator.hasNext()){
                    Parameter p = pIterator.next().value();

                    if (p.pNumber == 3){ // always the duration
                        duration = p.value;
                        continue;
                    }
                    highestPNumber = qMax(highestPNumber,p.pNumber);
                }
                QList<double>pArray;
                pArray.append(e.instrumentNumber);
                pArray.append(0);
                pArray.append(duration);
                Parameter *p;
                for (int pIndex = 4; pIndex <= highestPNumber;pIndex++){
                    pIterator.toFront();
                    p = nullptr;
                    while (pIterator.hasNext()){
                         Parameter pa = pIterator.next().value();
                         if (pa.pNumber == pIndex){
                          p = &pa;
                          break;
                         }
                    }
                    if (p != nullptr){
                        pArray.append(p->value);
                    }
                    else {
                        pArray.append(0);
                    }
                }

                //const double parameters[3] = {e.instrumentNumber,0,1.};
                this->perfThread->ScoreEvent( false,'i', pArray.count(), pArray.constData());
                qDebug()<<"trigger instr: "<< e.instrumentNumber;
            }
        }
        tickedTracks[trackIterator.key()] = currentTrack;
    }

    tracks.swap(tickedTracks);

}

void SoundEngine::setInstrumentDefinitions(QMap<QString,InstrumentDefinition>instrumentDefinitions){
    instruments = instrumentDefinitions;
}

