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

#include "samplemapper.h"
#include "ui_samplemapper.h"
#include <QFileDialog>
#include <QMessageBox>

const int maxEntries = 92;

SampleMapper::SampleMapper(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SampleMapper)
{
    ui->setupUi(this);
    model = new SampleListModel(this);
    ui->tableView->setModel(model);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
}

SampleMapper::~SampleMapper()
{
    delete ui;
}

void SampleMapper::on_Load_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this);
    if (!filePath.isEmpty()){
        model->loadFromFile(filePath);
        checkAddButton();
    }
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
    if (!fileName.isEmpty()){
        model->addFile(fileName);
        checkAddButton();
    }
}


void SampleMapper::on_Remove_clicked()
{
    QModelIndexList selected = ui->tableView->selectionModel()->selectedIndexes();
    if (!selected.isEmpty()){
        model->removeFile(selected.first().row());
        checkAddButton();
    }
}


void SampleMapper::on_activate_clicked()
{
    model->saveCurrentMap();
    emit activateMap(model);

}

void SampleMapper::checkAddButton()
{
    ui->Add->setEnabled(model->rowCount() < maxEntries);
}

void SampleMapper::newMap()
{

}

void SampleMapper::on_NewButton_clicked()
{
    // Show a warning dialog
           QMessageBox::StandardButton reply = QMessageBox::warning(
               this, "Warning", "This will remove all entries from the samplemap,do you want to perform that action?",
               QMessageBox::Yes | QMessageBox::No);

           // Check the user's response
           if (reply == QMessageBox::Yes) {
               newMap();
           }
}

