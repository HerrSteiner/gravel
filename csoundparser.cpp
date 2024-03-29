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

#include "csoundparser.h"

typedef enum {
    NONEINSTRUMENT,
    INSTRUMENT,
    INSTRUMENTNUMBER,
    INSTRUMENTNAME,
    PARAMETER,
    PARAMETERNUMBER,
    PARAMETERNAME,
    PARAMETERVALUE,
    PARAMETERDESCRIPTION,
    PARAMETERRANGEMIN,
    PARAMETERRANGEMAX
} InstrumentStates;

CsoundParser::CsoundParser(QObject *parent)
    : QObject{parent}
{
}

void CsoundParser::parseCsound(QString fileName){
    instruments.clear();
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;
    QTextStream in(&file);
    QString line;
    InstrumentDefinition *currentInstrument = NULL;
    Parameter *currentParameter = NULL;
    QChar ch;
    QString currentToken;
    InstrumentStates state = NONEINSTRUMENT;
    double rangeMin = 0;

    while (!in.atEnd()){
        line = in.readLine().trimmed();
        if (state>PARAMETER) state = PARAMETER;
        if (state == INSTRUMENTNUMBER) state = NONEINSTRUMENT; // this was appearently a instrument which should be not included (like effects)
        // parse that line of csound code
        qsizetype lineLength = line.length();
        currentToken.clear();
        for(int i = 0; i < lineLength; i++)
        {
            ch = line.at(i);
            if (state == NONEINSTRUMENT){
                currentToken.append(ch);
                if (currentToken == "instr"){
                    state = INSTRUMENTNUMBER;
                    if (currentInstrument) delete currentInstrument;
                    currentInstrument = new InstrumentDefinition();
                    currentToken.clear();
                }
                continue;
            }
            if (state == INSTRUMENTNUMBER){
                if (ch.isDigit()){
                    currentToken.append(ch);
                    continue;
                }
                if (ch == ';'){
                    state = INSTRUMENTNAME;

                    currentInstrument->instrNumber = currentToken.toInt();
                    currentToken.clear();
                }
                continue;
            }
            if (state == INSTRUMENTNAME){
                currentToken.append(ch);
                if (ch == ' ' || i == lineLength - 1){
                    currentInstrument->Name = currentToken;
                    currentToken.clear();
                    state = PARAMETER;
                }
                continue;
            }
            if (state == PARAMETER){
                if (ch == '='){
                    state = PARAMETERNUMBER;
                    currentToken.clear();

                    currentParameter = new Parameter();

                    continue;
                }

                currentToken.append(ch); // actually fishing here for endin statement
                if (currentToken == "endin"){
                    if (currentInstrument){
                        instruments[currentInstrument->Name] = *currentInstrument;
                        currentToken.clear();
                        state = NONEINSTRUMENT;
                    }

                }
                continue;
            }
            if (state == PARAMETERNUMBER){
                if (ch == ';') {
                    currentParameter->pNumber = currentToken.toInt();
                    currentToken.clear();
                    state = PARAMETERNAME;
                    continue;
                }
                if (ch.isDigit()){
                    currentToken.append(ch);
                }
                continue;
            }
            if (state == PARAMETERNAME){
                if (ch == ' '){
                    currentParameter->Name = currentToken;
                    currentToken.clear();
                    state = PARAMETERVALUE;
                    continue;
                }
                currentToken.append(ch);
                continue;
            }
            if (state == PARAMETERVALUE){
                if (ch == '#') {
                    state = PARAMETERDESCRIPTION;
                    currentParameter->value = currentToken.toDouble();
                    currentToken.clear();
                    if (currentParameter && currentParameter->Name != nullptr){
                        //QMap<QString,Parameter*> parameters = currentInstrument->parameters;
                        currentInstrument->parameters[currentParameter->Name] = currentParameter;

                    }
                    continue;
                }
                if (ch.isDigit() || ch == '.'){
                    currentToken.append(ch);
                }
                if (i == lineLength -1){
                    currentParameter->value = currentToken.toDouble();
                    currentToken.clear();
                    state = PARAMETER;
                    if (currentParameter && currentParameter->Name != nullptr){
                        //QMap<QString,Parameter*> parameters = currentInstrument->parameters;
                        currentInstrument->parameters[currentParameter->Name] = currentParameter;

                    }
                }
                continue;
            }
            if (state == PARAMETERDESCRIPTION){

                if (ch =='!'){
                    state = PARAMETERRANGEMIN;
                    currentParameter->Description = currentToken;
                    currentToken.clear();
                    rangeMin = 0;
                    continue;
                }
                currentToken.append(ch);
                if (i == lineLength -1){
                    currentParameter->Description = currentToken;
                    currentToken.clear();
                    state = PARAMETER;
                    continue;
                }
                continue;
            }
            if (state == PARAMETERRANGEMIN){
                if (ch.isDigit() || ch == '.'){
                    currentToken.append(ch);
                }
                if (i == lineLength - 1){ // we don't set the range if we only have the minimum
                    state = PARAMETER;
                    continue;
                }
                if (ch == ' '){
                    rangeMin = currentToken.toDouble();
                    state = PARAMETERRANGEMAX;
                }
                continue;
            }
            if (state == PARAMETERRANGEMAX){
                if (ch.isDigit() || ch == '.'){
                    currentToken.append(ch);
                }
                if (i == lineLength - 1 || ch == ' '){ // we don't set the range if we only have the minimum
                    double rangeMax = currentToken.toDouble();
                    currentParameter->setLimitRange(rangeMin,rangeMax);
                    state = PARAMETER;
                }
                continue;
            }

        }
    }
    file.close();
    if (currentInstrument!=NULL) delete(currentInstrument);
    if (currentParameter!=NULL) delete(currentParameter);
    displayInstruments();
    emit setInstrumentDefinitions(instruments);
    return;
}

void CsoundParser::displayInstruments(){
    QString instrumentMessage;
    QMapIterator<QString,InstrumentDefinition> iterator(instruments);
    instrumentMessage.append("<html>load Csound file with following instrument definitions:<br><table border='1'>");

    while (iterator.hasNext()){
        InstrumentDefinition instrument = iterator.next().value();
        instrumentMessage.append("<tr><td colspan=3><b>instrument <span style='color:#666;'>#");

        instrumentMessage.append(QString::number(instrument.instrNumber));
        instrumentMessage.append("</span><span style='color:#66F;'> ");
        instrumentMessage.append(instrument.Name);
        instrumentMessage.append("</span> parameters:</b></td></tr>");
        QMapIterator<QString,Parameter*> parameterIterator(instrument.parameters);
        while (parameterIterator.hasNext()) {
            parameterIterator.next();
            instrumentMessage.append(tr("<tr><td>parameter <span style='color:#66F;'>"));
            instrumentMessage.append(parameterIterator.key());
            instrumentMessage.append("</span></td><td>pNumber: <span style='color:#888;'>");
            Parameter *parameter = parameterIterator.value();
            instrumentMessage.append(QString::number(parameter->pNumber));
            instrumentMessage.append("</span></td><td>defaultValue: <span style='color:#888;'>");
            instrumentMessage.append(QString::number(parameter->value));
            instrumentMessage.append("</span></td><td><span style='color:#888;'>");
            instrumentMessage.append(parameter->Description);
            instrumentMessage.append("</span></td><td>range: <span style='color:#888;'>");
            if (parameter->hasLimit){
                instrumentMessage.append("</span></td><td>range: <span style='color:#888;'>");
                instrumentMessage.append(QString::number(parameter->limitMin));
                instrumentMessage.append(" - ");
                instrumentMessage.append(QString::number(parameter->limitMax));
            }
            else {
                instrumentMessage.append("</span></td><td><span>");
            }
            instrumentMessage.append("</span></td></tr>");
        }

    }
    instrumentMessage.append("</table><br>");
    instrumentMessage.append("</html>");
    emit display(instrumentMessage);
}


