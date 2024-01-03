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

#include "coder.h"
#include "QtCore/qcoreevent.h"
#include "QtGui/qevent.h"

Coder::Coder(QWidget *parent)
    : QTextEdit(parent)

{
    this->installEventFilter(this);
}

bool Coder::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type()==QEvent::KeyPress) {
        //qDebug()<<"------";
        QKeyEvent* key = static_cast<QKeyEvent*>(event);
        if ((key->modifiers() & Qt::ShiftModifier) && ( (key->key()==Qt::Key_Enter) || (key->key()==Qt::Key_Return) )) {
            // in case no selection was made, the current line shall be selected and parsed
            bool singleLineEvaluation = false;
            QTextCursor cursor = textCursor();
            QTextCursor backupCursor = textCursor();

            if (!cursor.hasSelection()){

                cursor.select(QTextCursor::LineUnderCursor);
                setTextCursor(cursor);
                singleLineEvaluation = true;
            }
            emit evaluation();
            //ui->console->clear();
            //ui->console->append(ui->textEdit->textCursor().selectedText());
            if (singleLineEvaluation) {
                setTextCursor(backupCursor);
            }
            return true;
        }
    }
    return false;
}

