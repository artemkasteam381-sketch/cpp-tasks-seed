#include <gtest/gtest.h>
#include <vector>
#include <algorithm>
#include "sorting.h"

TEST(SortingTest, BubbleSortCorrectness)
{
    std::vector<int> data = {5, 3, 8, 4, 1, 2, 9, 7, 6};
    bubble_sort(data.begin(), data.end());
    EXPECT_TRUE(std::is_sorted(data.begin(), data.end()));
}

TEST(SortingTest, QuickSortCorrectness)
{
    std::vector<int> data = {5, 3, 8, 4, 1, 2, 9, 7, 6};
    quick_sort(data.begin(), data.end());
    EXPECT_TRUE(std::is_sorted(data.begin(), data.end()));
}

TEST(SortingTest, HandlesEmptyRange)
{
    std::vector<int> data;
    EXPECT_NO_THROW(bubble_sort(data.begin(), data.end()));
    EXPECT_NO_THROW(quick_sort(data.begin(), data.end()));
}
