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

#ifndef PARSER_H
#define PARSER_H
#include "soundengine.h"
#include <QObject>
#include <QMap>

typedef enum {
    NONE,
    TOKEN,
    TRACK,
    TRACKPARAMETER,
    TRACKNAME,
    SEQUENCE,
    EUCLID,
    TRIGGER,
    AMOUNT,
    INSTRUMENTPARAMETER,
    INSTRUMENTPARAMETERVALUE,
    STOP,
    STOPPARAMETER,
    BPM,
    BPMPARAMETER
} States;

class Parser : public QObject
{
    Q_OBJECT
public:
    explicit Parser(QObject *parent = nullptr);
    void parseCode(QString code);
    TracksType tracks;
public slots:
    //void parseCsound(QString fileName);
private:
    void fillPattern(PatternType *pattern,int max = 16);
    void displayInstruments();
    QMap<QString,InstrumentDefinition> instruments;
    States state = NONE;
    QString code,error,message;
    int characterIndex;
    int codeLength;
    Track *currentTrack = nullptr;
    PatternType *currentPattern = nullptr;
    SequencesType *sequences = nullptr;

    void parseStop();
    void parseBPM();
    QString parseTrackName();

private slots:
    void setInstrumentDefinitions(QMap<QString,InstrumentDefinition>instrumentDefinitions);

signals:
    void display(QString message);
    void status(QString message);
    void setBPM(int bpm);
};

#endif // PARSER_H
