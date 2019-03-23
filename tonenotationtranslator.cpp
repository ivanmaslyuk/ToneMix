#include "tonenotationtranslator.h"

#include <QRegExp>
#include <QDebug>

ToneNotationTranslator::ToneNotationTranslator()
{

}

ToneNotation ToneNotationTranslator::getToneNotation(QString raw)
{
    QRegExp camelot("^(1|2|3|4|5|6|7|8|9|10|11|12)(A|B)$");
    QRegExp traktor("^(1|2|3|4|5|6|7|8|9|10|11|12)(m|d)$");
    QRegExp standard("^[A-G](b|bm|m)?$");

    if (camelot.exactMatch(raw))
    {
        return ToneNotation::Comelot;
    }
    else if (standard.exactMatch(raw))
    {
        return ToneNotation::Standard;
    }
    else if (traktor.exactMatch(raw))
    {
        return ToneNotation::Traktor;
    }
    else
    {
        return ToneNotation::UnknownToneNotation;
    }
}

QString ToneNotationTranslator::toCamelot(QString raw)
{
    ToneNotation notation = getToneNotation(raw);
    switch (notation) {
        case ToneNotation::Comelot: return raw;
        case ToneNotation::Standard: return standardToCamelot(raw);
        case ToneNotation::Traktor: return standardToCamelot(traktorToStandard(raw));
        case ToneNotation::UnknownToneNotation: return "?";
    }
}

QString ToneNotationTranslator::toStandard(QString raw)
{
    ToneNotation notation = getToneNotation(raw);
    switch (notation) {
        case ToneNotation::Comelot: return camelotToStandard(raw);
        case ToneNotation::Standard: return raw;
        case ToneNotation::Traktor: return traktorToStandard(raw);
        case ToneNotation::UnknownToneNotation: return "?";
    }
}

QString ToneNotationTranslator::toTraktor(QString raw)
{
    ToneNotation notation = getToneNotation(raw);
    switch (notation) {
        case ToneNotation::Comelot: return standardToTraktor(camelotToStandard(raw));
        case ToneNotation::Standard: return standardToTraktor(raw);
        case ToneNotation::Traktor: return raw;
        case ToneNotation::UnknownToneNotation: return "?";
    }
}

QString ToneNotationTranslator::camelotToStandard(QString raw)
{
    if (raw == "1B") return "B";
    if (raw == "2B") return "Gb";
    if (raw == "3B") return "Db";
    if (raw == "4B") return "Ab";
    if (raw == "5B") return "Eb";
    if (raw == "6B") return "Bb";
    if (raw == "7B") return "F";
    if (raw == "8B") return "C";
    if (raw == "9B") return "G";
    if (raw == "10B") return "D";
    if (raw == "11B") return "A";
    if (raw == "12B") return "E";
    if (raw == "1A") return "Abm";
    if (raw == "2A") return "Ebm";
    if (raw == "3A") return "Bbm";
    if (raw == "4A") return "Fm";
    if (raw == "5A") return "Cm";
    if (raw == "6A") return "Gm";
    if (raw == "7A") return "Dm";
    if (raw == "8A") return "Am";
    if (raw == "9A") return "Em";
    if (raw == "10A") return "Bm";
    if (raw == "11A") return "Gbm";
    if (raw == "12A") return "Dbm";
    qDebug() << "Не удалось перевести" << raw << "в стандартную систему нотации.";
    return "?";
}

QString ToneNotationTranslator::standardToCamelot(QString raw)
{
    if (raw == "B") return "1B";
    if (raw == "Gb") return "2B";
    if (raw == "Db") return "3B";
    if (raw == "Ab") return "4B";
    if (raw == "Eb") return "5B";
    if (raw == "Bb") return "6B";
    if (raw == "F") return "7B";
    if (raw == "C") return "8B";
    if (raw == "G") return "9B";
    if (raw == "D") return "10B";
    if (raw == "A") return "11B";
    if (raw == "E") return "12B";
    if (raw == "Abm") return "1A";
    if (raw == "Ebm") return "2A";
    if (raw == "Bbm") return "3A";
    if (raw == "Fm") return "4A";
    if (raw == "Cm") return "5A";
    if (raw == "Gm") return "6A";
    if (raw == "Dm") return "7A";
    if (raw == "Am") return "8A";
    if (raw == "Em") return "9A";
    if (raw == "Bm") return "10A";
    if (raw == "Gbm") return "11A";
    if (raw == "Dbm") return "12A";
    qDebug() << "Не удалось перевести" << raw << "в систему нотации Camelot.";
    return "?";
}

QString ToneNotationTranslator::traktorToStandard(QString raw)
{
    if (raw == "1d") return "C";
    if (raw == "2d") return "G";
    if (raw == "3d") return "D";
    if (raw == "4d") return "A";
    if (raw == "5d") return "E";
    if (raw == "6d") return "B";
    if (raw == "7d") return "Gb";
    if (raw == "8d") return "Db";
    if (raw == "9d") return "Ab";
    if (raw == "10d") return "Eb";
    if (raw == "11d") return "Bb";
    if (raw == "12d") return "F";

    if (raw == "1m") return "Am";
    if (raw == "2m") return "Em";
    if (raw == "3m") return "Bm";
    if (raw == "4m") return "Gbm";
    if (raw == "5m") return "Dbm";
    if (raw == "6m") return "Abm";
    if (raw == "7m") return "Ebm";
    if (raw == "8m") return "Bbm";
    if (raw == "9m") return "Fm";
    if (raw == "10m") return "Cm";
    if (raw == "11m") return "Gm";
    if (raw == "12m") return "Dm";
    qDebug() << "Не удалось перевести" << raw << "в систему нотации Traktor.";
    return "?";
}

QString ToneNotationTranslator::standardToTraktor(QString raw)
{
    if (raw == "C") return "1d";
    if (raw == "G") return "2d";
    if (raw == "D") return "3d";
    if (raw == "A") return "4d";
    if (raw == "E") return "5d";
    if (raw == "B") return "6d";
    if (raw == "Gb") return "7d";
    if (raw == "Db") return "8d";
    if (raw == "Ab") return "9d";
    if (raw == "Eb") return "10d";
    if (raw == "Bb") return "11d";
    if (raw == "F") return "12d";

    if (raw == "Am") return "1m";
    if (raw == "Em") return "2m";
    if (raw == "Bm") return "3m";
    if (raw == "Gbm") return "4m";
    if (raw == "Dbm") return "5m";
    if (raw == "Abm") return "6m";
    if (raw == "Ebm") return "7m";
    if (raw == "Bbm") return "8m";
    if (raw == "Fm") return "9m";
    if (raw == "Cm") return "10m";
    if (raw == "Gm") return "11m";
    if (raw == "Dm") return "12m";
    qDebug() << "Не удалось перевести" << raw << "в стандартную систему нотации.";
    return "?";
}
