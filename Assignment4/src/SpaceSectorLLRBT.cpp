#include "SpaceSectorLLRBT.h"

using namespace std;

SpaceSectorLLRBT::SpaceSectorLLRBT() : root(nullptr) {}

void SpaceSectorLLRBT::readSectorsFromFile(const std::string& filename) {
    // TODO: read the sectors from the input file and insert them into the LLRBT sector map
    // according to the given comparison critera based on the sector coordinates.

    std::ifstream input_file(filename); 

    if (!input_file.is_open()) {
        std::cerr << "Error opening file" << std::endl;// file cannot be open
        return;
    }

    std::string line;
    while (std::getline(input_file, line)) {
        // Process each line here except the first line
        if(line == "") continue;// if line is empty, continue

        if(line[line.size() - 1] == '\r' || line[line.size() - 1] == '\n' || line[line.size() - 1] == '\t'){
            line = line.substr(0, line.size() - 1);
        }
        if (line == "X,Y,Z") {// ignore the first line
            continue;
        }
        std::vector<std::string> line_vector = split(line, ',');
        int x = std::stoi(line_vector[0]);
        int y = std::stoi(line_vector[1]);
        int z = std::stoi(line_vector[2]);
        insertSectorByCoordinates(x, y, z);
    }
    // insertions are done!
    
    input_file.close(); // Closing the file after reading
}

// Remember to handle memory deallocation properly in the destructor.
SpaceSectorLLRBT::~SpaceSectorLLRBT() {
    // TODO: Free any dynamically allocated memory in this class.
    deleteWholeTree(root);
}

void SpaceSectorLLRBT::insertSectorByCoordinates(int x, int y, int z) {
    // TODO: Instantiate and insert a new sector into the space sector LLRBT map 
    // according to the coordinates-based comparison criteria.

    Sector* new_sector = new Sector(x, y, z);// instantiate a new sector
    
    if(root == nullptr){// if the tree is empty (inserting the first sector)
        root = new_sector;
        root->parent = nullptr;
        root->color = RED;
        return;
    }
    root = insertSectorByCoordinatesHelper(root, new_sector);// call the helper function
    root->color = BLACK;// the root must be black always

}

Sector* SpaceSectorLLRBT::insertSectorByCoordinatesHelper(Sector*& root, Sector*& new_sector) {
    if(root == nullptr) {
        root = new_sector;
        root->parent = nullptr;
        root->color = RED;
        root->left = nullptr;
        root->right = nullptr;
        return root;
    }
    if(root->x > new_sector->x) {
        root->left = insertSectorByCoordinatesHelper(root->left, new_sector);
    }
    else if(root->x < new_sector->x) {
        root->right = insertSectorByCoordinatesHelper(root->right, new_sector);
    }
    else {
        if(root->y > new_sector->y) {
            root->left = insertSectorByCoordinatesHelper(root->left, new_sector);
        }
        else if(root->y < new_sector->y) {
            root->right = insertSectorByCoordinatesHelper(root->right, new_sector);
        }
        else {
            if(root->z > new_sector->z) {
                root->left = insertSectorByCoordinatesHelper(root->left, new_sector);
            }
            else if(root->z < new_sector->z) {
                root->right = insertSectorByCoordinatesHelper(root->right, new_sector);
            }
        }
    }
    // Set the parent pointers before the fixUp call
    if(root->left != nullptr) {
        root->left->parent = root;
    }
    if(root->right != nullptr) {
        root->right->parent = root;
    }

    fixUp(root);

    return root;
}

void SpaceSectorLLRBT::fixUp(Sector*& root) {
    if (isRed(root->right) && !isRed(root->left)) {
        root = rotateLeft(root);
    }
    if (isRed(root->left) && isRed(root->left->left)) {
        root = rotateRight(root);
    }
    if (isRed(root->left) && isRed(root->right)) {
        flipColors(root);
    }
}

bool SpaceSectorLLRBT::isRed(Sector* sector) {
    if (sector == nullptr) {
        return false;
    }
    return sector->color == RED;
}

Sector* SpaceSectorLLRBT::rotateLeft(Sector*& sector) {
    Sector* newRoot = sector->right;
    sector->right = newRoot->left;
    if (newRoot->left != nullptr) {
        newRoot->left->parent = sector;
    }
    newRoot->left = sector;
    newRoot->color = sector->color;
    sector->color = RED;
    newRoot->parent = sector->parent;
    sector->parent = newRoot;
    return newRoot;
}

Sector* SpaceSectorLLRBT::rotateRight(Sector*& sector) {
    Sector* newRoot = sector->left;
    sector->left = newRoot->right;
    if (newRoot->right != nullptr) {
        newRoot->right->parent = sector;
    }
    newRoot->right = sector;
    newRoot->color = sector->color;
    sector->color = RED;
    newRoot->parent = sector->parent;
    sector->parent = newRoot;
    return newRoot;
}

void SpaceSectorLLRBT::flipColors(Sector*& sector) {
    sector->color = RED;
    sector->left->color = BLACK;
    sector->right->color = BLACK;
}


void SpaceSectorLLRBT::displaySectorsInOrder() {
    // TODO: Traverse the space sector LLRBT map in-order and print the sectors 
    // to STDOUT in the given format.
    if(root == nullptr){// if the tree is empty, do nothing
        return;
    }
    std::cout << "Space sectors inorder traversal:" << std::endl;
    displaySectorsInOrderHelper(root);// call the helper function
    std::cout << std::endl;
}

void SpaceSectorLLRBT::displaySectorsInOrderHelper(Sector* root){
    if(root == nullptr) { // base case(leaf node)
        return;
    }
    
    displaySectorsInOrderHelper(root->left);
    if(isRed(root)){
        std::cout << "RED";
    }
    else{
        std::cout << "BLACK";
    }
    std::cout << " sector: ";
    std::cout << root->sector_code << std::endl;
    displaySectorsInOrderHelper(root->right);
}

void SpaceSectorLLRBT::displaySectorsPreOrder() {
    // TODO: Traverse the space sector LLRBT map in pre-order traversal and print 
    // the sectors to STDOUT in the given format.
    if(!root){
        return;
    }
    std::cout << "Space sectors preorder traversal:" << std::endl;
    displaySectorsPreOrderHelper(root);
    std::cout << std::endl;
}

void SpaceSectorLLRBT::displaySectorsPreOrderHelper(Sector* root){
    if(root == nullptr) { // base case(leaf node)
        return;
    }
    if(isRed(root)){
        std::cout << "RED";
    }
    else{
        std::cout << "BLACK";
    }
    std::cout << " sector: ";
    std::cout << root->sector_code << std::endl;
    displaySectorsPreOrderHelper(root->left);
    displaySectorsPreOrderHelper(root->right);
}

void SpaceSectorLLRBT::displaySectorsPostOrder() {
    // TODO: Traverse the space sector LLRBT map in post-order traversal and print 
    // the sectors to STDOUT in the given format.
    if(!root){
        return;
    }
    std::cout << "Space sectors postorder traversal:" << std::endl;
    displaySectorsPostOrderHelper(root);
    std::cout << std::endl;
}

void SpaceSectorLLRBT::displaySectorsPostOrderHelper(Sector* root){
    if(root == nullptr) { // base case(leaf node)
        return;
    }
    displaySectorsPostOrderHelper(root->left);
    displaySectorsPostOrderHelper(root->right);
   if(isRed(root)){
        std::cout << "RED";
    }
    else{
        std::cout << "BLACK";
    }
    std::cout << " sector: ";
    std::cout << root->sector_code << std::endl;
}

Sector* SpaceSectorLLRBT::searchSector(Sector* root, std::string sector_code) {
    if (root == nullptr) {
        return nullptr; // Node not found
    }

    std::queue<Sector*> queue;
    queue.push(root);

    while (!queue.empty()) {
        Sector* current = queue.front();
        queue.pop();

        // Process current node
        if (current->sector_code == sector_code) {
            return current;
        }

        // Add left child to queue
        if (current->left != nullptr) {
            queue.push(current->left);
        }

        // Add right child to queue
        if (current->right != nullptr) {
            queue.push(current->right);
        }
    }
    return nullptr; // Node not found
}

std::vector<Sector*> SpaceSectorLLRBT::getStellarPath(const std::string& sector_code) {
    std::vector<Sector*> path;
    // find the sector that matches the sector_code in the BST
    Sector* sector = searchSector(root, sector_code);
    Sector* earth = searchSector(root, "0SSS");
    if(sector == nullptr || earth == nullptr) {
        // if either sector is nullptr, there is no node in the tree
        return path;
    }
    // If the destination sector is the parent of the "0SSS" sector, add both to the path
    if (earth->parent == sector) {
        path.push_back(earth);
        path.push_back(sector);
        return path;
    }
    // construct the path from the sector to the common ancestor
    std::vector<Sector*> pathToCommonAncestor1;
    while (sector != nullptr) {
        pathToCommonAncestor1.push_back(sector);
        if (std::find(pathToCommonAncestor1.begin(), pathToCommonAncestor1.end(), earth) != pathToCommonAncestor1.end()) {
            break;
        }
        sector = sector->parent;
    }

    // construct the path from "0SSS"(the earth) to the common ancestor
    std::vector<Sector*> pathToCommonAncestor2;
    while (earth != nullptr) {
        pathToCommonAncestor2.push_back(earth);
        earth = earth->parent;
    }

    // find the common ancestor of the sector and the earth
    Sector* commonAncestor = nullptr;
    for (int i = 0; i < pathToCommonAncestor1.size(); i++) {
        for (int j = 0; j < pathToCommonAncestor2.size(); j++) {
            if (pathToCommonAncestor1[i] == pathToCommonAncestor2[j]) {
                commonAncestor = pathToCommonAncestor1[i];
                i = pathToCommonAncestor1.size(); // break the outer loop
                break;
            }
        }
    }

    // reverse the commonAncestor1
    std::reverse(pathToCommonAncestor1.begin(), pathToCommonAncestor1.end());

    // find the index of the common ancestor in the pathToCommonAncestor1
    int index1 = 0;
    for (int i = 0; i < pathToCommonAncestor1.size(); i++) {
        if (pathToCommonAncestor1[i] == commonAncestor) {
            index1 = i;
            break;
        }
    }
    // find the index of the common ancestor in the pathToCommonAncestor2
    int index2 = 0;
    for (int i = 0; i < pathToCommonAncestor2.size(); i++) {
        if (pathToCommonAncestor2[i] == commonAncestor) {
            index2 = i;
            break;
        }
    }


    // add the path2 (from earth to the common ancestor) to the path
    for (int i = 0; i < index2 + 1; i++) {
        path.push_back(pathToCommonAncestor2[i]);
    }

    // add the path from the common ancestor to the sector to the path
    for (int i = index1; i < pathToCommonAncestor1.size(); i++) {
        path.push_back(pathToCommonAncestor1[i]);
    }

    // remove duplicates from the path
    auto end = std::unique(path.begin(), path.end());
    path.erase(end, path.end());
    return path;
}

void SpaceSectorLLRBT::printStellarPath(const std::vector<Sector*>& path) {
    // TODO: Print the stellar path obtained from the getStellarPath() function 
    // to STDOUT in the given format.

    // if path is empty, there is no path
    if(path.empty()) {
        std::cout << "A path to Dr. Elara could not be found." << std::endl;
        return;
    }
    // if path is not empty, print the path
    std::cout << "The stellar path to Dr. Elara: ";
    for(int i = 0; i < path.size(); i++) {
        std::cout << path[i]->sector_code;
        if(i != path.size() - 1) {// do not print "->" for the last element
            std::cout << "->";
        }
    }
    std::cout << std::endl;
}

std::vector<std::string> SpaceSectorLLRBT::split(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::istringstream ss(str);
    std::string token;
    while (std::getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

void SpaceSectorLLRBT::deleteWholeTree(Sector* root) {
    if(root == nullptr) {// if root is nullptr, there is no node in the tree
        return;
    }
    deleteWholeTree(root->left);
    deleteWholeTree(root->right);
    delete root;
}

void SpaceSectorLLRBT::displayTreeStructure(const Sector* node, int level) const {
    if (node == nullptr) {
        return;
    }

    displayTreeStructure(node->right, level + 2);

    for (int i = 0; i < level; ++i) {
        std::cout << "    "; // Adjust the number of spaces based on the level for better visualization
    }

    std::cout << node->sector_code << std::endl;

    displayTreeStructure(node->left, level + 2);
}
