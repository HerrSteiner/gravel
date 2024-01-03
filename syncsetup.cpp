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

#include "syncsetup.h"
#include "ui_syncsetup.h"
#include <QNetworkInterface>
#include <QSettings>

Syncsetup::Syncsetup(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Syncsetup)
{
    ui->setupUi(this);
    {
        QSettings settings;
        QString currentPort = "8001";
        if (settings.contains("syncPort")){
            currentPort = settings.value("syncPort").toString();
        }
        ui->portEdit->setText(currentPort);
    }
}

Syncsetup::~Syncsetup()
{
    delete ui;
}

void Syncsetup::on_startReceiverButton_clicked()
{
    QSettings settings;
    QString port = ui->portEdit->text();
    settings.setValue("syncPort",port);
    emit syncListen(port.toInt());
}


void Syncsetup::on_startSenderButton_clicked()
{
    QSettings settings;
    QString port = ui->portEdit->text();
    settings.setValue("syncPort",port);
    emit syncSend(port.toInt());
}


void Syncsetup::on_stopButton_clicked()
{
    emit syncStop();
}

