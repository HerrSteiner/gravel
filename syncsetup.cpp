#include "syncsetup.h"
#include "ui_syncsetup.h"
#include <QNetworkInterface>
#include <QSettings.h>

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

