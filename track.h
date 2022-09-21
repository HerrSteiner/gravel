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
#ifndef TRACK_H
#define TRACK_H

#include "patternEvent.h"
#include "sequence.h"
#include <QObject>

typedef QList<Sequence> SequencesType;

class Track
{

public:
    Track();
    QString name;
    SequencesType sequencesA;
    SequencesType sequencesB;
    void setSequences(SequencesType sequences);
    QList<PatternEvent>getNextEvents();

private:
    bool playSequencesA;
    bool swapSequences;

};

#endif // TRACK_H
