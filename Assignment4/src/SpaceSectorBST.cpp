#include "SpaceSectorBST.h"

using namespace std;

SpaceSectorBST::SpaceSectorBST() : root(nullptr) {}

SpaceSectorBST::~SpaceSectorBST() {
    // Free any dynamically allocated memory in this class.
    // free the tree structure
    deleteWholeTree(root); // delete the tree
}

void SpaceSectorBST::readSectorsFromFile(const std::string& filename) {
    // TODO: read the sectors from the input file and insert them into the BST sector map
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

void SpaceSectorBST::insertSectorByCoordinates(int x, int y, int z) {
    // Instantiate and insert a new sector into the space sector BST map according to the 
    // coordinates-based comparison criteria.
    Sector* new_sector = new Sector(x, y, z);

    if(!root) {// if root is nullptr (new_sector is the first node)
        root = new_sector;
        root->parent = nullptr;
        return;
    }
    insertSectorByCoordinatesHelper(root, new_sector);
}

void SpaceSectorBST::insertSectorByCoordinatesHelper(Sector*& root, Sector*& new_sector){
    if(root == nullptr) {
        root = new_sector;
        return;
    }

    if(new_sector->x < root->x) {
        if(root->left == nullptr) {
            root->left = new_sector;
            new_sector->parent = root;
        }
        else {
            insertSectorByCoordinatesHelper(root->left, new_sector);
        }
    }
    else if(new_sector->x > root->x) {
        if(root->right == nullptr) {
            root->right = new_sector;
            new_sector->parent = root;
        }
        else {
            insertSectorByCoordinatesHelper(root->right, new_sector);
        }
    }
    else if(new_sector->x == root->x) {
        if(new_sector->y < root->y) {
            if(root->left == nullptr) {
                root->left = new_sector;
                new_sector->parent = root;
            }
            else {
                insertSectorByCoordinatesHelper(root->left, new_sector);
            }
        }
        else if(new_sector->y > root->y) {
            if(root->right == nullptr) {
                root->right = new_sector;
                new_sector->parent = root;
            }
            else {
                insertSectorByCoordinatesHelper(root->right, new_sector);
            }
        }
        else if(new_sector->y == root->y) {
            if(new_sector->z < root->z) {
                if(root->left == nullptr) {
                    root->left = new_sector;
                    new_sector->parent = root;
                }
                else {
                    insertSectorByCoordinatesHelper(root->left, new_sector);
                }
            }
            else if(new_sector->z > root->z) {
                if(root->right == nullptr) {
                    root->right = new_sector;
                    new_sector->parent = root;
                }
                else {
                    insertSectorByCoordinatesHelper(root->right, new_sector);
                }
            }
            else if(new_sector->z == root->z) {
                return;
            }
        }
    }
}

// function to compare two sector codes
bool SpaceSectorBST::isLessThan(const std::string& sector_code1, const std::string& sector_code2) {

    // find the sector that matches the sector_code in the BST (implement a finder function)
    // after that you have the sector corresponding of a sector_code
    // then, here you have x, y and z coordinates of a sector_code

    Sector* sector1 = searchSector(root, sector_code1);
    Sector* sector2 = searchSector(root, sector_code2);

    if(sector1 == nullptr || sector2 == nullptr) {// if sector1 or sector2 is nullptr, there is no node in the tree
        return false;
    }// not found

    // x,y and z coordinates comparison
    if(sector1->x < sector2->x) {
        return true;
    }
    else if (sector1->x > sector2->x) {
        return false;
    }
    else if (sector1->x == sector2->x) {
        if(sector1->y < sector2->y) {
            return true;
        }
        else if (sector1->y > sector2->y) {
            return false;
        }
        else if (sector1->y == sector2->y) {
            if(sector1->z < sector2->z) {
                return true;
            }
            else if (sector1->z > sector2->z) {
                return false;
            }
            else if (sector1->z == sector2->z) {
                return false;
            }
        }
    }
    return false;
}

// private search function using a queue
Sector* SpaceSectorBST::searchSector(Sector* root, std::string sector_code) {
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


void SpaceSectorBST::deleteSector(const std::string& sector_code) {
    // TODO: Delete the sector given by its sector_code from the BST.
    
    if(root == nullptr){// if root is nullptr, there is no node in the tree
        return;
    }
    if(searchSector(root, sector_code) == nullptr){// if sector_code is not found
        return;
    }

    // comparison and deletion
    if(isLessThan(sector_code, root->sector_code)){
        root->left = deleteSectorHelper(root->left, sector_code);
    }
    else if (isLessThan(root->sector_code, sector_code)){
        root->right = deleteSectorHelper(root->right, sector_code);
    }
    else { // root will be deleted
        root = deleteSectorHelper(root, sector_code);
    }
}

Sector* SpaceSectorBST::deleteSectorHelper(Sector* root, const std::string& sector_code){
    if(root == nullptr){// if root is nullptr, there is no node in the tree
        return nullptr;
    }
    if(isLessThan(sector_code, root->sector_code)){
        root->left = deleteSectorHelper(root->left, sector_code);
    }
    else if(isLessThan(root->sector_code, sector_code)){
        root->right = deleteSectorHelper(root->right, sector_code);
    }
    else{// if sector_code == root->sector_code
        if(root->left == nullptr){// if root->left is nullptr
            Sector* temp = root->right;
            delete root;
            return temp;
        }
        else if(root->right == nullptr){// if root->right is nullptr
            Sector* temp = root->left;
            delete root;
            return temp;
        }
        else { // if root has two children
            Sector* temp = findMin(root->right); // root will be the minimum of the right subtree
            // copy the contents of the minimum node to the root
            root->sector_code = temp->sector_code;
            root->x = temp->x;
            root->y = temp->y;
            root->z = temp->z;
            root->distance_from_earth = temp->distance_from_earth;
            // Delete the temp node
            root->right = deleteSectorHelper(root->right, temp->sector_code);
            if (root->right != nullptr) {
            root->right->parent = root; // update the parent pointer of the right child
            }
        }
    }
    return root;
}

// finds the minimum sector code in the right subtree of the bst
Sector* SpaceSectorBST::findMin(Sector* root) {
    if (root == nullptr) {
        return nullptr;
    }
    // keep traversing the left subtree until the leftmost leaf node is reached
    while (root->left != nullptr) {
        root = root->left;
    }
    return root;
}


void SpaceSectorBST::displaySectorsInOrder() {
    // TODO: Traverse the space sector BST map in-order and print the sectors 
    // to STDOUT in the given format.
    if(!root){
        return;
    }
    std::cout << "Space sectors inorder traversal:" << std::endl;
    displaySectorsInOrderHelper(root);
    std::cout << std::endl;
}

void SpaceSectorBST::displaySectorsInOrderHelper(Sector* root) {
    if(root == nullptr) { // base case(leaf node)
        return;
    }
    displaySectorsInOrderHelper(root->left);
    std::cout << root->sector_code << std::endl;
    displaySectorsInOrderHelper(root->right);
}

void SpaceSectorBST::displaySectorsPreOrder() {
    // TODO: Traverse the space sector BST map in pre-order traversal and print 
    // the sectors to STDOUT in the given format.
    if(!root){
        return;
    }
    std::cout << "Space sectors preorder traversal:" << std::endl;
    displaySectorsPreOrderHelper(root);
    std::cout << std::endl;
}

void SpaceSectorBST::displaySectorsPreOrderHelper(Sector* root) {
    if(root == nullptr) { // base case(leaf node)
        return;
    }
    std::cout << root->sector_code << std::endl;
    displaySectorsPreOrderHelper(root->left);
    displaySectorsPreOrderHelper(root->right);
}

void SpaceSectorBST::displaySectorsPostOrder() {
    // TODO: Traverse the space sector BST map in post-order traversal and print 
    // the sectors to STDOUT in the given format.
    if(!root){
        return;
    }
    std::cout << "Space sectors postorder traversal:" << std::endl;
    displaySectorsPostOrderHelper(root);
    std::cout << std::endl;
}

void SpaceSectorBST::displaySectorsPostOrderHelper(Sector* root) {
    if(root == nullptr) { // base case(leaf node)
        return;
    }
    displaySectorsPostOrderHelper(root->left);
    displaySectorsPostOrderHelper(root->right);
    std::cout << root->sector_code << std::endl;
}


std::vector<Sector*> SpaceSectorBST::getStellarPath(const std::string& sector_code) {
    std::vector<Sector*> path;
    // TODO: Find the path from the Earth to the destination sector given by its
    // sector_code, and return a vector of pointers to the Sector nodes that are on
    // the path. Make sure that there are no duplicate Sector nodes in the path!

    // find the sector that matches the sector_code in the BST
    Sector* sector = searchSector(root, sector_code);
    Sector* earth = searchSector(root, "0SSS");
    Sector* current = earth;
    if(sector == nullptr || earth == nullptr) {// if sector is nullptr, there is no node in the tree
        return path;
    }
    if (sector == earth) {// if sector is the Earth
        path.push_back(sector);
        return path;
    }
    else { // the approach is to traverse until the destination sector, simply
        while (current != nullptr) {
            path.push_back(current);
            if (isLessThan(sector->sector_code, current->sector_code)) {
                current = current->left;
            }
            else if (isLessThan(current->sector_code, sector->sector_code)) {
                current = current->right;
            }
            else if (current->sector_code == sector->sector_code) {
                break;
            }
        }
    }
    return path;
}


void SpaceSectorBST::printStellarPath(const std::vector<Sector*>& path) {
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


std::vector<std::string> SpaceSectorBST::split(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::istringstream ss(str);
    std::string token;
    while (std::getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

void SpaceSectorBST::deleteWholeTree(Sector* &root) {
    if(root == nullptr) {
        return;
    }
    deleteWholeTree(root->left);
    deleteWholeTree(root->right);
    delete root;
    root = nullptr;
}

void SpaceSectorBST::displayTreeStructure(const Sector* node, int level) const {
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