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

#ifndef INSTRUMENTDEFINITION_H
#define INSTRUMENTDEFINITION_H
#import <QtCore>
#import "parameter.h"

class InstrumentDefinition
{
public:
    InstrumentDefinition();
    QString Name; // the name used in live code score
    int instrNumber; // the number of the Csound intrument
    QMap<QString,Parameter*> parameters; // list of parameters, mapped against their name to use in Gravel
};

#endif // INSTRUMENTDEFINITION_H
