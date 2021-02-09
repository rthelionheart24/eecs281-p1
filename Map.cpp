//Project Identifier: B99292359FFD910ED13A7E6C7F9705B8742F0D79
#include "Map.h"

void Map::initialize(unsigned int in_num_rooms,
                     unsigned int in_size_room,
                     char in_input_mode)
{
    num_rooms = in_num_rooms;
    size_room = in_size_room;
    input_mode = in_input_mode;

    layout.resize(static_cast<unsigned int>(num_rooms));
    for (unsigned int i = 0; i < num_rooms; i++)
    {
        layout[i].resize(static_cast<unsigned int>(size_room));
        for (unsigned int j = 0; j < size_room; j++)
        {
            layout[i][j].resize(static_cast<unsigned int>(size_room));
        }
    }

}
int Map::get_options(int argc, char *argv[])
{
    int err = 0;

    int choice;
    int option_index = 0;

    option long_options[] = {
        {"output", required_argument, nullptr, 'o'},
        {"stack", no_argument, nullptr, 's'},
        {"queue", no_argument, nullptr, 'q'},
        {"help", no_argument, nullptr, 'h'},
        {nullptr, 0, nullptr, '\0'}};

    while ((choice = getopt_long(argc, argv, "o:sqh", long_options, &option_index)) != -1)
    {
        switch (choice)
        {
        case 'h':
            std::cout
                << "This program reads in coordinates of a map in either map mode or list mode\n"
                << "It then figures out whether there is a way to rescue the countess\n"
                << "If there is, the program will output the path; if there isn't , the "
                   "program will exit\n";

            exit(0);
            break;

        case 's':
            if (search_mode == 's' || search_mode == 'q')
            {
                std::cerr << "Stack or queue can only be specified once\n";
                err = 1;
                return err;
            }
            search_mode = 's';
            break;

        case 'q':
            if (search_mode == 's' || search_mode == 'q')
            {
                std::cerr << "Stack or queue can only be specified once\n";
                err = 1;
                return err;
            }
            search_mode = 'q';
            break;

        case 'o':
            if (output_mode == 'L' || output_mode == 'M')
            {
                std::cerr << "More than one -o arg\n";
                err = 1;
                return err;
            }
            output_mode = *optarg;
            break;

        default:
            std::cerr << "Unknown command line option\n";
            err = 1;
            return err;
            break;
        }
    }
    if (search_mode == 'u')
    {
        std::cerr << "Stack or queue must be specified\n";
        err = 1;
        return err;
    }

    if (output_mode == 'u')
        output_mode = 'M';

    return err;
}

void Map::read_map()
{

    unsigned int room = 0, col = 0, row = 0;
    char type;
    std::string junk;

    while (std::cin >> type)
    {
        //Check for comments
        if (type == '/')
        {
            std::getline(std::cin, junk);
            continue;
        }

        //Check for illegal entries
        if (!check_legal_type(type))
        {
            std::cerr << "Unknown map character\n";
            exit(1);
        }

        //Push tile into the map
        Tile temp = {room, row, col, type};
        layout[room][row][col] = temp;

        //check for starting and ending point
        if (type == 'S')
        {
            starting = &layout[room][row][col];
        }
        if (type == 'C')
        {
            ending = &layout[room][row][col];
        }

        col++;

        if (col == size_room)
        {
            col = 0;
            row++;

            if (row == size_room)
            {
                room++;
                row = 0;
            }
        }
    }
}

void Map::read_list()
{
    unsigned int room, col, row;
    char type, junk;
    std::string junk_line;

    while (std::cin >> type)
    {
        //Check for comments
        if (type == '/')
        {
            std::getline(std::cin, junk_line);
            continue;
        }

        std::cin >> room >> junk >> row >> junk >> col >> junk >> type >> junk;

        //Check for illegal entries
        if (!check_legal_type(type))
        {
            std::cerr << "Unknown map character\n";
            exit(1);
        }

        //Check for illegal coordinates
        if (!check_legal_coord("room", room))
        {
            std::cerr << "Invalid room number\n";
            exit(1);
        }
        if (!check_legal_coord("row", row))
        {
            std::cerr << "Invalid row number\n";
            exit(1);
        }
        if (!check_legal_coord("col", col))
        {
            std::cerr << "Invalid column number\n";
            exit(1);
        }

        //Push tile into the map
        Tile temp = {room, row, col, type};
        layout[room][row][col] = temp;

        //check for starting and ending point
        if (type == 'S')
        {
            starting = &layout[room][row][col];
        }
        if (type == 'C')
        {
            ending = &layout[room][row][col];
        }
    }
    for (unsigned int room = 0; room < num_rooms; room++)
    {
        for (unsigned int row = 0; row < size_room; row++)
        {
            for (unsigned int col = 0; col < size_room; col++)
            {
                layout[room][row][col].room = room;
                layout[room][row][col].row = row;
                layout[room][row][col].col = col;
            }
        }
    }
}

bool Map::movable(char direction, unsigned int room,
                  unsigned int row, unsigned int col)
{
    if (direction == 'n')
    {
        if (row == 0)
            return false;
        return layout[room][row - 1][col].type != '#' &&
               layout[room][row - 1][col].type != '!' &&
               !layout[room][row - 1][col].discover;
    }

    else if (direction == 's')
    {
        if (row == size_room - 1)
            return false;
        return layout[room][row + 1][col].type != '#' &&
               layout[room][row + 1][col].type != '!' &&
               !layout[room][row + 1][col].discover;
    }
    else if (direction == 'e')
    {
        if (col == size_room - 1)
            return false;
        return layout[room][row][col + 1].type != '#' &&
               layout[room][row][col + 1].type != '!' &&
               !layout[room][row][col + 1].discover;
    }

    else if (direction == 'w')
    {
        if (col == 0)
            return false;
        return layout[room][row][col - 1].type != '#' &&
               layout[room][row][col - 1].type != '!' &&
               !layout[room][row][col - 1].discover;
    }
    else
    {
        Tile *warp = &layout[static_cast<unsigned int>(layout[room][row][col].type - '0')][row][col];

        return warp->type != '#' &&
               warp->type != '!' &&
               !warp->discover;
    }
}

bool Map::check_legal_coord(std::string dimension, unsigned int coord)
{
    if (dimension == "room")
        return coord < num_rooms;
    return coord < size_room;
}

bool Map::check_legal_type(char c)
{
    return (c == '.' ||
            c == '#' ||
            c == '!' ||
            c == 'S' ||
            c == 'C' ||
            isdigit(c));
}