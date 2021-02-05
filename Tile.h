#ifndef P1_TILE_H
#define P1_TILE_H

struct Tile
{

    unsigned int room;
    unsigned int row;
    unsigned int col;

    char type = '.';
    char previous = 't';
};

struct coordinates
{
    unsigned int room, row, col;
};

#endif
