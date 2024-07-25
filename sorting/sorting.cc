#include <vector>
#include <iostream>

struct SortingMetrics
{
    int comparisons;
    int swaps;

    SortingMetrics() : comparisons(0), swaps(0) {}

    void print_stats()
    {
        std::cout << "Comparisons: " << comparisons << std::endl;
        std::cout << "Swaps: " << swaps << std::endl;
    }
};

void swap_idx(std::vector<int>& arr, int idx1, int idx2)
{
    int temp = arr[idx1];
    arr[idx1] = arr[idx2];
    arr[idx2] = temp;
}

template<typename T>
class SortingAlgorithm
{
    public:
        void bubble_sort(std::vector<T>& arr, SortingMetrics& metrics);
        void selection_sort(std::vector<T>& arr, SortingMetrics& metrics);
        std::pair<int, int> merge_sort(std::vector<T>& arr, SortingMetrics& metrics, int left_boundary, int right_boundary);
};

/// @brief Sorts using bubble sort alg
/// The algorithm goes left to right, and sorts things with right most first, and then eventually
//  the left most one.
/// @tparam T any type that implements the comparable trait
/// @param arr  Array to be sorted
template<typename T>
void SortingAlgorithm<T>::bubble_sort(std::vector<T>& arr, SortingMetrics& metrics)
{
    for (size_t sorted_till = arr.size(); sorted_till > 0; sorted_till--)
    {
        for (size_t i = 0; i < sorted_till - 1; i++)
        {
            T current_item = arr[i];
            T next_item = arr[i+1];

            metrics.comparisons++;
            if (current_item > next_item)
            {
                metrics.swaps++;
                swap_idx(arr, i, i+1);
            }
        }
    }
}

/// @brief Sort via selection sort algorithm
/// Goes from left to right, selecting the smallest element and inserting from left_most spot to right_most spot
/// @tparam T type that suppports comparison
/// @param arr array to be sorted
/// @param metrics metric struct to keep track of comparisons and swaps
template<typename T>
void SortingAlgorithm<T>::selection_sort(std::vector<T>& arr, SortingMetrics& metrics)
{
    for (size_t spot = 0; spot < arr.size(); spot++)
    {
        T min_found = arr[spot];
        size_t min_idx = spot;
        // find the min from spot till end of array
        for (size_t i = spot; i < arr.size(); i++)
        {
            T candidate = arr[i];
            metrics.comparisons++;
            if (candidate < min_found)
            {
                min_found = candidate;
                min_idx = i;
            }
        }
        metrics.swaps++;
        swap_idx(arr, min_idx, spot);
    }
}

// TODO: Fix this shitty code
/// @brief Use merge sort algorithm to sort given arr
/// @tparam T Type that  supports comparison
/// @param arr array to sort
/// @param metrics metrics to track shit
/// @param left_boundary left inclusive idx of subarray
/// @param right_boundary right exclusive idx for subarray
/// @paragraph Divide the array in halfsies till you have onesies, then merge them together going up recursion stack
template <typename T>
std::pair<int, int> SortingAlgorithm<T>::merge_sort(std::vector<T> &arr, SortingMetrics &metrics, int left_boundary, int right_boundary)
{
    if (1 == right_boundary - left_boundary)
    {
        return { left_boundary, right_boundary };
    }

    int split_at = (right_boundary + left_boundary) / 2;
    std::pair<int, int> left_sorted = this->merge_sort(arr, metrics, left_boundary, split_at);
    std::pair<int, int> right_sorted = this->merge_sort(arr, metrics, split_at, right_boundary);

    // merge sorted subarray left_sorted, and right_sorted
    size_t left_sorted_subarr_ctr = left_sorted.first;
    size_t right_sorted_subarr_ctr = right_sorted.first;

    // ew heap memory allocation
    std::vector<T> temp;
    while (left_sorted_subarr_ctr < left_sorted.second || right_sorted_subarr_ctr < right_sorted.second)
    {
        if (left_sorted_subarr_ctr != SIZE_MAX && right_sorted_subarr_ctr != SIZE_MAX)
        {
            if (arr[left_sorted_subarr_ctr] < arr[right_sorted_subarr_ctr])
            {
                temp.push_back(arr[left_sorted_subarr_ctr]);
                left_sorted_subarr_ctr++;
            }
            else
            {
                temp.push_back(arr[right_sorted_subarr_ctr]);
                right_sorted_subarr_ctr++;
            }
        }
        else if (left_sorted_subarr_ctr != SIZE_MAX && right_sorted_subarr_ctr == SIZE_MAX)
        {
            // always choose left
            temp.push_back(arr[left_sorted_subarr_ctr]);
            left_sorted_subarr_ctr++;
        }
        else if (right_sorted_subarr_ctr != SIZE_MAX && left_sorted_subarr_ctr == SIZE_MAX)
        {
            // always choose right 
            temp.push_back(arr[right_sorted_subarr_ctr]);
            right_sorted_subarr_ctr++;
        }

        if (right_sorted_subarr_ctr != SIZE_MAX && right_sorted_subarr_ctr > left_sorted.second)
        {
            right_sorted_subarr_ctr = SIZE_MAX;
        }

        if (left_sorted_subarr_ctr != SIZE_MAX && left_sorted_subarr_ctr > right_sorted.second)
        {
            left_sorted_subarr_ctr = SIZE_MAX;
        }
        metrics.comparisons++;
    }

    // now take the heap memory allocation and insert it into the subarray that left and right guys come together
    for (int i = 0; i < temp.size(); i ++)
    {
        metrics.swaps++;
        arr[left_boundary + i] = temp[i];
    }

    return { left_boundary, right_boundary };
}

// Quick Sort

// Counting Sort

// Radix Sort

// Tim Sort

// Bucket Sort

// utils
void pretty_print_arr(std::vector<int>& arr)
{
    std::cout << "*****Pretty Print Start*****" << std::endl;
    for (int element : arr)
    {
        std::cout << element << std::endl;
    }
    std::cout << "*****Pretty Print End*****" << std::endl;
}

int main(int argc, char const *argv[])
{
    SortingAlgorithm<int> int_sorter;

    // Show performance of each sorting algorithm, counting comparisons and swaps
    std::vector<int> arr { 5, 6, 7, 3, 4, 9, 10, 23, 43, 23 };

    pretty_print_arr(arr);

    std::cout << "Bubble Sort" << std::endl;

    SortingMetrics bubble_sort_metrics;
    auto copy_arr = arr;
    int_sorter.bubble_sort(copy_arr, bubble_sort_metrics);
    bubble_sort_metrics.print_stats();
    pretty_print_arr(copy_arr);

    std::cout << "Selection Sort" << std::endl;

    SortingMetrics selection_sort_metrics;
    copy_arr = arr;
    int_sorter.selection_sort(copy_arr, selection_sort_metrics);
    selection_sort_metrics.print_stats();
    pretty_print_arr(copy_arr);

    std::cout << "Merge Sort" << std::endl;

    SortingMetrics merge_sort_metrics;
    copy_arr = arr;
    int_sorter.merge_sort(copy_arr, merge_sort_metrics, 0, arr.size());
    merge_sort_metrics.print_stats();
    pretty_print_arr(copy_arr);
    return 0;
}
