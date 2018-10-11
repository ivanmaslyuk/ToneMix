#include "tagreader.h"
#include <fileref.h>
#include <mpegfile.h>
#include <id3v2tag.h>
#include <textidentificationframe.h>
#include <QString>
#include <QTextStream>

TagReader::TagReader(QString dir, QStringList files)
{
    this->files = files;
    this->dir = dir;

//    QList<Track> list = read();
//    int i = 0;
}

QList<Track> &TagReader::read()
{
    QList<Track> *list = new QList<Track>();

    for (int i = 0; i < files.size(); i++) {
        // TODO: удрать это хрень отсюда!!!!!!!!!
        list->append( readFile(dir + "/" + files[i]) );
    }

    return *list;
}

Track &TagReader::readFile(QString path)
{
    // assuming file exists
    TagLib::MPEG::File file(path.toStdWString().c_str());

    TagLib::ID3v2::Tag *tag = file.ID3v2Tag(true);
    TagLib::ID3v2::FrameList TKEY = tag->frameList("TKEY");
    TagLib::ID3v2::FrameList TBPM = tag->frameList("TBPM");

    //QString title = asQString(tag->artist()) + " - " + asQString(tag->title());
    QString key_str;
    int bpm;

    if (!TKEY.isEmpty() && !TBPM.isEmpty()) {
        key_str = asQString( TKEY.front()->toString() );
        bpm = asQString( TBPM.front()->toString() ).toInt();
    }

//    if (!TKEY.isEmpty() && !TBPM.isEmpty()) {
//        return fillTrack(
//                    asQString(tag->artist()),
//                    asQString(tag->title()),
//                    asQString(TKEY.front()->toString()),
//                    asQString(TBPM.front()->toString()).toInt(),
//                    path
//                    );
//    }
//    else {
//        return fillTrack(
//                    asQString(tag->artist()),
//                    asQString(tag->title()),
//                    asQString(TKEY.front()->toString()),
//                    asQString(TBPM.front()->toString()).toInt(),
//                    path
//                    );
//    }

    return fillTrack(
                    asQString(tag->artist()),
                    asQString(tag->title()),
                    key_str,
                    bpm,
                    path
                    );

//    return fillTrack(title, key_str, bpm, path);

}

Track &TagReader::fillTrack(QString artist, QString title, QString key_str, int bpm, QString path)
{
    int num;
    char key;
    QTextStream stream(&key_str);
    stream >> num >> key;
    Track *track = new Track(artist, title, num, key, bpm, path);
    return *track;
}

QString TagReader::asQString(TagLib::String ts)
{
    return QString::fromStdWString(ts.toCWString());
}
