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
#include "track.h"

Track::Track()
{
    playSequencesA = false;
    swapSequences = false;
}

void Track::setSequences(SequencesType sequences){
    if (playSequencesA) sequencesB = sequences;
    else sequencesA = sequences;
    swapSequences = true;
}

QList<PatternEvent>Track::getNextEvents() {
    SequencesType sequences = playSequencesA ? sequencesA : sequencesB;
    SequencesType tickedSequences;
    QList<PatternEvent> currentEvents;
    qsizetype amountSequences = sequences.length();
    bool canSwapSequences = (amountSequences == 0);
    for (qsizetype i = 0; i<amountSequences;i++){
        Sequence seq = sequences.at(i);
        PatternEvent e = seq.nextStep();
        tickedSequences.append(seq);
        if (seq.index == 0) canSwapSequences = true;
        if (e.instrumentNumber == 0) continue;
        currentEvents.append(e);

    }
    if (playSequencesA){
        sequencesA.swap(tickedSequences);
    }
    else {
        sequencesB.swap(tickedSequences);
    }

    if (canSwapSequences && swapSequences) {
        if (sequencesA.empty() && sequencesB.empty()) return currentEvents;

        playSequencesA = !playSequencesA;
        swapSequences = false;
    }

    return currentEvents;
}
