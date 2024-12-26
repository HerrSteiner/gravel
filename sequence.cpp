/*
   gravel - live coding music system -
   Copyright (C) 2022-2024 Malte Steiner

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

#include "sequence.h"
#include "QtCore/QDebug"

Sequence::Sequence()
{
    this->ticksPerPattern = 1;
}

void Sequence::setPattern(PatternType pattern) {
    this->pattern = pattern;
    patternLength = pattern.length();
    index = amountOfPlays = tickCount = 0;
    qDebug()<<"set sequence pattern";
}

PatternEvent Sequence::nextStep(){
    if (pattern.empty()) {
        PatternEvent pause;
        return pause;
    }

    if (this->tickCount > 0){ // to get to the next event, we need more ticks
        this->tickCount++;
        if (this->tickCount == this->ticksPerPattern) {
            this->tickCount = 0;
        }
        PatternEvent pause;
        return pause;
    }

    PatternEvent nextEvent = pattern[this->index];
    this->index++;
    if (this->index == patternLength){
        this->index = 0;
        this->amountOfPlays++;
    }

    this->tickCount++;
    if (this->tickCount == this->ticksPerPattern) {
        this->tickCount = 0;
    }
    return nextEvent;
}
