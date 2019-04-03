#ifndef TONENOTATIONTRANSLATOR_H
#define TONENOTATIONTRANSLATOR_H

#include <QString>

enum ToneNotation
{
    Standard,
    Comelot,
    Traktor,
    UnknownToneNotation
};

class ToneNotationTranslator
{
public:
    ToneNotationTranslator();
    ToneNotation getToneNotation(QString raw);
    QString toCamelot(QString raw);
    QString toStandard(QString raw);
    QString toTraktor(QString raw);

private:
    QString camelotToStandard(QString raw);
    QString standardToCamelot(QString raw);
    QString traktorToStandard(QString raw);
    QString standardToTraktor(QString raw);
};

#endif // TONENOTATIONTRANSLATOR_H