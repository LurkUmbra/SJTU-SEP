#ifndef STACK_H
#define STACK_H

#include "node.h"
#include <cstddef>

template <typename T>
class Stack {
    size_t sz;
    Node<T>* head;

public:
    Stack();
    ~Stack();

    void push(T t);
    void pop();
    T& top();
    [[nodiscard]] bool empty() const;
    [[nodiscard]] size_t size() const;
};

#include "stack_impl.h"
#endif
