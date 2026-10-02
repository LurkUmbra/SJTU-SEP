#ifndef NODE_H_
#define NODE_H_

template <typename T>
struct Node {
    T val;
    Node* next;

    Node() : next(nullptr) { }

    explicit Node(T t) : val(t), next(nullptr) { }
};

#endif