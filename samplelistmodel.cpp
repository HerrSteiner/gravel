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

#include "samplelistmodel.h"
#include <QFile>
#include <QTextStream>
#include <QCoreApplication>

SampleListModel::SampleListModel(QObject *parent)
    : QAbstractListModel(parent)
{
    loadCurrentMap();
}

QVariant SampleListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    // FIXME: Implement me!
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
            if (section == 0) {
                return QString("Sample Number");
            } else if (section == 1) {
                return QString("File Location");
            }
        }
        return QVariant();
}

int SampleListModel::columnCount(const QModelIndex &parent) const {
    return 2;
}

int SampleListModel::rowCount(const QModelIndex &parent) const
{
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if (parent.isValid())
        return 0;

    // FIXME: Implement me!
    return fileList.size();
}

QVariant SampleListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || role != Qt::DisplayRole)
        return QVariant();

    // FIXME: Implement me!
    if (index.column()==0){
        return index.row() + 1;
    }
    return fileList.at(index.row());
}

QString SampleListModel::fileNameAt(uint row){
    if (row > fileList.size()) return "";
    return fileList.at(row);
}

void SampleListModel::addFile(const QString &fileName) {
       beginInsertRows(QModelIndex(), fileList.size(), fileList.size());
       fileList.append(fileName);
       endInsertRows();
}

void SampleListModel::removeFile(int index) {
       if (index >= 0 && index < fileList.size()) {
           beginRemoveRows(QModelIndex(), index, index);
           fileList.removeAt(index);
           endRemoveRows();
       }
}

void SampleListModel::saveToFile(const QString &filePath) {
       QFile file(filePath);
       if (file.open(QIODevice::WriteOnly)) {
           QTextStream stream(&file);
           for (const auto &fileName : fileList)
               stream << fileName << '\n';
       }
   }

void SampleListModel::loadFromFile(const QString &filePath) {
       QFile file(filePath);
       if (file.open(QIODevice::ReadOnly)) {
           QTextStream stream(&file);
           beginResetModel();
           fileList.clear();
           while (!stream.atEnd())
               fileList.append(stream.readLine());
           endResetModel();
       }
   }

void SampleListModel::saveCurrentMap(){
    saveToFile(currentSampleMapFileName());
}

void SampleListModel::loadCurrentMap() {
    loadFromFile(currentSampleMapFileName());
}

QString SampleListModel::currentSampleMapFileName(){
#ifdef Q_OS_MAC
    return QCoreApplication::applicationDirPath() + "/../../../currentSamppleMap.txt";
#else
    return currentMapFileLocation = "currentSamppleMap.txt";
#endif
}
