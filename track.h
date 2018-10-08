#ifndef TRACK_H
#define TRACK_H

#include <QString>

class Track
{
public:
    Track(QString, int, char, int, QString);
    Track();
    QString keyAsString();

    QString title;
    QString path;
    int num;
    char key;
    int bpm;
};

#endif // TRACK_H
