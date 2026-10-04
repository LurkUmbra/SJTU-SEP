#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <limits.h>
#include <vector>
#include <queue>
#include <algorithm>

#include "Tree.h"


/****************************************************************
 *                    Write your code below
 ****************************************************************/
std::ostream &operator<<(std::ostream &out, const TreeNode &b) {
    out << '(' << b.data[0] << ',' << b.data[1] << ')';
    return out;
}

long long TreeNode::getX() const {
    return data[0];
}

long long TreeNode::getY() const {
    return data[1];
}

TreeNode::TreeNode() {
    data[0] = data[1] = level = 0;
    left = right = nullptr;
}

TreeNode::~TreeNode() {
    left = right = nullptr;
    level = 0;
}

TreeNode* BinaryDimonTree::buildHelper(std::vector<Point>& points, long long lo,
                                       long long hi, int level)
{
    if (lo > hi) return nullptr;

    int axis = level % 2;
    std::sort(points.begin() + lo, points.begin() + hi + 1,
              [axis](const Point& a, const Point& b) {
                    return axis == 0 ? a.x < b.x : a.y < b.y;
              });
    long long mid = lo + (hi - lo) / 2;

    TreeNode* node = new TreeNode();
    node->data[0] = points[mid].x;
    node->data[1] = points[mid].y;
    node->level = level;
    node->left = buildHelper(points, lo, mid - 1, level + 1);
    node->right = buildHelper(points, mid + 1, hi, level + 1);

    return node;
}
void BinaryDimonTree::buildBDTree(std::vector<Point>& points)
{
    this->root = buildHelper(points, 0, (long long)points.size() - 1, 0);
}

std::istream &operator>>(std::istream &in, BinaryDimonTree &tree) {
    long long n;
    if (!(in >> n)) return in;

    std::vector<Point> points;
    points.reserve(static_cast<size_t>(n));

    for (long long i = 0; i < n; i++) {
        Point p;
        in >> p.x >> p.y;
        points.push_back(p);
    }

    tree.buildBDTree(points);
    return in;
}

BinaryDimonTree::BinaryDimonTree() {
    root = nullptr;
}

TreeNode *BinaryDimonTree::find_nearest_node(long long x, long long y) {
    if (root == nullptr) return nullptr;

    long long min_distance = LLONG_MAX;
    TreeNode* guess = nullptr;
    
    recur_search(root, x, y, min_distance, &guess);
    return guess;
}

void BinaryDimonTree::recur_search(TreeNode *cur, long long x, long long y, long long &min_distance, TreeNode **guess) {
    if (cur == nullptr) return;

    long long dx = cur->getX() - x;
    long long dy = cur->getY() - y;
    long long dist = dx * dx + dy * dy;

    if (dist < min_distance) {
        min_distance = dist;
        *guess = cur;
    }

    int axis = static_cast<int>(cur->level % 2);
    TreeNode* near = nullptr;
    TreeNode* far = nullptr;
    
    if (axis == 0) {
        if (x < cur->getX()) {
            near = cur->left;
            far = cur->right;
        } else {
            near = cur->right;
            far = cur->left;
        }
    } else {
        if (y < cur->getY()) {
            near = cur->left;
            far = cur->right;
        } else {
            near = cur->right;
            far = cur->left;
        }
    }

    recur_search(near, x, y, min_distance, guess);

    long long plane_dist = (axis == 0) ? dx * dx : dy * dy;
    if (plane_dist < min_distance) {
        recur_search(far, x, y, min_distance, guess);
    }
}

BinaryDimonTree::~BinaryDimonTree()
{
    TreeNode::destroyTree(root);
    root = nullptr;
}
