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
    }
    QDialog::done(r);
}

