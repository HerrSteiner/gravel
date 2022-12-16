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

#ifndef SEQUENCE_H
#define SEQUENCE_H

#include <QObject>
#include "patternEvent.h"

typedef QList<PatternEvent> PatternType;

// a sequence encapsulates a pattern and some additional data which is used at playback
class Sequence
{

public:
    Sequence();
    void setPattern(PatternType pattern);
    int patternLength;
    int index;
    int ticksPerPattern;
    int amountOfPlays;
    PatternEvent nextStep();

private:
    PatternType pattern;

};

#endif // SEQUENCE_H
