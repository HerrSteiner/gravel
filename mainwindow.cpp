/*
   gravel - live coding music system -
   Copyright (C) 2022 Malte Steiner

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

#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "QtCore/QDebug"
#include <QMessageBox>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    qDebug()<< "main "<< QApplication::instance()->thread();
    ui->setupUi(this);
    highlighter = new class Highlighter(ui->textEdit->document());

    // setting up the sound core to run in its own thread
    // so the sequencer clock doesn't get interrupted by the gui
    // csound itself runs in its own, separate thread
    QThread *soundThread = new QThread();
    this->soundThread = soundThread;
    soundEngine = new SoundEngine();
    soundEngine->moveToThread(soundThread);
    QObject::connect(soundThread, SIGNAL(started()), soundEngine, SLOT(process()), Qt::QueuedConnection);
    soundThread->start(QThread::TimeCriticalPriority);

    csoundParser = new CsoundParser();
    QObject::connect(csoundParser,SIGNAL(display(QString)),ui->console,SLOT(append(QString)));

    parser = new Parser();
    QObject::connect(ui->textEdit, &Coder::evaluation, this, &MainWindow::evaluate);

    QObject::connect(parser,SIGNAL(display(QString)),ui->console,SLOT(append(QString)));
    QObject::connect(soundEngine,SIGNAL(display(QString)),ui->console,SLOT(append(QString)));
    QObject::connect(soundEngine,SIGNAL(parseCsound(QString)),csoundParser,SLOT(parseCsound(QString)));
    QObject::connect(csoundParser,SIGNAL(setInstrumentDefinitions(QMap<QString,InstrumentDefinition>)),parser,SLOT(setInstrumentDefinitions(QMap<QString,InstrumentDefinition>)));
    QObject::connect(csoundParser,SIGNAL(setInstrumentDefinitions(QMap<QString,InstrumentDefinition>)),soundEngine,SLOT(setInstrumentDefinitions(QMap<QString,InstrumentDefinition>)));

    QObject::connect(parser,SIGNAL(status(QString)),statusBar(),SLOT(showMessage(QString)));
    QObject::connect(parser,SIGNAL(setBPM(int)),soundEngine,SLOT(setBPM(int)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::evaluate() {
    QString code = ui->textEdit->textCursor().selectedText();
    //ui->console->clear();
    ui->console->append("parsing:");
    ui->console->append(code);
    /*
    QList<int> seq = parser->parseCode(code);
    soundEngine->setPattern(seq);
    QStringList listString;
        QString outString;

        for(int i =0; i < seq.length(); i++)
        {
            listString << QString::number(seq.at(i));
        }
        outString = listString.join("\n");
         ui->console->append(outString);
*/
    parser->tracks.swap(soundEngine->tracks);
    parser->parseCode(code);
    soundEngine->tracks.swap( parser->tracks);
}

void MainWindow::on_actionAbout_gravel_triggered()
{
    QMessageBox::about(this, tr("gravel"),
                       tr("<h2>g<span style='color:#666;'>rave</span>l</h2><h3>live coding music system</h3>\
                          <p>© 2022 by Malte Steiner<br>Tina Mariane Krogh Madsen</p>\
                          <p>see also <a href='https://www.block4.com'>https://www.block4.com</a></p>\
            <p>gravel is free open source software distributed under GPL3 license</p>\
            <p>it uses QT 6.4.1 which can be obtained at <a href='https://www.qt.io'>https://www.qt.io</a></p>\
            <p>the audio synthesis is implemented with <a href='https://csound.com'>Csound</a></p>\
            <p>version 0.1 alpha use on your own risk</p>"));
}


void MainWindow::on_actionsave_code_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this,tr("Save Code"),QDir::homePath(), tr("Text files (*.txt)"));
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream out(&file);
    out << ui->textEdit->toPlainText();
    file.close();
}

void MainWindow::on_actionopen_code_triggered()
{
    loadCode(false);
}

void MainWindow::on_actionmerge_code_triggered()
{
    loadCode(true);
}

void MainWindow::loadCode(bool merge){
    QString fileName = QFileDialog::getOpenFileName(this,tr("Load Code"),QDir::homePath(), tr("Text files (*.txt)"));
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;
    QTextStream in(&file);
    QString code = in.readAll();
    if (!merge) ui->textEdit->clear();
    ui->textEdit->append(code);
    file.close();
}




