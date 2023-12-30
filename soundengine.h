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

#ifndef SOUNDENGINE_H
#define SOUNDENGINE_H

#include <QtGlobal>
#include <QObject>
#include <QTimer>
#include <QUdpSocket>
#include <QMap>
#include <csound.hpp>

#if (defined (Q_OS_MAC))
#include <CsoundLib64/csound_threaded.hpp>
#else
#include <csound_threaded.hpp>
#endif
#include "track.h"
#include <QThread>
#include "instrumentdefinition.h"

// the map string trackname / Track, trackname is usergiven
typedef QMap<QString,Track> TracksType;

// the soundengine holds the Csound thread and performs the sequence duties, triggering the instruments
class SoundEngine : public QObject
{
    Q_OBJECT

public:
    explicit SoundEngine(QObject *parent = nullptr);
    ~SoundEngine();
    //void setPattern(QList<int> pattern);
    TracksType tracks;
    TracksType parsedTracks;
    TracksType tempTracks;
    bool hasParsedTracks = false;

private:
    Csound *csound;
    CsoundThreaded *csoundThread;
    QTimer *timer;
    //QList<int> patternA;
    //QList<int> patternB;
    QMap<QString,InstrumentDefinition> instruments;
    int bpm = 120;
    int currentBeat; // current beat from 1 - 32, for quantized sequence update
    //bool playPatternA;
    //bool swapPattern;
    //int seqIndex;
    bool sendingSync = false;
    bool receivingSync = false;
    int syncPort;
    QUdpSocket *udpSocket;

protected:
    // void timerEvent(QTimerEvent *event) override;
public slots:
    void process();
    void setBPM(int bpm);

private slots:
    void audioSet();
    void seqStep();
    void setInstrumentDefinitions(QMap<QString,InstrumentDefinition>instrumentDefinitions);
    void stop();
    void syncListen(int port);
    void syncSend(int port);
    void syncStop();
    void processPendingDatagrams();

signals:
    void display(QString message);
    void status(QString message);
    void parseCsound(QString fileName);
};

#endif // SOUNDENGINE_H
