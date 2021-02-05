#ifndef P1_Search_list_H
#define P1_Search_list_H

#include <deque>
#include "Map.h"
#include "Tile.h"

class Search_list
{

public:
    int total_tiles;

    std::deque<coordinates> to_be_searched;

    virtual void add_tile(coordinates c) = 0;

    virtual void add_tile(Tile *t) = 0;

    virtual coordinates remove_tile() = 0;

    bool is_empty()
    {
        return to_be_searched.empty();
    }
};

class queue_Search_list : public Search_list
{

public:
    void add_tile(coordinates c)
    {
        to_be_searched.push_back(c);
    }
    void add_tile(Tile *t)
    {
        coordinates c = {t->room, t->row, t->col};
        to_be_searched.push_back(c);
    }

    coordinates remove_tile()
    {

        coordinates temp;
        temp = to_be_searched.front();
        to_be_searched.pop_front();
        return temp;
    }
};

class stack_Search_list : public Search_list
{

public:
    void add_tile(coordinates c)
    {
        to_be_searched.push_back(c);
    }

    void add_tile(Tile *t)
    {
        coordinates c = {t->room, t->row, t->col};
        to_be_searched.push_back(c);
    }

    coordinates remove_tile()
    {

        coordinates temp = to_be_searched.back();
        to_be_searched.pop_back();
        return temp;
    }
};

#endif //P1_Search_list_H