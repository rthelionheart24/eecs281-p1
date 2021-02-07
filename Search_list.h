//Project Identifier: B99292359FFD910ED13A7E6C7F9705B8742F0D79
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

    virtual ~Search_list() {}
};

class queue_Search_list : public Search_list
{

public:
    void add_tile(coordinates c) override
    {
        to_be_searched.push_back(c);
    }
    void add_tile(Tile *t) override
    {
        coordinates c = {t->room, t->row, t->col};
        to_be_searched.push_back(c);
    }

    coordinates remove_tile() override
    {

        coordinates temp;
        temp = to_be_searched.front();
        to_be_searched.pop_front();
        return temp;
    }
    ~queue_Search_list() {}
};

class stack_Search_list : public Search_list
{

public:
    void add_tile(coordinates c) override
    {
        to_be_searched.push_back(c);
    }

    void add_tile(Tile *t) override
    {
        coordinates c = {t->room, t->row, t->col};
        to_be_searched.push_back(c);
    }

    coordinates remove_tile() override
    {

        coordinates temp = to_be_searched.back();
        to_be_searched.pop_back();
        return temp;
    }
    ~stack_Search_list() {}
};

#endif //P1_Search_list_H