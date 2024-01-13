#include "KD_Tree.h"

// Default constructor implementation
KD_Tree::KD_Tree() : root(nullptr), split_threshold(0.1) {
}

// Parameterized constructor implementation
KD_Tree::KD_Tree(double threshold) : root(nullptr), split_threshold(threshold) {
}

// Destructor implementation
KD_Tree::~KD_Tree() {
    // Implementation for safely deleting the KD_Tree and its nodes
}

KDTreeNode* KD_Tree::buildHelper(std::vector<Point>& points, int depth = 0) {
    if (points.empty()) {
        return nullptr;
    }

    int dim = depth % points[0].features.size();
    sortPoints(points, dim);

    int median = points.size() / 2;

    
    
}

void KD_Tree::build(Dataset& data) {
    // Implementation for building the KD_Tree using the provided dataset
    std::vector<Point> points = data.points;

    if(points.empty()) {
        return;
    }

    // sort the points based on the first dimension
    sortPoints(points, 0);


}


void KD_Tree::sortPoints(std::vector<Point>& points, int dim) {
    std::sort(points.begin(), points.end(), [dim](const Point& a, const Point& b) {
        return a.features[dim] < b.features[dim];
    });
}


KDTreeNode* KD_Tree::getRoot() {
    // Implementation to return the root of the KD_Tree
}

// Add any additional function as you wish
