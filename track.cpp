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

QString Track::description()
{
    if (!this->title.isEmpty()) {
        return this->title;
    } else {
        return this->path;
    }
}

bool operator==(Track t1, Track t2)
{
    return t1.description() == t2.description();
//    return t1.path == t2.path;
}
