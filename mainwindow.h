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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "highlighter.h"
#include "soundengine.h"
#include "Parser.h"
#include "csoundparser.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event) override;

public slots:
    void evaluate();

private:
    QThread *soundThread;
    Ui::MainWindow *ui;
    Highlighter *highlighter;
    SoundEngine *soundEngine;
    Parser *parser;
    CsoundParser *csoundParser;
    void loadCode(bool merge);

private slots:
     void on_actionAbout_gravel_triggered();
     void on_actionAudio_setup_triggered();
     void on_actionsave_code_triggered();
     void on_actionopen_code_triggered();
     void on_actionmerge_code_triggered();
     void on_actionSync_setup_triggered();
     void on_actionSample_Mapper_triggered();

signals:
     void stop(); // tells the soundengine to stop before everything shuts down

};

#endif // MAINWINDOW_H
