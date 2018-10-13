#ifndef FILEREADER_H
#define FILEREADER_H

#include <QStringList>
#include "track.h"
#include <fileref.h>

class TagReader
{
public:
    TagReader(QStringList);
    QList<Track*> &read();
    Track *readFile(QString path);
private:

    Track *fillTrack(QString artist, QString title, QString key_str, int bpm, QString path);

    QStringList files;
    QString asQString(TagLib::String);
};

#endif // FILEREADER_H
