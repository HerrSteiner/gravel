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

#ifndef PATTERNEVENT_H
#define PATTERNEVENT_H

#include "parameter.h"
#include <QMap>

// defines a single event in a pattern, which triggers a note in one of the predefined synthesizers
// a pattern consists of one or several events.
class PatternEvent
{

public:
    PatternEvent();
    double instrumentNumber;
    QString instrumentName;
    QMap<QString,Parameter*> parameters;
};

#endif // PATTERNEVENT_H
