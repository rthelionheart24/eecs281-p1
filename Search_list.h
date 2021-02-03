#ifndef P1_Search_list_H
#define P1_Search_list_H

#include <deque>
#include "Map.h"

class Search_list
{

public:
    int total_tiles;

    std::deque<Tile> to_be_searched;

    std::deque<Tile> searched;

    Search_list();

    ~Search_list();

    virtual void add_tile(Tile t) = 0;

    virtual Tile remove_tile() = 0;

    bool is_empty()
    {
        return to_be_searched.empty();
    }
};

class queue_Search_list : public Search_list
{

public:
    void add_tile(Tile t)
    {
        to_be_searched.push_back(t);
    }

    Tile remove_tile()
    {
        if (!to_be_searched.empty())
        {
            Tile temp;
            temp = to_be_searched.front();
            to_be_searched.pop_front();
            return temp;
        }
        std::cout << "to_be_searched; is empty\n";
    }
};

class stack_Search_list : public Search_list
{

public:
    void add_tile(Tile t)
    {
        to_be_searched.push_back(t);
    }

    Tile remove_tile()
    {
        if (!to_be_searched.empty())
        {
            Tile temp = to_be_searched.back();
            to_be_searched.pop_back();
            return temp;
        }

        std::cout << "to_be_searched; is empty\n";
    }
};

#endif //P1_Search_list_H