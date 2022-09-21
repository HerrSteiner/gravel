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

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    highlighter = new class highlighter(ui->textEdit->document());
    soundEngine = new SoundEngine();
    parser = new Parser();
    QObject::connect(ui->textEdit, &Coder::evaluation, this, &MainWindow::evaluate);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::evaluate() {
    QString code = ui->textEdit->textCursor().selectedText();
    ui->console->clear();
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

/*
void MainWindow::actionAbout_gravel()
{
    QMessageBox::about(this, tr("About Syntax Highlighter"),
                tr("<p>The <b>Syntax Highlighter</b> example shows how " \
                   "to perform simple syntax highlighting by subclassing " \
                   "the QSyntaxHighlighter class and describing " \
                   "highlighting rules using regular expressions.</p>"));
}
*/
