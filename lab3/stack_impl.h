#ifndef STACK_IMPL_H
#define STACK_IMPL_H
#include <cassert>
#include <cstddef>

template <typename T>
Stack<T>::Stack() : sz(0), head(nullptr) { }

template <typename T>
Stack<T>::~Stack() {
    auto cur = head;
    while (cur != nullptr) {
        auto nxt = cur->next;
        delete cur;
        cur = nxt;
    }
    head = nullptr;
    sz = 0;
}

template <typename T>
void Stack<T>::push(T t) {
    auto cur = head;
    while (cur->next != nullptr) cur = cur->next;
    cur->next = new Node(t, nullptr);
    sz++;
}

template <typename T>
void Stack<T>::pop() {
    assert(!empty());
    auto cur = head;
    while (cur->next->next != nullptr) cur = cur->next;
    delete cur->next;
    cur->next = nullptr;
    sz--;
}

template <typename T>
T& Stack<T>::top() {
    assert(!empty())
    auto cur = head;
    while (cur->next != nullptr) cur = cur->next;
    return cur->val;
}

template <typename T>
bool Stack<T>::empty() const {
    return size() == 0;
}

template <typename T>
size_t Stack<T>::size() const {
    return sz;
}

#endif
