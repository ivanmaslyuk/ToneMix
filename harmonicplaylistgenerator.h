#ifndef HARMONICPLAYLISTGENERATOR_H
#define HARMONICPLAYLISTGENERATOR_H

#include "track.h"
#include <QList>

class HarmonicPlaylistGenerator
{
public:
    static QList<Track> &harmonicSort(QList<Track>, bool random = false);

private:
    // не разрешается создавать объекты этого класса
    HarmonicPlaylistGenerator();

    static bool matchKey(char, char);
    static bool sameNum(int, int);
    static bool adjacentNum(int, int);
    static bool checkCompatible(int, char, int, char);
    static QList<Track> &harmonicSortHelper(QList<Track> &, QList<Track> &);
    static QList<Track> &harmonicSortRandomHelper(QList<Track> &, QList<Track> &);
    static Track takeRandomCompatible(QList<Track> &playlist, Track track);
};

#endif // HARMONICPLAYLISTGENERATOR_H
