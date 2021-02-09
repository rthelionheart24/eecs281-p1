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
	//Number of rooms in the map
	unsigned int num_rooms;

	//Length of each room
	unsigned int size_room;

	//The algorithm used to search the board
	char search_mode = 'u';

	//Reading mode
	char input_mode, output_mode = 'u';

	//Vectorized representation of the map
	std::vector<std::vector<std::vector<Tile>>> layout;

	//Location of starting and ending positions
	Tile *starting, *ending;

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

	void discover(unsigned int room,
				  unsigned int row,
				  unsigned int col) { layout[room][row][col].discover = true; }

	Tile *get_starting() const
	{
		return starting;
	}

	Tile *get_ending() const { return ending; }

	Tile *get_tile(unsigned int room,
				   unsigned int row,
				   unsigned int col) { return &layout[room][row][col]; }

	void set_prev(char prev, unsigned int room,
				  unsigned int row, unsigned int col) { layout[room][row][col].previous = prev; }

	bool movable(char direction, unsigned int room,
				 unsigned int row, unsigned int col);

	unsigned int get_size_room() { return size_room; }

	unsigned int get_num_room() { return num_rooms; }

	char get_output_mode() { return output_mode; }

	char get_search_mode() { return search_mode; }

	bool check_legal_type(char c);

	bool check_legal_coord(std::string dimension, unsigned int coord);
};

#endif //P1_MAP_H
