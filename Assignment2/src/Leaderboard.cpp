#include "Leaderboard.h"


void Leaderboard::insert_new_entry(LeaderboardEntry * new_entry) {
    // TODO: Insert a new LeaderboardEntry instance into the leaderboard, such that the order of the high-scores
    //       is maintained, and the leaderboard size does not exceed 10 entries at any given time (only the
    //       top 10 all-time high-scores should be kept in descending order by the score).

    // if the leaderboard is empty or the new score is higher than the head's score
    if (!head_leaderboard_entry || new_entry->score > head_leaderboard_entry->score) {
        new_entry->next_leaderboard_entry = head_leaderboard_entry;
        head_leaderboard_entry = new_entry;
    } else {
        LeaderboardEntry* iter = head_leaderboard_entry;

        // traverse the list to find the correct position for new_entry
        while (iter->next_leaderboard_entry && new_entry->score <= iter->next_leaderboard_entry->score) {
            iter = iter->next_leaderboard_entry;
        }

        // insert the new entry at the correct position
        new_entry->next_leaderboard_entry = iter->next_leaderboard_entry;
        iter->next_leaderboard_entry = new_entry;
    }

    // increment the size
    size++;

    // remove the last entry if the leaderboard size exceeds 10
    if (size > MAX_LEADERBOARD_SIZE) {
        if (head_leaderboard_entry) {
            LeaderboardEntry* current = head_leaderboard_entry;

            // Traverse the list to the end (stop before the last element)
            while (current->next_leaderboard_entry->next_leaderboard_entry) {
                current = current->next_leaderboard_entry;
            }

            // remove the last entry
            delete current->next_leaderboard_entry;
            current->next_leaderboard_entry = nullptr;

            // decrement the size
            size--;
        }
    }
}


void Leaderboard::write_to_file(const string& filename) {
    std::ofstream out_file("leaderboard.txt", std::ios::app); // open the file with append mode

    if (!out_file) {
        std::cerr << "Error opening the file: " << filename << ". " << std::endl;
        return;
    }

    LeaderboardEntry* current = head_leaderboard_entry;

    while (current) {
        out_file << current->score << " " << current->last_played << " " << current->player_name << std::endl;
        current = current->next_leaderboard_entry;
    }

    out_file.close();
}


void Leaderboard::read_from_file(const string& filename) {
    // Open the file in append mode to create it if it does not exist
    std::ofstream out_file(filename, std::ios::app);
    out_file.close();

    // Now open the file for reading
    std::ifstream in_file(filename);

    if (!in_file) {
        std::cerr << "Error opening the file: " << filename << ". " << std::endl;
        return;
    }

    clear_leaderboard();

    long score;
    time_t timestamp;
    std::string name;

    while (in_file >> score >> timestamp >> name) {
        LeaderboardEntry* new_entry = new LeaderboardEntry(score, timestamp, name);

        if (!head_leaderboard_entry || score > head_leaderboard_entry->score) {
            new_entry->next_leaderboard_entry = head_leaderboard_entry;
            head_leaderboard_entry = new_entry;
        } else {
            LeaderboardEntry* current = head_leaderboard_entry;
            LeaderboardEntry* previous = nullptr;

            while (current && score <= current->score) {
                previous = current;
                current = current->next_leaderboard_entry;
            }

            new_entry->next_leaderboard_entry = current;

            if (previous) {
                previous->next_leaderboard_entry = new_entry;
            }
        }
    }

    in_file.close();
}


void Leaderboard::print_leaderboard() {
    // Print the current leaderboard status to the standard output in the format specified in the PA instructions
    // header
    std::cout << "Leaderboard" << std::endl;
    std::cout << "-----------" << std::endl;

    LeaderboardEntry* current = head_leaderboard_entry;
    int position = 1;

    //printing the linked list respectively
    while (current) {
        std::cout << std::setw(2) << position << ". " << std::setw(15) << std::left << current->player_name << " " << std::setw(5) << std::right << current->score << " ";

        // Convert time_t to tm struct
        struct tm * timeinfo = localtime (&(current->last_played));

        // Print time in the format "HH:MM:SS/DD.MM.YYYY"
        char buffer [80];
        strftime (buffer, 80, "%H:%M:%S/%d.%m.%Y", timeinfo);
        std::cout << buffer << std::endl;

        current = current->next_leaderboard_entry;
        position++;
    }
}

// Function to delete the existing linked list, while reading from leaderboard.txt file
void Leaderboard::clear_leaderboard() {// in order not to have multiple linked lists
    LeaderboardEntry* current = head_leaderboard_entry;
    LeaderboardEntry* next = nullptr;

    // deallocation process
    while (current) {
        next = current->next_leaderboard_entry;
        delete current;
        current = next;
    }

    head_leaderboard_entry = nullptr;
    size = 0;  // reset the size to zero
}


Leaderboard::~Leaderboard() {
    // Free dynamically allocated memory used for storing leaderboard entries
    LeaderboardEntry* iter = head_leaderboard_entry;

    while (iter) {
        LeaderboardEntry* temp = iter;
        iter = iter->next_leaderboard_entry;
        temp->next_leaderboard_entry = nullptr; // Set next pointer to nullptr before deletion
        delete temp;
    }

    head_leaderboard_entry = nullptr; // Set head to nullptr after deleting all entries
}

