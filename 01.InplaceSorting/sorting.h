#ifndef SORTING_H
#define SORTING_H

#include <algorithm>
#include <iterator>
#include "collvalue.h"

// Сортировка пузырьком — O(N^2)
template <typename Iterator>
void bubble_sort(Iterator begin, Iterator end)
{
    if (begin == end) return;
    for (auto i = begin; i != std::prev(end); ++i) {
        for (auto j = begin; j != std::prev(end, std::distance(begin, i) + 1); ++j) {
            if (*std::next(j) < *j) {
                std::swap(*j, *std::next(j));
            }
        }
    }
}

// Быстрая сортировка (QuickSort) — O(N log N)
template <typename Iterator>
void quick_sort(Iterator begin, Iterator end)
{
    auto size = std::distance(begin, end);
    if (size <= 1) return;

    auto pivot = *std::prev(end); // Выбор опорного элемента
    auto i = begin;
    for (auto j = begin; j != std::prev(end); ++j) {
        if (*j < pivot) {
            std::swap(*i, *j);
            ++i;
        }
    }
    std::swap(*i, *std::prev(end));

    quick_sort(begin, i);
    quick_sort(std::next(i), end);
}

#endif
