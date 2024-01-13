#ifndef SPACESECTORBST_H
#define SPACESECTORBST_H

#include <iostream>
#include <fstream>  
#include <sstream>
#include <vector>
#include <queue>

#include "Sector.h"

class SpaceSectorBST {
  
public:
    Sector *root;
    SpaceSectorBST();
    ~SpaceSectorBST();
    void readSectorsFromFile(const std::string& filename); 
    void insertSectorByCoordinates(int x, int y, int z);
    void deleteSector(const std::string& sector_code);
    void displaySectorsInOrder();
    void displaySectorsPreOrder();
    void displaySectorsPostOrder();
    std::vector<Sector*> getStellarPath(const std::string& sector_code);
    void printStellarPath(const std::vector<Sector*>& path);
    void displayTreeStructure(const Sector* node, int level) const; 

//private:
    void deleteWholeTree(Sector*& root);
    void displaySectorsPreOrderHelper(Sector* root);
    void displaySectorsInOrderHelper(Sector* root);
    void displaySectorsPostOrderHelper(Sector* root);
    Sector* searchSector(Sector* root, std::string sector_code);
    bool isLessThan(const std::string& sector_code1, const std::string& sector_code2);
    std::vector<std::string> split(const std::string& str, char delimiter);//custom split function
    void insertSectorByCoordinatesHelper(Sector*& root, Sector*& new_sector);// helper function for insertSectorByCoordinates
    Sector* findMin(Sector* root);
    Sector* deleteSectorHelper(Sector* root, const std::string& sector_code);
    
};

#endif // SPACESECTORBST_H
