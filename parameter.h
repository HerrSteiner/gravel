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

#ifndef PARAMETER_H
#define PARAMETER_H

#import <QRandomGenerator>
#import <QString>
#import <QLIST>

typedef enum {
    SINGLE,
    ARRAY,
    RANDOM
} ParameterMode;
class Parameter
{

public:

    Parameter();
    ParameterMode mode = SINGLE;
    QString Name;
    int pNumber; // the number of the p in Csound
    double value;
    QList <double> valueArray;
    void setRandomRange(double min,double max);
    int arrayIndex = 0;
    double getValue();
private:
    QRandomGenerator randomGenerator;

    double randomMax = 1;
    double randomMin = 0;
};

#endif // PARAMETER_H
