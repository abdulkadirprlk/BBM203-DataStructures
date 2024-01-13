#ifndef SPACESECTORLLRBT_H
#define SPACESECTORLLRBT_H

#include "Sector.h"
#include <iostream>
#include <fstream>  
#include <sstream>
#include <vector>
#include <queue>
#include <algorithm>

class SpaceSectorLLRBT {
public:
    Sector* root;
    SpaceSectorLLRBT();
    ~SpaceSectorLLRBT();
    void readSectorsFromFile(const std::string& filename);
    void insertSectorByCoordinates(int x, int y, int z);
    void displaySectorsInOrder();
    void displaySectorsPreOrder();
    void displaySectorsPostOrder();
    std::vector<Sector*> getStellarPath(const std::string& sector_code);
    void printStellarPath(const std::vector<Sector*>& path);
    void displayTreeStructure(const Sector* node, int level) const; 

private:
    
    bool isRed(Sector* sector);
    void fixUp(Sector*& root);
    Sector* rotateLeft(Sector*& sector);
    Sector* rotateRight(Sector*& sector);
    void flipColors(Sector*& sector);
    void deleteWholeTree(Sector* root);
    Sector* searchSector(Sector* root, std::string sector_code);  
    void displaySectorsInOrderHelper(Sector* root);
    void displaySectorsPreOrderHelper(Sector* root);
    void displaySectorsPostOrderHelper(Sector* root);
    std::vector<std::string> split(const std::string& str, char delimiter);//custom split function
    Sector* insertSectorByCoordinatesHelper(Sector*& root, Sector*& new_sector);// helper function for insertSectorByCoordinates
    
};

#endif // SPACESECTORLLRBT_H
