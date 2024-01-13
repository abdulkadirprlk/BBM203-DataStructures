#include "Sector.h"
#include <cmath>

// Constructor implementation

Sector::Sector(int x, int y, int z) : x(x), y(y), z(z), left(nullptr), right(nullptr), parent(nullptr), color(RED) {
        // TODO: Calculate the distance to the Earth, and generate the sector code
        distance_from_earth = sqrt(pow(x,2) + pow(y,2) + pow(z,2));

        // generate the sector code
        sector_code = std::to_string(int(distance_from_earth));
        if(x > 0){sector_code += "R";}
        if(x == 0){sector_code += "S";}
        if(x < 0){sector_code += "L";}

        if(y > 0){sector_code += "U";}
        if(y == 0){sector_code += "S";}
        if(y < 0){sector_code += "D";}

        if(z > 0){sector_code += "F";}
        if(z == 0){sector_code += "S";}
        if(z < 0){sector_code += "B";}
}

Sector::~Sector() {
    // TODO: Free any dynamically allocated memory if necessary
}

Sector& Sector::operator=(const Sector& other) {
    if (this != &other) {
        this->distance_from_earth = other.distance_from_earth;
        this->sector_code = other.sector_code;
        this->x = other.x;
        this->y = other.y;
        this->z = other.z;
        this->color = other.color;
        if (other.right) {
            delete right;
            right = new Sector(*other.right);
        } else {
            delete right;
            right = nullptr;
        }

        if (other.left) {
            delete left;
            left = new Sector(*other.left);
        } else {
            delete left;
            left = nullptr;
        }
    }

    return *this;
}

bool Sector::operator==(const Sector& other) const {
    return (x == other.x && y == other.y && z == other.z);
}

bool Sector::operator!=(const Sector& other) const {
    return !(*this == other);
}
