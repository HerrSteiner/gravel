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

#ifndef DIALOGAUDIOSETUP_H
#define DIALOGAUDIOSETUP_H

#include <QDialog>

namespace Ui {
class DialogAudioSetup;
}

class DialogAudioSetup : public QDialog
{
    Q_OBJECT

public:
    explicit DialogAudioSetup(QWidget *parent = nullptr);
    ~DialogAudioSetup();

protected:
    void done(int r) override;

private:
    Ui::DialogAudioSetup *ui;

signals:
     void audioSet();
};

#endif // DIALOGAUDIOSETUP_H
