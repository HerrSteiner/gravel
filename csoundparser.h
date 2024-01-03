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

#ifndef CSOUNDPARSER_H
#define CSOUNDPARSER_H

#include <QObject>
#include <QMap>
#include "instrumentdefinition.h"


// Parser for Csound instrument files to obtain a list of instrument names and parameters
// only invoked once at program start
class CsoundParser : public QObject
{
    Q_OBJECT
public:
    explicit CsoundParser(QObject *parent = nullptr);
public slots:
    void parseCsound(QString fileName);
private:
     void displayInstruments();
     QMap<QString,InstrumentDefinition> instruments;
signals:
    void setInstrumentDefinitions(QMap<QString,InstrumentDefinition>instrumentDefinitions);
    void display(QString message);
};

#endif // CSOUNDPARSER_H
