#include "samplemapper.h"
#include "ui_samplemapper.h"
#include <QFileDialog>

SampleMapper::SampleMapper(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SampleMapper)
{
    ui->setupUi(this);
    model = new SampleListModel(this);
    ui->tableView->setModel(model);
}

SampleMapper::~SampleMapper()
{
    delete ui;
}




void SampleMapper::on_Load_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this);
            if (!filePath.isEmpty())
                model->loadFromFile(filePath);
}


void SampleMapper::on_Save_clicked()
{
    QString filePath = QFileDialog::getSaveFileName(this);
           if (!filePath.isEmpty())
               model->saveToFile(filePath);
}


void SampleMapper::on_Add_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this);
           if (!fileName.isEmpty())
               model->addFile(fileName);
}


void SampleMapper::on_Remove_clicked()
{
    QModelIndexList selected = ui->tableView->selectionModel()->selectedIndexes();
           if (!selected.isEmpty())
               model->removeFile(selected.first().row());
}


void SampleMapper::on_activate_clicked()
{
    emit activateMap(model);
}

