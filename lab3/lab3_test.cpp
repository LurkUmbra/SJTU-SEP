#include "stack.h"

#include <gtest/gtest.h>

TEST(StackTest, basicUsage) {
    Stack<int> s;
    s.push(1);
    s.push(2);
    EXPECT_EQ(s.top(), 2);
}
