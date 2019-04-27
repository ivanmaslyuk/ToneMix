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

// Переводит тональность в различные системы нотации.
class ToneNotationTranslator
{
public:
    ToneNotationTranslator();
    // Получить систему нотации, в которой записано переданное значение.
    ToneNotation getToneNotation(QString raw);
    // Переводит из любой системы нотации в стандарт Camelot Wheel.
    QString toCamelot(QString raw);
    // Переводит из любой системы нотации в стандартную.
    QString toStandard(QString raw);
    // Переводит из любой системы нотации в стандарт программы Traktor.
    QString toTraktor(QString raw);

private:
    // Переводит из системы Camelot Wheel в стандартную.
    QString camelotToStandard(QString raw);
    // Переводит из стандартной системы в Camelot Wheel.
    QString standardToCamelot(QString raw);
    // Переводит из системы программы Traktor в стандартную систему.
    QString traktorToStandard(QString raw);
    // Переводит из стандартной системы в систему программы Traktor.
    QString standardToTraktor(QString raw);
};

#endif // TONENOTATIONTRANSLATOR_H
