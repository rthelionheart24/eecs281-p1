//Project Identifier: B99292359FFD910ED13A7E6C7F9705B8742F0D79
#include "Search_list.h"

Coordinates queue_Search_list::remove_tile()
{

    Coordinates temp = to_be_searched.front();
    to_be_searched.pop_front();
    total_tiles++;
    return temp;
}

Coordinates stack_Search_list::remove_tile()
{
    Coordinates temp = to_be_searched.back();
    to_be_searched.pop_back();
    total_tiles++;
    return temp;
}