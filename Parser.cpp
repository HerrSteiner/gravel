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

#include "Parser.h"
#include "QtCore/QDebug"

typedef enum {
    NONE,
    TOKEN,
    TRACK,
    TRACKPARAMETER,
    TRACKNAME,
    SEQUENCE,
    EUCLID,
    TRIGGER,
    AMOUNT,
    INSTRUMENTPARAMETER,
    INSTRUMENTPARAMETERVALUE,
    STOP,
    STOPPARAMETER
} States;

typedef enum {
    NONEINSTRUMENT,
    INSTRUMENT,
    INSTRUMENTNUMBER,
    INSTRUMENTNAME,
    PARAMETER,
    PARAMETERNUMBER,
    PARAMETERNAME,
    PARAMETERVALUE
} InstrumentStates;

Parser::Parser(QObject *parent)
    : QObject{parent}
{

}

void Parser::parseCode(QString code){
    States state = NONE;
    QChar ch;
    QString trackName,stopToken,stopParameter,divisor,instrumentName,instrumentParameter,instrumentParameterValue;
    QStringList stopParameters;
    Track *currentTrack = nullptr;
    PatternType *currentPattern = nullptr;
    SequencesType *sequences = nullptr;
    QMap<QString,Parameter> instrumentParameters;
    QMap<QString, QMap<QString,Parameter> > formerParametersByInstrumentName;
    int amountTriggers = 1;
    QString amountString;

    for(int i = 0; i< code.length(); i++)
    {
        ch = code.at(i);
        if (ch == 't' && state == NONE){
            state = TRACKNAME;
            trackName.clear();
            continue;
        }
        if (ch == 's' && state == NONE){
            state = STOP;
            stopToken.clear();
            stopToken.append(ch);
            continue;
        }
        if (ch == '[' && !trackName.isEmpty()) {
            state = SEQUENCE;
            currentPattern = new PatternType();
            continue;
        }
        if (ch == '{' && !trackName.isEmpty()) {
            state = EUCLID;
            currentPattern = new PatternType();
            continue;
        }
        if (!trackName.isEmpty() && (state == SEQUENCE || state == EUCLID || state == INSTRUMENTPARAMETERVALUE || state == TRIGGER || state == INSTRUMENTPARAMETER || state == AMOUNT)){
            if (ch == ',' || ch == '}' || ch == ']') {
                // first handle possible existing intrument parameter
                if (state == INSTRUMENTPARAMETERVALUE && !instrumentParameter.isEmpty() && !instrumentParameterValue.isEmpty()){
                    Parameter iParameter;
                    iParameter.Name = instrumentParameter;
                    iParameter.value = instrumentParameterValue.toDouble();
                    instrumentParameters[instrumentParameter] = iParameter;
                    //if (formerParametersByInstrumentName.contains(instrumentName)){
                    QMap<QString,Parameter> formerParameters = formerParametersByInstrumentName[instrumentName];
                    formerParameters[instrumentParameter] = iParameter;
                    formerParametersByInstrumentName[instrumentName] = formerParameters;
                    //}
                }

                PatternEvent p; // if no instrument is given it counts as a pause
                if (instruments.contains(instrumentName)){
                    InstrumentDefinition instrument = instruments[instrumentName];
                    p.instrumentNumber = instrument.instrNumber;
                    p.instrumentName = instrument.Name;
                    // fill in default parameters
                    p.parameters = instrument.parameters;
                    //QMapIterator<QString,Parameter> iterator(instrument.parameters);
                    /*
                    while (iterator.hasNext()){
                        p.parameters.append(iterator.next().value());
                    }*/
                    // replace parametervalues with possible set values
                    QMapIterator<QString,Parameter> parameterIterator(instrumentParameters);
                    while (parameterIterator.hasNext()){
                        parameterIterator.next();
                        if (p.parameters.contains(parameterIterator.key())){ // only known parameters are set
                            Parameter iParameter = parameterIterator.value();
                            p.parameters[iParameter.Name].value = iParameter.value;
                        }
                    }
                }

                instrumentName.clear();
                instrumentParameters.clear();
                for (int amount = 0; amount < amountTriggers;amount++){
                    currentPattern->append(p);
                }
                amountTriggers = 1; // per default only one note or pause is set

                //if (ch == ','){
                state = TRIGGER;
                //    continue;
                //}
                if (ch == '}') {
                    state = TRACKPARAMETER;
                    continue;
                }
                if (ch == ']') {
                    state = TRACK;
                    if ((currentPattern != nullptr) && (sequences != nullptr)){
                        Sequence seq;
                        seq.setPattern(*currentPattern);
                        sequences->append(seq);
                    }
                    else {
                        qDebug()<<"error creating sequence";
                    }
                    continue;
                }

                continue;
            }

            if (ch != ' ') {
                if (state == INSTRUMENTPARAMETER) {
                    if (ch == ':'){
                        state = INSTRUMENTPARAMETERVALUE;
                    }

                    else if (ch == '$') {
                        // overide parameters with the last set ones
                        if (formerParametersByInstrumentName.contains(instrumentName)){
                            QMap<QString,Parameter> formerParameters = formerParametersByInstrumentName[instrumentName];
                            instrumentParameters = formerParameters;
                            state = INSTRUMENTPARAMETERVALUE;// so that other parameter still are possible after the former ones are written
                            instrumentParameter.clear();// to make sure nothing is accidently set
                        }
                    }

                    else {
                        instrumentParameter.append(ch);
                    }
                    continue;
                }

                else if (ch == '#'){ // a new parameter is given
                    if (state == INSTRUMENTPARAMETERVALUE && !instrumentParameter.isEmpty() && !instrumentParameterValue.isEmpty()){ // this parameter is precedet by another one, so store that first
                        Parameter iParameter;
                        iParameter.Name = instrumentParameter;
                        iParameter.value = instrumentParameterValue.toDouble();
                        instrumentParameters[instrumentParameter] = iParameter;
                        QMap<QString,Parameter> formerParameters = formerParametersByInstrumentName[instrumentName];
                        formerParameters[instrumentParameter] = iParameter;
                        formerParametersByInstrumentName[instrumentName] = formerParameters;
                    }
                    state = INSTRUMENTPARAMETER;
                    instrumentParameter.clear();
                    instrumentParameterValue.clear();
                    continue;
                }
                else if (state == INSTRUMENTPARAMETERVALUE) {
                    instrumentParameterValue.append(ch);
                    continue;
                }
                else if (ch == '*' && (state == TRIGGER || state == SEQUENCE || state == EUCLID)){
                    state = AMOUNT;
                    amountString.clear();
                    continue;
                }
                else if (state == AMOUNT){
                    amountString.append(ch);
                    amountTriggers = amountString.toInt();
                    continue;
                }

                else {
                    instrumentName.append(ch);
                    continue;
                }
            }
        }


        if (state == TRACKPARAMETER){
            if (ch=='$'){
                divisor.clear();
                continue;
            }
        }

        if ( ch == ',' && state == TRACKPARAMETER){
            if ((currentPattern != nullptr) && (sequences != nullptr)){
                if (!divisor.isEmpty()){
                    int divNumber = divisor.toInt();
                    fillPattern(currentPattern,divNumber);
                    divisor.clear();
                }
                else {
                    fillPattern(currentPattern);
                }
                Sequence seq;

                seq.setPattern(*currentPattern);
                sequences->append(seq);
                state = TRACK;
            }
            else {
                qDebug()<<"error creating sequence";
            }
            continue;
        }
        if (ch == ';'){
            if (state == TRACKPARAMETER){
                if ((currentPattern != nullptr) && (sequences != nullptr)){
                    if (!divisor.isEmpty()){
                        int divNumber = divisor.toInt();
                        fillPattern(currentPattern,divNumber);
                        divisor.clear();
                    }
                    else {
                        fillPattern(currentPattern);
                    }
                    Sequence seq;

                    seq.setPattern(*currentPattern);
                    sequences->append(seq);
                    state = TRACK;
                }
                else {
                    qDebug()<<"error creating sequence";
                }
            }

            qDebug()<<"wrapping";
            if (state == TRACK){
                if (currentTrack != nullptr){
                state = NONE;
                currentTrack->setSequences(*sequences);
                tracks[trackName] = *currentTrack;
                }
                continue;
            }

        }

        if (state == TRACKNAME){
            if (ch == ':'){
                state = TRACK;
                if (tracks.contains(trackName)){
                    currentTrack = &tracks[trackName];
                }
                else {
                    currentTrack = new Track();
                    currentTrack->name = trackName;
                }
                sequences = new SequencesType();
                continue;
            }
            trackName.append(ch);
            continue;
        }
        if (state == STOP){
            stopToken.append(ch);
            if (stopToken == "stop"){
                state = STOPPARAMETER;
                stopParameter.clear();
                stopParameters.clear();
            }
            continue;
        }
        if (state == STOPPARAMETER){
            if (ch== ';') {
                if (stopParameter.length() > 0){ // taking the last stopparameter into account
                    stopParameters.append(stopParameter);
                    stopParameter.clear();
                }
                if (stopParameters.empty()){ // general stop
                    tracks.clear();
                    break;// no need to go further
                }
                else {
                    QStringListIterator trackname(stopParameters);
                    while (trackname.hasNext()){
                        tracks.remove(trackname.next()); // stop that track be removing it from the tracks
                    }
                    continue;
                }
            }
            if (ch == ','){
                stopParameters.append(stopParameter);
                stopParameter.clear();
                continue;
            }
            if (ch == ' '){ // ignoring space
                continue;
            }
            stopParameter.append(ch);
            continue;
        }

        if (state == TRACKPARAMETER){ // should be at the end, catch all digits
            if (ch.isDigit()){
                divisor.append(ch);
                continue;
            }
        }
    }
}

void Parser::fillPattern(PatternType *pattern, int max){
    int length = pattern->length();
    if (length<max){
        int steps = max / length;
        PatternType finalPattern;
        int pIndex = 0;
        for (int index = 0;index<max;index+=steps){
            if (pIndex<length){
                finalPattern.append(pattern->at(pIndex));
                pIndex++;
                for (int j = 0;j<steps-1;j++){
                    PatternEvent e;
                    finalPattern.append(e);
                }
            }
            else {PatternEvent e;
                finalPattern.append(e);
            }
        }
        *pattern =  finalPattern;
    }
}

void Parser::parseCsound(QString fileName){
    instruments.clear();
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;
    QTextStream in(&file);
    QString line;
    InstrumentDefinition *currentInstrument;
    Parameter *currentParameter;
    QChar ch;
    QString currentToken;
    InstrumentStates state = NONEINSTRUMENT;

    while (!in.atEnd()){
        line = in.readLine().trimmed();
        if (state>PARAMETER) state = PARAMETER;
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
                if (ch.isDigit() || ch == '.'){
                    currentToken.append(ch);
                }
                if (i == lineLength -1){
                    currentParameter->value = currentToken.toDouble();
                    currentToken.clear();
                    state = PARAMETER;
                    if (currentParameter && currentParameter->Name != nullptr){
                        currentInstrument->parameters[currentParameter->Name] = *currentParameter;
                    }
                }
                continue;
            }

        }
    }
    file.close();
    displayInstruments();
    emit setInstrumentDefinitions(instruments);
    return;
}

void Parser::displayInstruments(){
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
        QMapIterator<QString,Parameter> parameterIterator(instrument.parameters);
        while (parameterIterator.hasNext()) {
            parameterIterator.next();
            instrumentMessage.append(tr("<tr><td>parameter <span style='color:#66F;'>"));
            instrumentMessage.append(parameterIterator.key());
            instrumentMessage.append("</span></td><td>pNumber: <span style='color:#666;'>");
            Parameter parameter = parameterIterator.value();
            instrumentMessage.append(QString::number(parameter.pNumber));
            instrumentMessage.append("</span></td><td>defaultValue: <span style='color:#666;'>");
            instrumentMessage.append(QString::number(parameter.value));
            instrumentMessage.append("</span></td></tr>");
        }

    }
    instrumentMessage.append("</table><br>");
    instrumentMessage.append("</html>");
    emit display(instrumentMessage);
}
