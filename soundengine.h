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

#include <QObject>
#include <QTimer>
#include <QMap>
#include <csound.hpp>
//#include <csound/csound.hpp>
#include <CsoundLib64/csound_threaded.hpp>
#include "track.h"
#include <QThread>
#include "instrumentdefinition.h"

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

private:
    Csound *csound;
    CsoundThreaded *csoundThread;
    QTimer *timer;
    //QList<int> patternA;
    //QList<int> patternB;
    QMap<QString,InstrumentDefinition> instruments;

    //bool playPatternA;
    //bool swapPattern;
    //int seqIndex;

protected:
    // void timerEvent(QTimerEvent *event) override;
public slots:
    void process();
    void setBPM(int bpm);

private slots:
    void audioSet();
    void seqStep();
    void setInstrumentDefinitions(QMap<QString,InstrumentDefinition>instrumentDefinitions);
signals:
    void display(QString message);
    void status(QString message);
    void parseCsound(QString fileName);
};

#endif // SOUNDENGINE_H
