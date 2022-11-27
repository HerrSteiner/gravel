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
//#include <csPerfThread.hpp>
#include "csPerfThread.hpp"
//#include "CsoundLib64/csPerfThread.hpp"
#include "track.h"
#include <QThread>
#include "instrumentdefinition.h"

typedef QMap<QString,Track> TracksType;
class SoundEngine : public QObject
{
    Q_OBJECT

public:
    explicit SoundEngine(QObject *parent = nullptr);
    //void setPattern(QList<int> pattern);
    TracksType tracks;

private:
    Csound *csound;
    CsoundPerformanceThread* perfThread;
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

private slots:
    void seqStep();
    void setInstrumentDefinitions(QMap<QString,InstrumentDefinition>instrumentDefinitions);
signals:
    void display(QString message);
    void parseCsound(QString fileName);
};

#endif // SOUNDENGINE_H
