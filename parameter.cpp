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

#include "parameter.h"


Parameter::Parameter()
{

}

void Parameter::setRandomRange(double min,double max){
    if (min > max) {
        randomMax = min;
        randomMin = max;
    }
    else {
        randomMin = min;
        randomMax = max;
    }
}

double Parameter::getValue() {
    switch (mode) {
    default:
    case SINGLE:
        checkLimit(&value);
        return value;
        break; // unnecessary but I have OCD
    case ARRAY:
        if (valueArray.isEmpty()) return 0;
        if (arrayIndex < valueArray.count()) {
            double currentValue = valueArray[arrayIndex];
            arrayIndex++;
            checkLimit(&currentValue);
            return currentValue;
        }
        else {
            arrayIndex = 1; // reset to the index for the value we want to get NEXT time
            double currentValue = valueArray[0];
            checkLimit(&currentValue);
            return currentValue;
        }
        break;
    case PICK:
        if (valueArray.isEmpty()) return 0;
        {
        int pick = randomGenerator.bounded(valueArray.count());
        double currentValue = valueArray[pick];
        checkLimit(&currentValue);
        return currentValue;
        }
        break;
    case RANDOM:
        {
        double rMax = randomMax - randomMin;
        double zufall = randomGenerator.bounded(rMax);
        zufall += randomMin;
        checkLimit(&zufall);
        return zufall;
        }
        break;
    }
}

void Parameter::checkLimit(double *valueToCheck) {
    if (!hasLimit) return;
    if (*valueToCheck < limitMin) *valueToCheck = limitMin;
    if (*valueToCheck > limitMax) *valueToCheck = limitMax;
}

void Parameter::setLimitRange(double min,double max) {
    if (min > max) {
        limitMax = min;
        limitMin = max;
    }
    else {
        limitMin = min;
        limitMax = max;
    }
    hasLimit = true;
}
