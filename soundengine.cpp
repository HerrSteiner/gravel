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


SoundEngine::SoundEngine(QObject *parent)
    : QObject{parent}
{
    Csound *csound = new Csound();//csoundCreate(0);
    std::string csd = "/Users/herrsteiner/Projects/gravel/csoundIntruments.csd";
    int result = csound->CompileCsd(csd.c_str());
    //int result = csoundCompileCsd(csound, csd);
    this->csound = csound;
    csound->SetDebug(0);

    if (!result) {


    csound->Start();
    CsoundPerformanceThread* perfThread = new CsoundPerformanceThread(csound->GetCsound());
    this->perfThread = perfThread;
    perfThread->Play();
    //perfThread->
    /*
    QTimer timer;
    timer.setInterval(500);
    timer.setTimerType(Qt::PreciseTimer);
*/
    /*
    playPatternA = true;
    swapPattern = false;
    patternA = {1,0,2,0,1,1,0,2,1,0,0,1,0,0,1,0};
    //this->patternA = patternA;
    seqIndex = 0;
    */

    //60*4 / BPM = 1 Bar in s
    int rate =  54;
    startTimer(rate,Qt::PreciseTimer);

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

void SoundEngine::timerEvent(QTimerEvent *event)
{
    //qDebug() << "Timer ID:" << event->timerId()<<" index "<<seqIndex;
    /*
    int i = playPatternA ? patternA[seqIndex]: patternB[seqIndex];
    seqIndex++;
    if (seqIndex>15){
        seqIndex = 0;
        if (swapPattern){
            playPatternA = !playPatternA;
            swapPattern = false;
        }
    }
    if (i == 0) return;
*/
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
