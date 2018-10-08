#ifndef FILEREADER_H
#define FILEREADER_H

#include <QStringList>
#include "track.h"
#include <fileref.h>

class TagReader
{
public:
    TagReader(QString, QStringList);
    QList<Track> &read();
private:
    Track &readFile(QString path);
    Track &fillTrack(QString title, QString key_str, int bpm, QString path);

    QStringList files;
    QString dir;
    QString asQString(TagLib::String);
};

#endif // FILEREADER_H
