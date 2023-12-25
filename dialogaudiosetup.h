#ifndef DIALOGAUDIOSETUP_H
#define DIALOGAUDIOSETUP_H

#include <QDialog>

namespace Ui {
class DialogAudioSetup;
}

class DialogAudioSetup : public QDialog
{
    Q_OBJECT

public:
    explicit DialogAudioSetup(QWidget *parent = nullptr);
    ~DialogAudioSetup();

protected:
    void done(int r) override;

private:
    Ui::DialogAudioSetup *ui;

signals:
     void audioSet();
};

#endif // DIALOGAUDIOSETUP_H
