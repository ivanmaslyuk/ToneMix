#include "harmonicplaylistgenerator.h"
#include <QList>
#include <QRandomGenerator>
#include <QTime>

//https://github.com/NRec22/harmonic-mixing-generator/blob/master/generator.js
HarmonicPlaylistGenerator::HarmonicPlaylistGenerator() {}

// check matching
bool HarmonicPlaylistGenerator::matchKey(char key1, char key2)
{
    return key1 == key2;
}

// 4A <-> 4A
bool HarmonicPlaylistGenerator::sameNum(int num1, int num2)
{
    return num1 == num2;
}

//4A <-> 5A or 4B <-> 5B
bool HarmonicPlaylistGenerator::adjacentNum(int num1, int num2)
{
    if (num1 == num2 - 1 || num1 == num2 + 11)
        return true;
    else if(num1 == num2 + 1 || num1 == num2 - 11)
        return true;
    else
        return false;
}

//given two songs, returns whether they can be harmonically mixed
bool HarmonicPlaylistGenerator::checkCompatible(int num1, char key1, int num2, char key2)
{
    if(sameNum(num1, num2) == true){
        return true;
    }
    else if(adjacentNum(num1, num2) == true && matchKey(key1, key2) == true){
        return true;
    }
    else{
        return false;
    }
}

/**
 * @brief Вынимает из плейлиста случайный, совместимый с данной песней, трек.
 * @param playlist
 * @param track
 * @return
 */
Track HarmonicPlaylistGenerator::takeRandomCompatible(QList<Track> &playlist, Track track)
{
    QList<Track> compatible;

    for (int i = 0; i < playlist.size(); i++)
        if (checkCompatible(track.num, track.key, playlist[i].num, playlist[i].key))
            compatible.append(playlist.takeAt(i));

    // Если нет совместимых треков, возвращаем пустой Track
    if (compatible.size() == 0) return Track();

    QRandomGenerator generator(QTime::currentTime().msec());
    int index = generator.bounded(0, compatible.size() - 1);

    return compatible.takeAt(index);
}

/*
Given a playlist, this method sorts the playlist prioritizing harmonic transitions first.
Works in a first come, first serve basis.
*/
QList<Track> &HarmonicPlaylistGenerator::harmonicSort(QList<Track> playlist, bool random)
{
    QList<Track> *unsort = new QList<Track>(playlist);

    QList<Track> *sortedPlaylist = new QList<Track>();
    sortedPlaylist->append(unsort->takeFirst());

    if (random)
        return harmonicSortRandomHelper(*sortedPlaylist, *unsort);
    else
        return harmonicSortHelper(*sortedPlaylist, *unsort);
}

QList<Track> &HarmonicPlaylistGenerator::harmonicSortHelper(QList<Track> &sortlist, QList<Track> &unsortlist)
{
    // можно написать функцию которая будет находить наиболее подходящую песню для данной песни, чтобы было меньше отсеенных треков
    // одним из критериев оценивания будет то, сколько песен после себя она может поставить
    // главный критерий -- если есть песня с такой же тональностью, ставим ее.
    for(int i = 0; i < unsortlist.size(); i++) {
        if (checkCompatible(sortlist[sortlist.size() - 1].num, sortlist[sortlist.size() - 1].key, unsortlist[i].num, unsortlist[i].key)) {
            sortlist.append(unsortlist.takeAt(i));
            harmonicSortHelper(sortlist, unsortlist);
        }
        else if (i == unsortlist.size() - 1) {
            //sortlist.append(unsortlist.takeFirst()); // добавляет не поддающиеся сортировке песни в конец списка
            unsortlist.removeFirst(); // удаляет не поддающиеся сортировке песни из плейлиста
            harmonicSortHelper(sortlist, unsortlist);
        }
    }
    return sortlist;
}

QList<Track> &HarmonicPlaylistGenerator::harmonicSortRandomHelper(QList<Track> &sortlist, QList<Track> &unsortlist)
{
    if (unsortlist.size() != 0) {
        // Берем последний трек в сортированном списке, чтобы подобрать к нему совместимый.
        Track last = sortlist.last();

        // Добавить случайный совместимый
        Track randomCompatible = takeRandomCompatible(unsortlist, last);
        // Если в полученном Track есть путь к файлу, то найти совместимый трек удалось.
        if (randomCompatible.path != "") {
            sortlist.append(randomCompatible);
            harmonicSortRandomHelper(sortlist, unsortlist);
        }
        // Если в полученном Track нет пути к файлу, значит не удалось найти совместимый трек.
        else {
            return sortlist;
        }
    }


    return sortlist;
}
