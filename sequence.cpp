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
    this->tickCount = this->tickCount + 1;
    if (this->tickCount < this->ticksPerPattern) {
        PatternEvent pause;
        return pause;
    }
    this->tickCount = 0;
    PatternEvent nextEvent = pattern[this->index];
    this->index = this->index + 1;
    if (this->index == patternLength){
        this->index = 0;
        this->amountOfPlays++;
    }
    return nextEvent;
}
