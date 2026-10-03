#ifndef STACK_IMPL_H
#define STACK_IMPL_H
#include <cassert>
#include <cstddef>

template <typename T>
Stack<T>::Stack() : sz(0), head(nullptr) { }

template <typename T>
Stack<T>::~Stack() {
    while (head) {
        auto nxt = head->next;
        delete head;
        head = nxt;
    }
    sz = 0;
}

template <typename T>
void Stack<T>::push(T t) {
    auto n = new Node<T>(t);
    n->next = head;
    head = n;
    sz++;
}

template <typename T>
void Stack<T>::pop() {
    assert(!empty());
    auto nxt = head->next;
    delete head;
    head = nxt;
    sz--;
}

template <typename T>
T& Stack<T>::top() {
    assert(!empty());
    return head->val;
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
