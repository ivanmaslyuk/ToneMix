#ifndef HARMONICPLAYLISTGENERATOR_H
#define HARMONICPLAYLISTGENERATOR_H

#include "track.h"
#include <QList>

class HarmonicPlaylistGenerator
{
public:
    HarmonicPlaylistGenerator();
    QList<Track> &listCompatible(QList<Track>);
    QList<Track> &harmonicSort(QList<Track>);
    void storeTrack(QList<Track> &, QString, int, char);

private:
    bool matchKey(char, char);
    bool sameNum(int, int);
    bool adjacentNum(int, int);
    bool energyNum(int, int);
    bool diagMix(int, char, int, char);
    bool diagMix2(int, char, int, char);
    bool checkCompatible(int, char, int, char);
    QList<Track> &harmonicSortHelper(QList<Track> &, QList<Track> &);

    //Track shiftArray(QVector<Track> &);

    //QVector<Track> playlist;
};

#endif // HARMONICPLAYLISTGENERATOR_H
