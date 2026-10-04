#ifndef C_BINARYDIMEN_TREE_H
#define C_BINARYDIMEN_TREE_H

#include <stdio.h>
#include <iostream>
#include <vector>

/****************************************************************
 *                    Write your code below
 ****************************************************************/

struct Point{
    long long x;
    long long y;
};

class TreeNode
{
  friend std::ostream &operator<<(std::ostream &out, const TreeNode &b);
  friend class BinaryTree;
  friend class BinaryDimonTree;

private:
  /* data */
  long long data[2];
  TreeNode* left;
  TreeNode* right;
  long long level;
public:
  /* methods */
  TreeNode();
  long long getX() const;  /* DO NOT CHANGE */
  long long getY() const;  /* DO NOT CHANGE */
  ~TreeNode(); /* DO NOT CHANGE */
  static void destroyTree(TreeNode* node) {
    if (!node) return;
    destroyTree(node->left);
    destroyTree(node->right);
    delete node;
  }
};


class BinaryDimonTree
{
friend std::istream &operator>>(std::istream &in, BinaryDimonTree &tree); /* DO NOT CHANGE */

private:
  /* data */
  TreeNode *root;

public:
  /* methods */
  BinaryDimonTree();          /* DO NOT CHANGE */
  TreeNode *find_nearest_node(long long x, long long y);  /* DO NOT CHANGE */

  void recur_search(TreeNode *cur, long long x, long long y, long long int &min_distance, TreeNode **guess);
  ~BinaryDimonTree();
  void buildBDTree(std::vector<Point>& points);
  TreeNode* buildHelper(std::vector<Point>& points, long long lo, long long hi, int level);
};

#endif //C_BINARYDIMEN_TREE_H
