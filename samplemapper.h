#ifndef SAMPLEMAPPER_H
#define SAMPLEMAPPER_H

#include <QDialog>
#include "samplelistmodel.h"

namespace Ui {
class SampleMapper;
}

class SampleMapper : public QDialog
{
    Q_OBJECT

public:
    explicit SampleMapper(QWidget *parent = nullptr);
    ~SampleMapper();

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
    SampleListModel *model;
};

#endif // SAMPLEMAPPER_H
