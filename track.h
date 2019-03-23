#ifndef TRACK_H
#define TRACK_H

#include <QString>
#include "tonenotationtranslator.h"

class Track
{
public:
    Track(QString, QString, int, char, int, QString);
    Track(QString title, QString artist, QString toneRaw, int BPM, QString path);
    Track();
    QString keyAsString(); // тональность
    QString bpmAsString(); // ударов в минуту
    QString description();

    QString title; // наазвание
    QString artist; // исполнитель
    QString path; // путь к файлу
    int num;
    char key;
    int bpm;
    bool repeatedInPlaylist = false;
    bool excluded = false;

    friend bool operator==(Track t1, Track t2);

    // имеет отношение к авто-определению тональности по регуляркам
    ToneNotation notation;
    QString toneRaw;
    bool isAnalyzed;
};

#endif // TRACK_H
