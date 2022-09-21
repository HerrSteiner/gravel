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
    TRACKNAME,
    SEQUENCE,
    EUCLID,
    STOP
} States;

Parser::Parser(QObject *parent)
    : QObject{parent}
{

}

void Parser::parseCode(QString code){
    States state = NONE;
    QChar ch;
    QString trackName,stopToken;
    Track *currentTrack;
    PatternType *currentPattern;
    SequencesType *sequences;
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
        if (ch == '[') {
            state = SEQUENCE;
            currentPattern = new PatternType();
            continue;
        }
        if (ch == '{') {
            state = EUCLID;
            currentPattern = new PatternType();
            continue;
        }
        if (state == SEQUENCE || state == EUCLID){
            if (ch.isNumber()) {
                PatternEvent p;
                p.instrumentNumber = (double) ch.digitValue();
                currentPattern->append(p);
                continue;
            }
        }
        if (ch == '}') {
            state = TRACK;
            if ((currentPattern != nullptr) && (sequences != nullptr)){
            Sequence seq;
            fillPattern(currentPattern);
            seq.setPattern(*currentPattern);
            sequences->append(seq);
            }
            else {
                qDebug()<<"error creating sequence";
            }
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
        if (ch == ';'){
            qDebug()<<"wrapping";
            if (state == TRACK){
                state = NONE;
                currentTrack->setSequences(*sequences);
                tracks[trackName] = *currentTrack;
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
            if (ch== ';') {
                if (stopToken == "stop") {
                    tracks.clear();
                    break;
                }
            }
            stopToken.append(ch);
        }
    }
}

void Parser::fillPattern(PatternType *pattern){
    int length = pattern->length();
    int max = 16;
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
