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
        // list all networks
        QString ips;
        QList<QHostAddress> interfaces = QNetworkInterface::allAddresses();
        for (const QHostAddress &interface : interfaces){
            //if (!interface.CanBroadcast || !interface.IsRunning) continue;
            //if (interface.type() == QNetworkInterface::Ethernet || interface.type() == QNetworkInterface::Wifi || interface.type() == QNetworkInterface::Loopback){
                ips.clear();
             //   QList<QNetworkAddressEntry> allAddresses = interface.addressEntries();
               // for (const QNetworkAddressEntry &address : allAddresses){
               //     if (address.)
               //     QHostAddress ha = address.ip();
                    //if (interface.isLoopback()) continue;
                    //ips.append(" "+interface.toString());
                //}
                ui->networkComboBox->addItem(interface.toString());
           // }
        }
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

