#ifndef CSOUNDPARSER_H
#define CSOUNDPARSER_H

#include <QObject>
#include <QMap>
#include "instrumentdefinition.h"

class CsoundParser : public QObject
{
    Q_OBJECT
public:
    explicit CsoundParser(QObject *parent = nullptr);
public slots:
    void parseCsound(QString fileName);
private:
     void displayInstruments();
     QMap<QString,InstrumentDefinition> instruments;
signals:
    void setInstrumentDefinitions(QMap<QString,InstrumentDefinition>instrumentDefinitions);
    void display(QString message);
};

#endif // CSOUNDPARSER_H
