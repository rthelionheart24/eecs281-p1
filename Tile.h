//Project Identifier: B99292359FFD910ED13A7E6C7F9705B8742F0D79
#ifndef P1_TILE_H
#define P1_TILE_H

struct Coordinates
{
    unsigned int room;
    unsigned int row;
    unsigned int col;
};

struct Tile
{
    char type = '.';
    char previous = 't';
    bool discover = false;
};

#endif
