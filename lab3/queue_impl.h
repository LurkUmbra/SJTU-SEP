#ifndef QUEUE_IMPL_H
#define QUEUE_IMPL_H
#include <cassert>
#include <cstddef>

template <typename T>
Queue<T>::Queue() : sz(0), head(nullptr), tail(nullptr) { }

template <typename T>
Queue<T>::~Queue() {
    while (head != nullptr) {
        auto nxt = head->next;
        delete head;
        head = nxt;
    }
    tail = nullptr;
    sz = 0;
}

template <typename T>
void Queue<T>::push(T t) {
    auto n = new Node<T>(t);
    if (!empty()) {
        tail->next = n;
    } else {
        head = n;
    }
    tail = n;
    sz++;
}

template <typename T>
void Queue<T>::pop() {
    assert(!empty());
    auto nxt = head->next;
    delete head;
    head = nxt;
    sz--;
}

template <typename T>
T &Queue<T>::front() {
    assert(!empty());
    return head->val;
}

template <typename T>
bool Queue<T>::empty() const {
    return size() == 0;
}

template <typename T>
size_t Queue<T>::size() const {
    return size_t(sz);
}
#endif
