//Project Identifier: B99292359FFD910ED13A7E6C7F9705B8742F0D79
#include <iostream>
#include <cstdlib>
#include <vector>
#include <deque>
#include <stack>
#include <queue>
#include <string>
#include <algorithm>
#include "Map.h"
#include "Tile.h"
#include "Search_list.h"
#include <getopt.h>

using namespace std;

bool stack_action(Map map, Search_list *list, Tile t);
void label_path(Map map, Search_list *list);

void output_map(Map map);
void output_list(Map map);

int main(int argc, char **argv)
{
	ios_base::sync_with_stdio(false);

	unsigned int num_rooms, size_room;
	char input_mode;
	Tile current_tile, starting_tile, ending_tile;

	cin >> input_mode >> num_rooms >> size_room;

	Map map(num_rooms, size_room, input_mode);

	map.get_options(argc, argv);

	//Confirm input mode
	if (input_mode == 'M')
		map.read_map();
	else if (input_mode == 'L')
		map.read_list();

	//Confirm search mode
	Search_list *list;
	if (map.get_search_mode() == 's')
		list = new stack_Search_list();
	else
		list = new queue_Search_list();

	//Add the starting position to search list
	list->add_tile(map.get_starting());
	starting_tile = map.get_starting();
	current_tile = starting_tile;
	ending_tile = map.get_ending();

	bool result = false;

	while (!list->is_empty())
	{
		//1.Remove the next position
		Tile current = list->remove_tile();

		result = stack_action(map, list, current);

		if (result)
		{
			//TODO: backtrack the path to countess and leave the loop
			if (map.get_output_mode() == 'm')
				output_map(map);
			else
				output_list(map);
		}
	}

	cout << "No solution, " << list->total_tiles << " tiles discovered";

	delete list;

	return 0;
}

bool stack_action(Map map, Search_list *list, Tile current)
{

	//2. If the position is a warp pipe
	if (isdigit(current.type))
	{

		Tile next = map.get_tile(static_cast<unsigned int>(current.type - '0'),
								 current.row, current.col);
		next.previous = 'p';
		if (next.type == 'd' || next.type == '#' || next.type == '!')
			return false;
		if (next.type == 'C')
			return true;

		list->add_tile(next);
		next.previous = static_cast<char>('0' + current.room);
		return false;
	}

	//3. If the position isn't a warp pipe
	else
	{
		if (map.movable('n', current.room, current.row, current.col))
		{
			Tile next = map.get_tile(current.room, current.row + 1, current.col);

			//When the next tile is C
			if (next.type == 'C')
			{
				map.get_ending().previous = 's';
				return true;
			}

			next.previous = 's';
			list->add_tile(next);

			map.discover(next);
		}

		if (map.movable('e', current.room, current.row, current.col))
		{
			Tile next = map.get_tile(current.room, current.row, current.col + 1);
			//When the next tile is C
			if (next.type == 'C')
			{
				map.get_ending().previous = 'w';
				return true;
			}

			next.previous = 'w';
			list->add_tile(next);
			map.discover(next);
		}

		if (map.movable('s', current.room, current.row, current.col))
		{
			Tile next = map.get_tile(current.room, current.row - 1, current.col);
			//When the next tile is C
			if (next.type == 'C')
			{
				map.get_ending().previous = 'n';
				return true;
			}
			next.previous = 'n';
			list->add_tile(next);
			map.discover(next);
		}

		if (map.movable('w', current.room, current.row, current.col))
		{
			Tile next = map.get_tile(current.room, current.row, current.col - 1);

			//When the next tile is C
			if (next.type == 'C')
			{
				map.get_ending().previous = 'e';
				return true;
			}
			next.previous = 'e';
			list->add_tile(next);
			map.discover(next);
		}

		current.type == 'd';
	}
}

void label_path(Map map)
{
	Tile temp = map.get_ending();

	while (temp.type != 'S')
	{

		if (temp.previous == 'n')
		{
			temp = map.get_tile(temp.room, temp.row + 1, temp.col);
			temp.type = 's';
		}

		else if (temp.previous == 's')
		{
			temp = map.get_tile(temp.room, temp.row - 1, temp.col);
			temp.type = 'n';
		}

		else if (temp.previous == 'e')
		{
			temp = map.get_tile(temp.room, temp.row, temp.col + 1);
			temp.type = 'w';
		}

		else if (temp.previous == 'w')
		{
			temp = map.get_tile(temp.room, temp.row, temp.col - 1);
			temp.type = 'e';
		}

		else
		{
			temp = map.get_tile(static_cast<unsigned int>(temp.previous - '0'), temp.row, temp.col);
			temp.type = 'p';
		}
	}

}

void output_map(Map map)
{
	cout << "Start in room " << map.get_starting().room
		 << ", row " << map.get_starting().row
		 << ", column " << map.get_starting().col << "\n";

	for (unsigned int room = 0; room < map.get_num_room(); room++)
	{
		cout << "//castle room " << room << "\n";
		for (unsigned int row = 0; row < map.get_size_room(); row++)
		{
			for (unsigned int col = 0; col < map.get_size_room(); col++)
				cout << map.get_tile(room, row, col).type;
			cout << "\n";
		}
	}
}

void output_list(Map map)
{
	cout << "Path taken:\n";

	Tile temp = map.get_starting();
	while (temp.type != 'C')
	{
		cout << "(" << temp.room << ","
			 << temp.row << ","
			 << temp.col << ","
			 << temp.type << ")\n";

		if (temp.type == 'p')
		{
			temp = map.get_tile(temp.room, temp.row, temp.col);
			continue;
		}

		if (temp.type == 'n')
			temp = map.get_tile(temp.room, temp.row - 1, temp.col);
		else if (temp.type == 's')
			temp = map.get_tile(temp.room, temp.row + 1, temp.col);
		else if (temp.type == 'w')
			temp = map.get_tile(temp.room, temp.row, temp.col - 1);
		else if (temp.type == 'e')
			temp = map.get_tile(temp.room, temp.row, temp.col + 1);
	}
}
