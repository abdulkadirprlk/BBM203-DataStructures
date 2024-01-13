#include <fstream>
#include <sstream>
#include <iostream>

#include "BlockFall.h"


BlockFall::BlockFall(string grid_file_name, string blocks_file_name, bool gravity_mode_on, const string &leaderboard_file_name, const string &player_name) : gravity_mode_on(
        gravity_mode_on), leaderboard_file_name(leaderboard_file_name), player_name(player_name) {
    initialize_grid(grid_file_name);
    read_blocks(blocks_file_name);
    leaderboard.read_from_file(leaderboard_file_name);
}


std::vector<std::string> BlockFall::splitString(const std::string& input, char delimiter) {
    std::vector<std::string> tokens;
    std::istringstream stream(input);
    std::string token;

    while (std::getline(stream, token, delimiter)) {
        tokens.push_back(token);
    }
    
    return tokens;
}


Block* BlockFall::generate_right_rotation(Block* original_block) {
    int height = original_block->shape.size();
    int width = original_block->shape[0].size();

    // Create a block and instantiate its shape (width and height are swapped because of rotation)
    Block* rotated_block = new Block(width, height);

    // Generate the right rotation
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            bool originalValue = original_block->shape[i][j];
            rotated_block->shape[j][height - 1 - i] = originalValue;
        }
    }
    return rotated_block;
    
}


Block* BlockFall::generate_left_rotation(Block* original_block) {
    int height = original_block->shape.size();
    int width = original_block->shape[0].size();

    // Create a block and instantiate its shape (width and height are swapped because of rotation)
    Block* rotated_block = new Block(width, height);

    // Generate the left rotation
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            bool originalValue = original_block->shape[i][j];
            rotated_block->shape[width - 1 - j][i] = originalValue;
        }
    }
    return rotated_block;
}


// creates a block pointer, takes a block string ie "101\n010" as argument
Block* BlockFall::make_block(const std::string& input_string){

    // find the height and weight of the shape of the block
    std::vector<std::string> block_vector = splitString(input_string, '\n'); // splits the block string into a vector
    int block_height = block_vector.size();
    int block_width = block_vector[0].size();

    // INSTANTIATE THE BLOCK (FROM A VECTOR CONTANINING DATA OF THE SHAPE)
    Block* block = new Block(block_vector);

    return block;
}


void BlockFall::read_blocks(const string &input_file) {
    // TODO: Read the blocks from the input file and initialize "initial_block" and "active_rotation" member variables

    // Open a file for reading
    std::ifstream inputFile(input_file);

    // Check if the file is open
    if (!inputFile.is_open()) {
        std::cerr << "Error file cannot open!" << std::endl;
        return; // Exit the program with an error code
    }

    // Vector to store raw blocks(not processed)
    std::vector<std::string> raw_blocks;

    std::string line;
    // Read the file block by block
    // Read the file block by block
    while (std::getline(inputFile, line, ']')) {
        // Skip empty lines
        if (line.find_first_not_of(" \t\n\r\f\v") == std::string::npos) {
            continue;
        }

        // remove leading whitespace and '['
        line.erase(0, line.find_first_not_of("[ \t\n\r\f\v"));

        raw_blocks.push_back(line);
    }
    
    inputFile.close();// file is read.

    // create the power-up
    std::vector<std::string> power_up_vector = splitString(raw_blocks[raw_blocks.size() - 1], '\n');
    int power_up_height = power_up_vector.size();
    int power_up_width = power_up_vector[0].size();

    power_up = std::vector<std::vector<bool>>(power_up_height, std::vector<bool>(power_up_width));

    // fill the power_up's shape with corresponding 1's and 0's
    for(int j = 0; j < power_up_height; j++) {
        for (int k = 0; k < power_up_width; k++) {
            if(power_up_vector[j][k] == '1') {
                power_up[j][k] = true;
            } else {
                power_up[j][k] = false;
            }
        }
    }

    // FIRST ELEMENT IN THE LINKED LIST
    // create the first block of the linked list
    Block* block = make_block(raw_blocks[0]);

    // initial block is this block
    initial_block = block;
    active_rotation = initial_block;
    Block* current_block = initial_block; // to iterate over the singly linked list

    // SINGLY LINKED LIST STRUCTURE
    for(int i = 1; i < raw_blocks.size() - 1; i++){// power-up excluded
        
        //create a new block and add it to linked list (generate its left and right rotations)
        Block* new_block = make_block(raw_blocks[i]);//dynamic memory allocation???
        current_block->next_block = new_block;
        current_block = current_block->next_block; // iter = iter->next 
        
    }   

    // DOUBLY CIRCULAR LINKED LIST STRUCTURE
    // iterate over the singly linked list and create all the possible rotations
    Block* original_block = initial_block;
    while(original_block != nullptr){
        original_block = create_rotation_list(original_block, original_block->next_block);
        original_block = original_block->next_block;  
    }
/////////printing the list
/*
    Block* current = initial_block;   
    while (current != nullptr) {
        // Print the content of the 2D bool vector in the current node
        for (const auto& row : current->shape) {
            for (bool value : row) {
                // Print the boolean value
                std::cout << value << " ";
            }
            std::cout << std::endl; // Move to the next line after each row
        }

        std::cout << "------" << std::endl; // Separator between nodes

        // Move to the next node in the linked list
        current = current->next_block;
    }
*/
//////////////

    // TODO: For every block, generate its rotations and properly implement the multilevel linked list structure
    //       that represents the game blocks, as explained in the PA instructions.
    // TODO: Initialize the "power_up" member variable as the last block from the input file (do not add it to the linked list!)
}


// Function to create a circular doubly linked list of rotations for a block
Block* BlockFall::create_rotation_list(Block*& original_block, Block*& next_original_block) {

    Block* lefted = generate_left_rotation(original_block);
    // Check if the block has rotations
    if (original_block->shape == lefted->shape) {
        delete lefted;//deallocation because it has no rotations
        original_block->right_rotation = original_block;
        original_block->left_rotation = original_block;
        return original_block; // Block has no rotations
    }

    // Generate the right and left rotations
    Block* righted = generate_right_rotation(original_block);
    
    Block* right_right_rotation = generate_right_rotation(righted);

    // Connect blocks in a circular doubly linked list
    original_block->right_rotation = righted;
    righted->left_rotation = original_block;

    righted->right_rotation = right_right_rotation;
    right_right_rotation->left_rotation = righted;
    right_right_rotation->right_rotation = lefted;
    lefted->left_rotation = right_right_rotation;

    lefted->right_rotation = original_block;
    original_block->left_rotation = lefted;

    // Connect all rotations' pointers to the next_block_node
    if (next_original_block) {
        lefted->next_block = next_original_block;
        righted->next_block = next_original_block;
        right_right_rotation->next_block = next_original_block;
    }

    return original_block; // Return the head of the circular doubly linked list
}


void BlockFall::initialize_grid(const string &input_file) {
    // TODO: Initialize "rows" and "cols" member variables
    std::ifstream inputFile(input_file);

    // Check if the file is open
    if (!inputFile.is_open()) {
        std::cerr << "Error: file cannot open!" << std::endl;
        return; // Exit the program with an error code
    }

    std::string line;
    while (std::getline(inputFile, line)) {

        // create a stringstream from the line
        std::istringstream iss(line);

        // Vector to store the current row
        std::vector<int> row;

        int value;
        // read integers from the stringstream and add them to the current row
        while (iss >> value) {
            row.push_back(value);
        }

        // add line row to the game grid
        grid.push_back(row);
    }

    inputFile.close();// grid file is read.

    rows = grid[0].size();
    cols = grid.size();
    // TODO: Initialize "grid" member variable using the command-line argument 1 in main

    
}

BlockFall::~BlockFall() {
    // TODO: Free dynamically allocated memory used for storing game blocks

    // iterate over the singly linked list of blocks
    while (initial_block) {
        Block* iter = initial_block;// iterator

        // iterate over the doubly circular linked list of rotations
        Block* current_rotation = iter->left_rotation;
        while (current_rotation && current_rotation != initial_block) {
            Block* next_rotation = current_rotation->left_rotation;
            delete current_rotation;
            current_rotation = next_rotation;
        }

        initial_block = initial_block->next_block; // Move to the next block in the singly linked list
        delete iter;
    }
    
}
