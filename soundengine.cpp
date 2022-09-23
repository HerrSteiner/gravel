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
#include "QtCore/qcoreevent.h"
#include "QtCore/QDebug"
#include <QApplication>

SoundEngine::SoundEngine(QObject *parent)
    : QObject{parent}
{

}

void SoundEngine::process(void)
{
    qDebug() << "worker process";       //This works

    // start Csound thread
    Csound *csound = new Csound();
    std::string csd = "/Users/herrsteiner/Projects/gravel/csoundIntruments.csd";
    int result = csound->CompileCsd(csd.c_str());

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

void SoundEngine::setPattern(QList<int> pattern){
    if (playPatternA) patternB = pattern;
    else patternA = pattern;
    swapPattern = true;
}

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
