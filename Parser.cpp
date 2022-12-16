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



Parser::Parser(QObject *parent)
    : QObject{parent}
{

}

void Parser::parseCode(QString code){
    emit status("parsing...");
    state = NONE;
    QChar ch;
    QString trackName,divisor,instrumentName,instrumentParameter,instrumentParameterValue;

    QMap<QString,Parameter*> instrumentParameters;
    QMap<QString, QMap<QString,Parameter*> > formerParametersByInstrumentName;
    int amountTriggers = 1;
    QString amountString;
    QList<double>valueList;
    ParameterMode parameterMode = SINGLE;
    this->code = code;
    codeLength = code.length();
    characterIndex = 0;
    error.clear();
    message.clear();
    do {
        ch = code.at(characterIndex);
        characterIndex++;

        if (ch == 't' && state == NONE){
            state = TRACKNAME;
            trackName = parseTrackName();
            if (trackName.isEmpty()) {
                error = "trackname error";
            }
            continue;
        }
        if (ch == 's' && state == NONE){
            parseStop();
            continue;
        }
        if (ch == 'b' && state == NONE){
            parseBPM();
            continue;
        }
        if (ch == '[' && state == TRACK) {
            state = SEQUENCE;
            currentPattern = new PatternType();
            continue;
        }
        if (ch == '{' && state == TRACK) {
            state = EUCLID;
            currentPattern = new PatternType();
            continue;
        }
        if (state == SEQUENCE || state == EUCLID || state == INSTRUMENTPARAMETERVALUE || state == TRIGGER || state == INSTRUMENTPARAMETER || state == AMOUNT){
            if (ch == ',' || ch == '}' || ch == ']') {
                // first handle possible existing intrument parameter
                if (state == INSTRUMENTPARAMETERVALUE && !instrumentParameter.isEmpty() && (!instrumentParameterValue.isEmpty() || !valueList.isEmpty())){
                    Parameter *iParameter = new Parameter();
                    iParameter->Name = instrumentParameter;
                    switch (parameterMode) {
                    default:
                    case SINGLE:
                        iParameter->value = instrumentParameterValue.toDouble();
                        break;
                    case ARRAY:
                        iParameter->valueArray = valueList;
                        iParameter->mode = ARRAY;
                        break;
                    case PICK:
                        iParameter->valueArray = valueList;
                        iParameter->mode = PICK;
                        break;
                    case RANDOM:
                    {
                        double min = 0;
                        double max = 1;
                        if (valueList.count()>1){
                            min = valueList.first();
                            max = valueList.last();
                        }
                        iParameter->setRandomRange(min,max);
                        iParameter->mode = RANDOM;
                    }
                        break;
                    }

                    instrumentParameters[instrumentParameter] = iParameter;

                    QMap<QString,Parameter*> formerParameters = formerParametersByInstrumentName[instrumentName];
                    formerParameters.remove(instrumentParameter);
                    //formerParameters[instrumentParameter] = iParameter;
                    formerParametersByInstrumentName[instrumentName] = formerParameters;

                }

                PatternEvent p; // if no instrument is given it counts as a pause
                if (instruments.contains(instrumentName)){
                    InstrumentDefinition instrument = instruments[instrumentName];
                    p.instrumentNumber = instrument.instrNumber;
                    p.instrumentName = instrument.Name;
                    // fill in default parameters
                    p.parameters = instrument.parameters;// .parameters;

                    // replace parametervalues with possible set values
                    QMapIterator<QString,Parameter*> parameterIterator(instrumentParameters);
                    while (parameterIterator.hasNext()){
                        parameterIterator.next();
                        if (p.parameters.contains(parameterIterator.key())){ // only known parameters are set
                            Parameter *iParameter = parameterIterator.value();
                            iParameter->pNumber = p.parameters[iParameter->Name]->pNumber;// set the Csound parameter number on the new parameter
                            /*if (p.parameters.contains(iParameter->Name)) {
                                p.parameters.remove(iParameter->Name);
                            }*/
                            p.parameters[iParameter->Name] = iParameter;


                        }
                    }
                }


                // applying the patternevent
                for (int amount = 0; amount < amountTriggers;amount++){
                    currentPattern->append(p);
                }
                amountTriggers = 1; // per default only one note or pause is set

                instrumentName.clear();
                instrumentParameters.clear();
                state = TRIGGER;

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
                        parameterMode = SINGLE;
                    }

                    else if (ch == '$') {
                        // overide parameters with the last set ones
                        if (formerParametersByInstrumentName.contains(instrumentName)){
                            QMap<QString,Parameter*> formerParameters = formerParametersByInstrumentName[instrumentName];
                            if (!formerParameters.isEmpty()){
                                qDebug()<<formerParameters;
                                instrumentParameters.insert(formerParameters);
                            }
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
                    if (state == INSTRUMENTPARAMETERVALUE && !instrumentParameter.isEmpty()){ // this parameter is precedet by another one, so store that first
                        Parameter *iParameter = new Parameter();
                        iParameter->Name = instrumentParameter;
                        switch (parameterMode) {
                        default:
                        case SINGLE:
                            iParameter->value = instrumentParameterValue.toDouble();
                            break;
                        case ARRAY:
                            iParameter->valueArray = valueList;
                            iParameter->mode = ARRAY;
                            break;
                        case PICK:
                            iParameter->valueArray = valueList;
                            iParameter->mode = PICK;
                            break;
                        case RANDOM:
                        {
                            double min = 0;
                            double max = 1;
                            if (valueList.count()>1){
                                min = valueList.first();
                                max = valueList.last();
                            }
                            iParameter->setRandomRange(min,max);
                            iParameter->mode = RANDOM;
                        }
                            break;
                        }

                        instrumentParameters[instrumentParameter] = iParameter;
                        QMap<QString,Parameter*> formerParameters = formerParametersByInstrumentName[instrumentName];
                        formerParameters.remove(instrumentParameter);
                        formerParameters[instrumentParameter] = iParameter;
                        formerParametersByInstrumentName[instrumentName] = formerParameters;
                    }
                    state = INSTRUMENTPARAMETER;
                    instrumentParameter.clear();
                    instrumentParameterValue.clear();
                    continue;
                }
                else if (state == INSTRUMENTPARAMETERVALUE) {
                    if (ch=='[') {
                        state = INSTRUMENTPARAMETERVALUELIST;
                        valueList.clear();
                        instrumentParameterValue.clear();
                        parameterMode = ARRAY;
                        continue;
                    }
                    if (ch=='?') {
                        state = INSTRUMENTPARAMETERVALUERANDOM;
                        valueList.clear();
                        instrumentParameterValue.clear();
                        parameterMode = RANDOM;
                        continue;
                    }


                    if (ch.isDigit() || ch == '.'){
                        instrumentParameterValue.append(ch);
                    }
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

        if (state == INSTRUMENTPARAMETERVALUELIST){
            if (ch == ','){
                valueList.append(instrumentParameterValue.toDouble());
                instrumentParameterValue.clear();
                continue;
            }
            if (ch == ']'){
                state = INSTRUMENTPARAMETERVALUE;
                if (!instrumentParameterValue.isEmpty()){
                    valueList.append(instrumentParameterValue.toDouble());
                    instrumentParameterValue.clear();
                }
                continue;
            }
            if (ch.isDigit() || ch == '.'){
                instrumentParameterValue.append(ch);
            }
            continue;
        }

        if (state == INSTRUMENTPARAMETERVALUERANDOM){
            if (ch=='['){
                state = INSTRUMENTPARAMETERVALUEPICK;
                parameterMode = PICK;
                continue;
            }
            if (ch=='('){
                continue;
            }
            if (ch == ','){
                valueList.append(instrumentParameterValue.toDouble());
                instrumentParameterValue.clear();
                continue;
            }
            if (ch == ')'){
                state = INSTRUMENTPARAMETERVALUE;
                if (!instrumentParameterValue.isEmpty()){
                    valueList.append(instrumentParameterValue.toDouble());
                    instrumentParameterValue.clear();
                }
                continue;
            }
            if (ch.isDigit() || ch == '.'){
                instrumentParameterValue.append(ch);
            }
            continue;
        }
        if (state == INSTRUMENTPARAMETERVALUEPICK){
            if (ch == ','){
                valueList.append(instrumentParameterValue.toDouble());
                instrumentParameterValue.clear();
                continue;
            }
            if (ch == ']'){
                state = INSTRUMENTPARAMETERVALUE;
                if (!instrumentParameterValue.isEmpty()){
                    valueList.append(instrumentParameterValue.toDouble());
                    instrumentParameterValue.clear();
                }
                continue;
            }
            if (ch.isDigit() || ch == '.'){
                instrumentParameterValue.append(ch);
            }
            continue;
        }

        if (state == TRACKPARAMETER){
            if (ch=='/'){
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
                error = "error creating sequence";
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
                    error = "error creating sequence";
                }
            }

            qDebug()<<"wrapping";
            if (state == TRACK){
                if (currentTrack != nullptr && sequences != nullptr){
                    state = NONE;
                    currentTrack->setSequences(*sequences);
                    tracks[trackName] = *currentTrack;
                }
                continue;
            }

        }

        if (state == TRACKPARAMETER){ // should be at the end, catch all digits
            if (ch.isDigit()){
                divisor.append(ch);
                continue;
            }
        }
    }
    while (characterIndex < codeLength && error.length() == 0);
    if (error.length() > 0) {
        emit status(error);
    }
    else {
        if (message.length() == 0) message = "parsing successful";
        emit status(message);
    }
}

/**
 * @brief Parser::parseTrackName
 * parses trackname and initialize further track properties in case of success
 * @return trackname or empty string
 */
QString Parser::parseTrackName(){
    state = TRACKNAME;
    QChar ch;
    QString trackName;
    do {
        ch = code.at(characterIndex);
        characterIndex++;
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
            return trackName;// we got a trackname and setup everything for things to come
        }
        trackName.append(ch);
    }
    while (characterIndex < codeLength);
    // if we end up here then there was no valid trackname so return empty string to signalise error
    return "";
}

void Parser::parseStop(){
    QString stopToken,stopParameter;
    QStringList stopParameters;
    QChar ch;
    state = STOP;
    stopToken.clear();
    stopToken.append('s');
    do {
        ch = code.at(characterIndex);
        characterIndex++;
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
    }    while (characterIndex < codeLength);
}

void Parser::parseBPM(){
    QString bpmToken,bpmParameter;
    QChar ch;
    state = BPM;
    bpmToken.clear();
    bpmToken.append('b');
    do {
        ch = code.at(characterIndex);
        characterIndex++;
        if (state == BPM){
            bpmToken.append(ch);
            if (bpmToken == "bpm"){
                state = BPMPARAMETER;
                bpmParameter.clear();
            }
            continue;
        }
        if (state == BPMPARAMETER){
            if (ch== ';') {
                int bpm = bpmParameter.toInt();
                emit setBPM(bpm);
                message = "set bpm to "+bpmParameter;
                return;
            }

            if (ch >= '0' && ch<='9'){
                bpmParameter.append(ch);
            }
            continue;
        }
    }    while (characterIndex < codeLength);
    error = ("error setting the bpm");
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

void Parser::setInstrumentDefinitions(QMap<QString,InstrumentDefinition>instrumentDefinitions){
    instruments = instrumentDefinitions;
}
