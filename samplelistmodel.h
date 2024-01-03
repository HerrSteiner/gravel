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

#ifndef SAMPLELISTMODEL_H
#define SAMPLELISTMODEL_H

#include <QAbstractListModel>

class SampleListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit SampleListModel(QObject *parent = nullptr);

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Basic functionality:
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QString fileNameAt(uint row);

    // Add data:
    //bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    void addFile(const QString &fileName);

    // Remove data:
    //bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
     void removeFile(int index);

    void saveToFile(const QString &filePath) ;
    void loadFromFile(const QString &filePath);

    private:
    QStringList fileList;
};

#endif // SAMPLELISTMODEL_H
