#ifndef QUEUE_H
#define QUEUE_H
#include "node.h"
#include <cstddef>

template <typename T>
class Queue {
    int sz;
    Node<T> *head;
    Node<T> *tail;

public:
    Queue();
    ~Queue();

    void push(T t);
    void pop();
    T &front();
    [[nodiscard]] bool empty() const;
    [[nodiscard]] size_t size() const;
};

#include "queue_impl.h"
#endif
