#include <vector>
#include <iostream>

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

    // fuck heap memory allocation, i will die on the hill that I shouldn't need to allocate a fucking vector
    merge(arr, left_sorted, right_sorted, metrics);

    return { left_boundary, right_boundary };
}

const inline bool right_ended(int right_ctr, std::pair<int, int>& sorted_right_subarr) 
{
    return right_ctr >= sorted_right_subarr.second;
}

const inline std::pair<bool, int> left_ended(int left_ctr, std::pair<int, int>& sorted_left_subarr, int temp_redirection)
{
    if (temp_redirection != -1)
        return { false, temp_redirection }; 

    return { left_ctr >= sorted_left_subarr.second , -1 };
}

template<typename T>
const void print_sub_arr_(std::vector<T>& arr, std::pair<int, int>& subarr)
{
    for (size_t i = subarr.first; i < subarr.second; i++)
        std::cout << arr[i] << ", ";

    std::cout << "\n";
}

template<typename T>
void merge(std::vector<T>& arr, std::pair<int, int>& sorted_left_subarr, std::pair<int, int>& sorted_right_subarr, SortingMetrics& metrics)
{
    int left_ctr = sorted_left_subarr.first;
    int right_ctr = sorted_right_subarr.first;
    int temp_redirection = -1;

    // inlined function call for loop termination
    std::pair<bool, int> has_left_ended_res = left_ended(left_ctr, sorted_left_subarr, temp_redirection);
    bool has_right_ended = right_ended(right_ctr, sorted_right_subarr);

    // we only care when both counters are in their bound, this is because if the left or the right end
    // then due to the contiguous nature of the underlying array, they will already be in sorted sequence
    while (!has_left_ended_res.first && !has_right_ended)
    {
        int redirection_layer = has_left_ended_res.second;
        bool use_redirection_layer = redirection_layer != -1;

        // if we are working with indirection layer then this is the logical replacement of the left_ctr
        if (use_redirection_layer)
        {
            metrics.comparisons++;
            if (redirection_layer > arr[right_ctr])
            {
                metrics.swaps++;
                arr[left_ctr] = arr[right_ctr];
                right_ctr++;
            }
            else
            {
                metrics.swaps++;
                arr[left_ctr] = redirection_layer;
                temp_redirection = -1; // we dont need to do first link pattern anymore
                // after this we should go back to using the left_ctr on the next iteration for tracking the left subarr
            }
        } 
        else 
        {
            // if we are not using indirection layer, then we are working with the left_ctr, and right_ctr
            // we only need to make swaps when the value at left_ctr is greater than the value at the right_ctr
            // otherwise they are already in appropriate order
            metrics.comparisons++;
            if (arr[left_ctr] > arr[right_ctr])
            {
                metrics.swaps++;
                temp_redirection = arr[left_ctr];
                arr[left_ctr] = arr[right_ctr];
                right_ctr++;
            }
        }
        left_ctr++;

        has_left_ended_res = left_ended(left_ctr, sorted_left_subarr, temp_redirection);
        has_right_ended = right_ended(right_ctr, sorted_right_subarr);
    }
    // if at the end we have temp_redirection as no -1, we have one last fuckery to do
    if (!has_left_ended_res.first && has_left_ended_res.second != -1)
    {
        metrics.swaps++;
        arr[right_ctr-1] = temp_redirection;
        temp_redirection = -1;
    }
}

// Quick Sort

// Counting Sort

// Radix Sort

// Tim Sort

// Bucket Sort

int main(int argc, char const *argv[])
{
    SortingAlgorithm<int> int_sorter;

    // Show performance of each sorting algorithm, counting comparisons and swaps
    std::vector<int> arr { 5, -15, -99, 6, 7, 3, 1738, 4, 9, 10, 23, 43, 23 };

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
