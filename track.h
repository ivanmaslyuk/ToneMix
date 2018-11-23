#ifndef TRACK_H
#define TRACK_H

#include <QString>

class Track
{
public:
    Track(QString, QString, int, char, int, QString);
    Track();
    QString keyAsString(); // тональность
    QString bpmAsString(); // ударов в минуту

    QString title; // наазвание
    QString artist; // исполнитель
    QString path; // путь к файлу
    int num;
    char key;
    int bpm;
    bool repeatedInPlaylist = false;
    bool excluded = false;

    friend bool operator==(Track t1, Track t2);
};

#endif // TRACK_H
