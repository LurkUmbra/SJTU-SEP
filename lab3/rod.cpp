#include "rod.h"
#include <cstddef>

/**
 * @param id starts from 0
 */
Rod::Rod(const int capacity, const int id)
    : capacity(capacity), id(id) {};

bool Rod::push(const Disk d) {
    if (!full()) {
        stack.push(d);
        return true;
    }
    return false;
}

const Disk &Rod::top() {
    return stack.top();
}

void Rod::pop() {
    stack.pop();
}

size_t Rod::size() const {
    return stack.size();
}

bool Rod::empty() const {
    return stack.empty();
}
bool Rod::full() const {
    return int(stack.size()) == capacity;
}
void Rod::draw(Canvas &canvas) {
    const int s_x = 5 + (id * 15);
    int n = int(size());
    Disk* tmp = new Disk[n];

    // draw disks, and in the middle is '|' 
    for (int i = n - 1; i >= 0; i--) {
        tmp[i] = stack.top();
        stack.top().draw(canvas, i, id);
        stack.pop();
        canvas.buffer[Canvas::HEIGHT - 2 * i - 1][s_x] = '|';   
    }
    // draw '|' above disks
    for (int i = 0; i < Canvas::HEIGHT - 2 * n; i++) {
        canvas.buffer[i][s_x] = '|';
    }
    for (int i = 0; i < n; i++) {
        stack.push(tmp[i]);
    }
    delete[] tmp;
}
