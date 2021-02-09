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

bool action(Map &map, Search_list *list, Tile *current);
void label_path(Map &map);

void output_map(Map &map);
void output_list(Map &map);

int main(int argc, char **argv)
{
	ios_base::sync_with_stdio(false);

	unsigned int num_rooms, size_room;
	char input_mode;

	Map map;

	int err = map.get_options(argc, argv);
	if (err == 1)
		exit(1);

	cin >> input_mode >> num_rooms >> size_room;
	map.initialize(num_rooms, size_room, input_mode);

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

	Tile *starting = map.get_starting();

	//Add the starting position to search list
	list->add_tile(starting);
	map.discover(starting->room, starting->row, starting->col);

	bool result = false;

	while (!list->is_empty())
	{
		//1.Remove the next position
		Tile *current = map.get_tile(list->remove_tile());

		result = action(map, list, current);

		if (result == true)
		{
			label_path(map);
			if (map.get_output_mode() == 'M')
			{
				output_map(map);
				break;
			}
			else
			{
				output_list(map);
				break;
			}
		}
	}
	if (result == false)
		cout << "No solution, " << list->total_tiles << " tiles discovered.";

	delete list;

	return 0;
}

bool action(Map &map, Search_list *list, Tile *current)
{
	//Tile *current_tile = map.get_tile(current);

	//2. If the position is a warp pipe
	if (isdigit(current->type))
	{
		//When the room doesn't exist
		if (static_cast<unsigned int>(current->type - '0') >= map.get_num_room())
			return false;

		Tile *next = map.get_tile(static_cast<unsigned int>(current->type - '0'),
								  current->row, current->col);

		//When the warp pipe leads to itself
		// if (next == current_tile)
		// 	return false;

		//Check whether we can use the warp pipe
		if (!map.movable('p', current->room, current->row, current->col))
			return false;

		if (next->type == 'C')
		{
			map.set_prev(static_cast<char>('0' + current->room),
						 next->room, next->row, next->col);
			return true;
		}

		list->add_tile(next);

		map.set_prev(static_cast<char>('0' + current->room),
					 next->room, next->row, next->col);
		map.discover(next->room, next->row, next->col);
		return false;
	}

	//3. If the position isn't a warp pipe
	else
	{
		if (map.movable('n', current->room, current->row, current->col))
		{
			Tile *next = map.get_tile(current->room, current->row - 1, current->col);

			map.set_prev('s', next->room, next->row, next->col);

			//When the next tile is C
			if (next->type == 'C')
			{
				return true;
			}

			list->add_tile(next);

			map.discover(next->room, next->row, next->col);
		}

		if (map.movable('e', current->room, current->row, current->col))
		{
			Tile *next = map.get_tile(current->room, current->row, current->col + 1);

			map.set_prev('w', next->room, next->row, next->col);

			//When the next tile is C
			if (next->type == 'C')
			{
				return true;
			}

			list->add_tile(next);
			map.discover(next->room, next->row, next->col);
		}

		if (map.movable('s', current->room, current->row, current->col))
		{
			Tile *next = map.get_tile(current->room, current->row + 1, current->col);

			map.set_prev('n', next->room, next->row, next->col);

			//When the next tile is C
			if (next->type == 'C')
			{
				return true;
			}

			list->add_tile(next);
			map.discover(next->room, next->row, next->col);
		}

		if (map.movable('w', current->room, current->row, current->col))
		{
			Tile *next = map.get_tile(current->room, current->row, current->col - 1);

			map.set_prev('e', next->room, next->row, next->col);

			//When the next tile is C
			if (next->type == 'C')
			{
				return true;
			}

			list->add_tile(next);
			map.discover(next->room, next->row, next->col);
		}
	}

	return false;
}

void label_path(Map &map)
{
	Tile *temp = map.get_ending();

	while (temp != map.get_starting())
	{

		if (temp->previous == 'n')
		{
			temp = map.get_tile(temp->room,
								static_cast<unsigned int>(temp->row - 1), temp->col);
			temp->type = 's';
		}

		else if (temp->previous == 's')
		{
			temp = map.get_tile(temp->room,
								static_cast<unsigned int>(temp->row + 1), temp->col);
			temp->type = 'n';
		}

		else if (temp->previous == 'e')
		{
			temp = map.get_tile(temp->room, temp->row,
								static_cast<unsigned int>(temp->col + 1));
			temp->type = 'w';
		}

		else if (temp->previous == 'w')
		{
			temp = map.get_tile(temp->room, temp->row,
								static_cast<unsigned int>(temp->col - 1));
			temp->type = 'e';
		}

		else
		{
			temp = map.get_tile(static_cast<unsigned int>(temp->previous - '0'),
								temp->row, temp->col);
			//temp->type = 'p';
		}
	}
}

void output_map(Map &map)
{
	cout << "Start in room " << map.get_starting()->room
		 << ", row " << map.get_starting()->row
		 << ", column " << map.get_starting()->col << "\n";

	Tile *temp = map.get_starting();
	while (temp->type != 'C')
	{

		if (temp->type == 'n')
			temp = map.get_tile(temp->room, temp->row - 1, temp->col);
		else if (temp->type == 's')
			temp = map.get_tile(temp->room, temp->row + 1, temp->col);
		else if (temp->type == 'w')
			temp = map.get_tile(temp->room, temp->row, temp->col - 1);
		else if (temp->type == 'e')
			temp = map.get_tile(temp->room, temp->row, temp->col + 1);
		else
		{
			Tile *warp = temp;
			temp = map.get_tile(static_cast<unsigned int>(temp->type - '0'),
								temp->row, temp->col);
			warp->type = 'p';
		}
	}

	for (unsigned int room = 0; room < map.get_num_room(); room++)
	{
		cout << "//castle room " << room << "\n";
		for (unsigned int row = 0; row < map.get_size_room(); row++)
		{
			for (unsigned int col = 0; col < map.get_size_room(); col++)
			{
				Tile *t = map.get_tile(room, row, col);
				cout << t->type;
			}
			cout << "\n";
		}
	}
}

void output_list(Map &map)
{
	cout << "Path taken:\n";

	Tile *temp = map.get_starting();
	while (temp->type != 'C')
	{
		if (isdigit(temp->type))
		{
			cout << "(" << temp->room << ","
				 << temp->row << ","
				 << temp->col << ","
				 << 'p' << ")\n";
		}
		else
		{
			cout << "(" << temp->room << ","
				 << temp->row << ","
				 << temp->col << ","
				 << temp->type << ")\n";
		}
		if (temp->type == 'n')
			temp = map.get_tile(temp->room, temp->row - 1, temp->col);
		else if (temp->type == 's')
			temp = map.get_tile(temp->room, temp->row + 1, temp->col);
		else if (temp->type == 'w')
			temp = map.get_tile(temp->room, temp->row, temp->col - 1);
		else if (temp->type == 'e')
			temp = map.get_tile(temp->room, temp->row, temp->col + 1);
		else
			temp = map.get_tile(static_cast<unsigned int>(temp->type - '0'), temp->row, temp->col);
	}
}
