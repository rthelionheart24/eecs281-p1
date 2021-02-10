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

bool action(Map &map, Search_list *list, Coordinates current_c);
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

	Coordinates starting = map.get_starting();

	//Add the starting position to search list
	list->add_tile(starting);
	map.discover(starting);

	bool result = false;

	while (!list->is_empty())
	{
		//1.Remove the next position
		Coordinates current_c = list->remove_tile();

		result = action(map, list, current_c);

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

bool action(Map &map, Search_list *list, Coordinates current_c)
{
	Tile current_t = map.get_tile(current_c);

	//2. If the position is a warp pipe
	if (isdigit(current_t.type))
	{
		//When the room doesn't exist
		if (static_cast<unsigned int>(current_t.type - '0') >= map.get_num_room())
			return false;

		Coordinates next_c = {static_cast<unsigned int>(current_t.type - '0'),
							  current_c.row,
							  current_c.col};

		//Check whether we can use the warp pipe
		if (!map.movable('p', current_c))
			return false;

		if (map.get_tile(next_c).type == 'C')
		{
			map.set_prev(static_cast<char>('0' + current_c.room), next_c);
			return true;
		}

		list->add_tile(next_c);
		map.set_prev(static_cast<char>('0' + current_c.room), next_c);
		map.discover(next_c);
		return false;
	}

	//3. If the position isn't a warp pipe
	else
	{
		if (map.movable('n', current_c))
		{
			Coordinates next_c = {current_c.room, current_c.row - 1, current_c.col};

			map.set_prev('s', next_c);

			Tile next_t = map.get_tile(next_c);

			//When the next tile is C
			if (next_t.type == 'C')
				return true;

			list->add_tile(next_c);

			map.discover(next_c);
		}

		if (map.movable('e', current_c))
		{
			Coordinates next_c = {current_c.room, current_c.row, current_c.col + 1};

			map.set_prev('w', next_c);

			Tile next_t = map.get_tile(next_c);

			//When the next tile is C
			if (next_t.type == 'C')
				return true;

			list->add_tile(next_c);
			map.discover(next_c);
		}

		if (map.movable('s', current_c))
		{
			Coordinates next_c = {current_c.room, current_c.row + 1, current_c.col};

			map.set_prev('n', next_c);

			Tile next_t = map.get_tile(next_c);

			//When the next tile is C
			if (next_t.type == 'C')
				return true;

			list->add_tile(next_c);
			map.discover(next_c);
		}

		if (map.movable('w', current_c))
		{
			Coordinates next_c = {current_c.room, current_c.row, current_c.col - 1};

			map.set_prev('e', next_c);

			Tile next_t = map.get_tile(next_c);

			//When the next tile is C
			if (next_t.type == 'C')
				return true;

			list->add_tile(next_c);
			map.discover(next_c);
		}
	}

	return false;
}

void label_path(Map &map)
{
	Coordinates temp = map.get_ending();

	while (temp.room != map.get_starting().room ||
		   temp.row != map.get_starting().row ||
		   temp.col != map.get_starting().col)
	{

		if (map.get_tile(temp).previous == 'n')
		{
			temp = {temp.room, static_cast<unsigned int>(temp.row - 1), temp.col};

			map.get_tile(temp).type = 's';
		}

		else if (map.get_tile(temp).previous == 's')
		{
			temp = {temp.room, static_cast<unsigned int>(temp.row + 1), temp.col};

			map.get_tile(temp).type = 'n';
		}

		else if (map.get_tile(temp).previous == 'e')
		{
			temp = {temp.room, temp.row, static_cast<unsigned int>(temp.col + 1)};

			map.get_tile(temp).type = 'w';
		}

		else if (map.get_tile(temp).previous == 'w')
		{
			temp = {temp.room, temp.row, static_cast<unsigned int>(temp.col - 1)};

			map.get_tile(temp).type = 'e';
		}
		else if (isdigit(map.get_tile(temp).previous))
		{
			temp = {static_cast<unsigned int>(map.get_tile(temp).previous - '0'),
					temp.row, temp.col};

			//temp->type = 'p';
		}
	}
}

void output_map(Map &map)
{
	cout << "Start in room " << map.get_starting().room
		 << ", row " << map.get_starting().row
		 << ", column " << map.get_starting().col << "\n";

	Coordinates temp = map.get_starting();

	while (map.get_tile(temp).type != 'C')
	{

		if (map.get_tile(temp).type == 'n')
			temp = {temp.room, temp.row - 1, temp.col};
		else if (map.get_tile(temp).type == 's')
			temp = {temp.room, temp.row + 1, temp.col};
		else if (map.get_tile(temp).type == 'w')
			temp = {temp.room, temp.row, temp.col - 1};
		else if (map.get_tile(temp).type == 'e')
			temp = {temp.room, temp.row, temp.col + 1};
		else
		{
			Coordinates next = {static_cast<unsigned int>(map.get_tile(temp).type - '0'),
								temp.row, temp.col};
			map.get_tile(temp).type = 'p';
			temp = next;
		}
	}

	for (unsigned int room = 0; room < map.get_num_room(); room++)
	{
		cout << "//castle room " << room << "\n";
		for (unsigned int row = 0; row < map.get_size_room(); row++)
		{
			for (unsigned int col = 0; col < map.get_size_room(); col++)
			{
				cout << map.get_tile(room, row, col).type;
			}
			cout << "\n";
		}
	}
}

void output_list(Map &map)
{
	cout << "Path taken:\n";

	Coordinates temp = map.get_starting();

	while (map.get_tile(temp).type != 'C')
	{
		if (isdigit(map.get_tile(temp).type))
		{
			cout << "(" << temp.room << ","
				 << temp.row << ","
				 << temp.col << ","
				 << 'p' << ")\n";
		}
		else
		{
			cout << "(" << temp.room << ","
				 << temp.row << ","
				 << temp.col << ","
				 << map.get_tile(temp).type << ")\n";
		}
		if (map.get_tile(temp).type == 'n')
			temp = {temp.room, temp.row - 1, temp.col};
		else if (map.get_tile(temp).type == 's')
			temp = {temp.room, temp.row + 1, temp.col};
		else if (map.get_tile(temp).type == 'w')
			temp = {temp.room, temp.row, temp.col - 1};
		else if (map.get_tile(temp).type == 'e')
			temp = {temp.room, temp.row, temp.col + 1};
		else
			temp = {static_cast<unsigned int>(map.get_tile(temp).type - '0'), temp.row, temp.col};
	}
}
