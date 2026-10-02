#ifndef QUEUE_IMPL_H
#define QUEUE_IMPL_H
#include <cassert>
#include <cstddef>

template <typename T>
Queue<T>::Queue() : sz(0), head(nullptr), tail(nullptr) { }

template <typename T>
Queue<T>::~Queue() {
    // TODO
}

template <typename T>
void Queue<T>::push(T t) {
    // TODO
}

template <typename T>
void Queue<T>::pop() {
    // TODO
}

template <typename T>
T &Queue<T>::front() {
    // TODO
}

template <typename T>
bool Queue<T>::empty() const {
    // TODO
}

template <typename T>
size_t Queue<T>::size() const {
    // TODO
}
#endif
