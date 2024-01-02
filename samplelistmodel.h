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
