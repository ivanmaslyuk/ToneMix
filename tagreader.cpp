#include "tagreader.h"
#include <fileref.h>
#include <mpegfile.h>
#include <id3v2tag.h>
#include <textidentificationframe.h>
#include <QString>
#include <QTextStream>
#include "tonenotationtranslator.h"

TagReader::TagReader(QStringList files)
{
    this->files = files;
}

QList<Track*> &TagReader::read()
{
    QList<Track*> *list = new QList<Track*>();

    for (int i = 0; i < files.size(); i++) {
        list->append( readFile(files[i]) );
    }

    return *list;
}

Track *TagReader::readFile(QString path)
{
    // assuming file exists
    TagLib::MPEG::File file(path.toStdWString().c_str());

    TagLib::ID3v2::Tag *tag = file.ID3v2Tag(true);
    TagLib::ID3v2::FrameList TKEY = tag->frameList("TKEY");
    TagLib::ID3v2::FrameList TBPM = tag->frameList("TBPM");

    QString key_str;
    int bpm = 0;

    if (!TKEY.isEmpty() && !TBPM.isEmpty()) {
        key_str = asQString( TKEY.front()->toString() );
        bpm = asQString( TBPM.front()->toString() ).toInt();
    }

    return fillTrack(
                    asQString(tag->artist()),
                    asQString(tag->title()),
                    key_str,
                    bpm,
                    path
                    );

}

Track *TagReader::fillTrack(QString artist, QString title, QString key_str, int bpm, QString path)
{
    ToneNotationTranslator translator;
    Track *track = new Track(title, artist, key_str, bpm, path);

    // Получаем тон в системе трактора, т.к. он нужен генератору.
    QString traktor = translator.toTraktor(key_str);
    if (traktor != "?")
    {
        int num;
        char key;
        QTextStream stream(&traktor);
        stream >> num >> key;
        track->num = num;
        track->key = key;
    }

    track->toneRaw = key_str;
    track->notation = translator.getToneNotation(key_str);
    track->isAnalyzed = key_str != "";
    return track;
}

QString TagReader::asQString(TagLib::String ts)
{
    return QString::fromStdWString(ts.toCWString());
}
