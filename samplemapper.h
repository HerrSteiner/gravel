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

#ifndef SAMPLEMAPPER_H
#define SAMPLEMAPPER_H

#include <QDialog>
#include "samplelistmodel.h"

namespace Ui {
class SampleMapper;
}

/// samples has to be loaded in tables in Csound and referenced with an index number
/// The mapper let the user compile a map of all samplefiles which are assigned from number 1 to maximal 92
/// When the map is activated it is stored as currentMap.txt and Csound is restartet to clear former loaded samples and load the current ones.
/// Different maps can be exported and imported to disk

class SampleMapper : public QDialog
{
    Q_OBJECT

public:
    explicit SampleMapper(QWidget *parent = nullptr);
    ~SampleMapper();
    SampleListModel *model;

private slots:
    void on_Load_clicked();
    void on_Save_clicked();
    void on_Add_clicked();
    void on_Remove_clicked();
    void on_activate_clicked();

signals:
    void activateMap(SampleListModel *model);

private:
    Ui::SampleMapper *ui;
    void checkAddButton();
};

#endif // SAMPLEMAPPER_H
