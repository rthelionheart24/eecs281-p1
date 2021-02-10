//Project Identifier: B99292359FFD910ED13A7E6C7F9705B8742F0D79
// Created by richa on 1/28/2021.
//
#include <iostream>
#include <vector>
#include <string>
#include <getopt.h>
#include "Tile.h"

#ifndef P1_MAP_H
#define P1_MAP_H

class Map
{
private:
	//Vectorized representation of the map
	std::vector<std::vector<std::vector<Tile>>> layout;

	//Number of rooms in the map
	unsigned int num_rooms;

	//Length of each room
	unsigned int size_room;

	//The algorithm used to search the board
	char search_mode = 'u';

	//Reading mode
	char input_mode, output_mode = 'u';

	//Location of starting and ending positions
	Coordinates starting, ending;

public:
	void initialize(unsigned int in_num_rooms,
					unsigned int in_size_room,
					char in_input_mode);

	//Get options from the command line to initialize parameters
	int get_options(int argc, char **argv);

	//Read in map-form data
	void read_map();

	//Read in list-form data
	void read_list();

	void discover(Coordinates c) { layout[c.room][c.row][c.col].discover = true; }

	Coordinates get_starting() const { return starting; }

	Coordinates get_ending() const { return ending; }

	Tile &get_tile(unsigned int room,
				   unsigned int row,
				   unsigned int col) { return layout[room][row][col]; }

	Tile &get_tile(Coordinates c) { return layout[c.room][c.row][c.col]; }

	void set_prev(char prev, Coordinates c) { layout[c.room][c.row][c.col].previous = prev; }

	bool movable(char direction, Coordinates c);

	unsigned int get_size_room() { return size_room; }

	unsigned int get_num_room() { return num_rooms; }

	char get_output_mode() { return output_mode; }

	char get_search_mode() { return search_mode; }

	bool check_legal_type(char c);

	bool check_legal_coord(char dimension, unsigned int coord);
};

#endif //P1_MAP_H
