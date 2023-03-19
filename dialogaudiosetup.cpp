#include "dialogaudiosetup.h"
#include "ui_dialogaudiosetup.h"

DialogAudioSetup::DialogAudioSetup(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogAudioSetup)
{
    ui->setupUi(this);
}

DialogAudioSetup::~DialogAudioSetup()
{
    delete ui;
}
