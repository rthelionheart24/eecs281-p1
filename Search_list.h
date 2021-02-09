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

    std::deque<Coordinates> to_be_searched;

    virtual void add_tile(Tile *t) { to_be_searched.push_back({t->room, t->row, t->col}); }

    virtual Coordinates remove_tile() = 0;

    bool is_empty() { return to_be_searched.empty(); }

    virtual ~Search_list() {}
};

class queue_Search_list : public Search_list
{

public:
    // void add_tile(Tile *t) override { to_be_searched.push_back(*t); }

    Coordinates remove_tile() override;
};

class stack_Search_list : public Search_list
{

public:
    // void add_tile(Tile *t) override { to_be_searched.push_back({t->room, t->row, t->col}); }

    Coordinates remove_tile() override;

    ~stack_Search_list() {}
};

#endif //P1_Search_list_H