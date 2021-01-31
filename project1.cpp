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

int main(int argc, char **argv)
{
	ios_base::sync_with_stdio(false);

	unsigned int num_rooms, size_room;
	char input_mode;
	//int room, col, row;

	cin >> input_mode >> num_rooms >> size_room;

	Map map(num_rooms, size_room, input_mode);

	map.get_options(argc, argv);

	if (input_mode == 'M')
		map.read_map();
	else if (input_mode == 'L')
		map.read_list();

	cout << "Great\n";

	return 0;
}
