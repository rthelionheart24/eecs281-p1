#ifndef P1_Search_list_H
#define P1_Search_list_H

#include <deque>
#include "Map.h"

class Search_list
{

public:
    std::deque<Tile> container;

    Search_list();

    ~Search_list();

    virtual void add_tile(Tile t) const = 0;

    virtual Tile remove_tile() const = 0;
};

class queue_Search_list : public Search_list
{

public:
    void add_tile(Tile t)
    {
        container.push_back(t);
    }

    Tile remove_tile()
    {
        if (!container.empty())
        {
            Tile temp;
            temp = container.front();
            container.pop_front();
            return temp;
        }
        std::cout << "Container is empty\n";
    }
};

class stack_Search_list : public Search_list
{

public:
    void add_tile(Tile t)
    {
        container.push_back(t);
    }

    Tile remove_tile()
    {
        if (!container.empty())
        {
            Tile temp = container.back();
            container.pop_back();
            return temp;
        }

        std::cout << "Container is empty\n";
    }
};

#endif //P1_Search_list_H