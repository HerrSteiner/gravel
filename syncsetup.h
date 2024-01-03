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

#ifndef SYNCSETUP_H
#define SYNCSETUP_H

#include <QDialog>

namespace Ui {
class Syncsetup;
}

/// syncsetup let the customer activate either the sending or recieving of a clock signal
/// which is the actual sequence step number broadcasted on all networks via UDP with a port of custom choice.
/// There should be only one sender, otherwise weidness is breaking lose.
class Syncsetup : public QDialog
{
    Q_OBJECT

public:
    explicit Syncsetup(QWidget *parent = nullptr);
    ~Syncsetup();

private:
    Ui::Syncsetup *ui;

signals:
    void syncListen(int port);
    void syncSend(int port);
    void syncStop();
private slots:
    void on_startReceiverButton_clicked();
    void on_startSenderButton_clicked();
    void on_stopButton_clicked();
};

#endif // SYNCSETUP_H
