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

#include "dialogaudiosetup.h"
#include "ui_dialogaudiosetup.h"
#include <QMediaDevices>
#include <QAudioDevice>
#include <QSettings>

DialogAudioSetup::DialogAudioSetup(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogAudioSetup)
{
    ui->setupUi(this);
    // list all audio devices with the same order and id which also csound --devices would show
    // and populate the combobox with them
    {
        QList<QAudioDevice> deviceInfos = QMediaDevices::audioOutputs();

        std::sort(deviceInfos.begin(),deviceInfos.end(),
                  [](const QAudioDevice a, const QAudioDevice b) -> bool
        {
            return a.id()<b.id();
        });

        int deviceIndex = 0;
        for (const QAudioDevice &deviceInfo : deviceInfos){
            ui->devices->addItem( QString::number(deviceIndex) +" : " + deviceInfo.description() +" : " + deviceInfo.id() );
            deviceIndex++;
        }
        QSettings settings;
        int currentDeviceID = 0;
        if (settings.contains("audioDevice")){
            currentDeviceID = settings.value("audioDevice").toInt();
            if (currentDeviceID < deviceIndex){
                ui->devices->setCurrentIndex(currentDeviceID);
            }
        }

    }
}

DialogAudioSetup::~DialogAudioSetup()
{
    delete ui;
}

void DialogAudioSetup::done(int r)
{
    if(QDialog::Accepted == r)  // ok was pressed
    {
        QSettings settings;
        settings.setValue("audioDevice",ui->devices->currentIndex());
        emit audioSet();
    }
    QDialog::done(r);
}

