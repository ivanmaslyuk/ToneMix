#include "track.h"

Track::Track()
{

}

Track::Track(QString artist, QString title, int num, char key, int bpm, QString path)
{
    this->artist = artist;
    this->title = title;
    this->num = num;
    this->key = key;
    this->bpm = bpm;
    this->path = path;
}

QString Track::keyAsString()
{
    return QString::number(this->num) + QString(this->key);
}

QString Track::bpmAsString()
{
    return QString::number(this->bpm);
}

bool operator==(Track t1, Track t2)
{
    return t1.title == t2.title;
//    return t1.path == t2.path;
}
