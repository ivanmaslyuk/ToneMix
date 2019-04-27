#ifndef FILEREADER_H
#define FILEREADER_H

#include <QStringList>
#include "track.h"
#include <fileref.h>

class TagReader
{
public:
    // Инициализирует класс списком файлов.
    TagReader(QStringList);
    // Считывает метаданные треков и возвращает список с ними.
    QList<Track*> &read();
    // Считывает метаданные трека и возвращает их.
    Track *readFile(QString path);
private:
    // Инициализирует объект Track и возвращает его.
    Track *fillTrack(QString artist, QString title, QString key_str, int bpm, QString path);
    // Преобразует TagLib::String в QString
    QString asQString(TagLib::String);

    // Хранит список файлов, переданный в конструктор.
    QStringList files;

};

#endif // FILEREADER_H
