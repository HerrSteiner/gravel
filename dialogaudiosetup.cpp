#include "dialogaudiosetup.h"
#include "ui_dialogaudiosetup.h"
#include <QMediaDevices>
#include <QAudioDevice>

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
    }
}

DialogAudioSetup::~DialogAudioSetup()
{
    delete ui;
}
