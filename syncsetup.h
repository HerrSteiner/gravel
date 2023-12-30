#ifndef SYNCSETUP_H
#define SYNCSETUP_H

#include <QDialog>

namespace Ui {
class Syncsetup;
}

class Syncsetup : public QDialog
{
    Q_OBJECT

public:
    explicit Syncsetup(QWidget *parent = nullptr);
    ~Syncsetup();

private:
    Ui::Syncsetup *ui;

signals:
    void syncListen(int port);
    void syncSend(int port);
    void syncStop();
private slots:
    void on_startReceiverButton_clicked();
    void on_startSenderButton_clicked();
    void on_stopButton_clicked();
};

#endif // SYNCSETUP_H
