#include "track.h"

Track::Track()
{

}

Track::Track(QString title, int num, char key, int bpm, QString path)
{
    this->title = title;
    this->num = num;
    this->key = key;
    this->bpm = bpm;
    this->path = path;
}

QString Track::keyAsString()
{
    return QString::number(num) + QString(key);
}
