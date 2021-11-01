#ifndef VE281P1_SORT_HPP
#define VE281P1_SORT_HPP

#include <vector>
#include <stdlib.h>
#include <functional>
#include<iostream>
using namespace std;

template<typename T, typename Compare>
void bubble_sort(std::vector<T> &vector, Compare comp = std::less<T>()) {
    // TODO: implement
    size_t size = vector.size();
    for (size_t i = size; i > 0 ; i--) {
        size_t current = i - 1;
        for (size_t j = current + 1; j < size ; j++) {
            if (comp(vector[j], vector[current])) {
                T temp = vector[j];
                vector[j] = vector[current];
                vector[current] = temp;
                current = j;
            }
            else {
                break;
            }
        }
    }
}

template<typename T, typename Compare>
size_t bin_search (T i, std::vector<T> &arr , size_t left , size_t right, Compare comp = std::less<T>()) {
    if(left >= right) {
        return left;
    }
    else {
        if (comp(arr[(left + right)/2], i)) {
           return bin_search(i, arr, (left + right)/2 + 1, right, comp);
        }
        else {
            return bin_search(i, arr, left, (left + right)/2, comp);
        }
    }
}


template<typename T, typename Compare>
void insertion_sort(std::vector<T> &vector, Compare comp = std::less<T>()) {
    // TODO: implement
    
    size_t size = vector.size();
    for (size_t i = 1; i < size ; i++) {
        T victim = vector[i];
        size_t pos = bin_search(victim , vector, 0, i-1, comp);

        //if the bucket is smaller than the current number
        if (comp(victim, vector[pos])) {
            for(size_t j = i ; j >= pos + 1 ; j--) {
                vector[j] = vector [j - 1];
            }
            vector[pos] = victim;
        }
        else {
            for(size_t j = i ; j > pos + 1 ; j--) {
                vector[j] = vector [j - 1] ;
            }
            vector[pos + 1] = victim;
        }
    }
}

template<typename T, typename Compare>
void selection_sort(std::vector<T> &vector, Compare comp = std::less<T>()) {
    // TODO: implement

    size_t size = vector.size();
    for(size_t i = 0 ; i < size - 1 ; i++) {
        T min = vector[i];
        size_t min_tag = i;
        for (size_t j = i ; j < size ; j++) {
            if(comp(vector[j],min)) {
                min = vector[j];
                min_tag = j;
            }
        }
        T temp = vector[i];
        vector[i] = vector[min_tag];
        vector[min_tag] = temp;
    }
}

template<typename T, typename Compare>
void merge_sort(std::vector<T> &vector, Compare comp = std::less<T>()) {
    // TODO: implement

    size_t size = vector.size();
    if (size == 1) {
        return;
    }
    else {
        std::vector<T> left;
        std::vector<T> right;
        for (size_t i = 0; i < size/2 ; i++) {
            left.push_back(vector[i]);
        }
        for (size_t j = size/2 ; j < size ; j++) {
            right.push_back(vector[j]);
        }
        merge_sort(left, comp);
        merge_sort(right, comp);  
        size_t left_ptr = 0;
        size_t right_ptr = 0;
        size_t overall_ptr = 0;
        size_t l_size = left.size();
        size_t r_size = right.size();
        while (left_ptr < l_size && right_ptr < r_size) {
            if(comp(right[right_ptr], left[left_ptr])) {
                vector[overall_ptr] = right[right_ptr];
                right_ptr++;
                overall_ptr++;
            }
            else {
                vector[overall_ptr] = left[left_ptr];
                left_ptr++;
                overall_ptr++;
            }
        }
        if(left_ptr == l_size) {
            while(right_ptr < r_size) {
                vector[overall_ptr] = right[right_ptr];
                right_ptr++;
                overall_ptr++;
            }
        }
        else {
            while(left_ptr < l_size) {
                vector[overall_ptr] = left[left_ptr];
                left_ptr++;
                overall_ptr++;
            }
        }
        return;
    }
}

//input a vector and the sorting region, sort it, return the final place of pivot
template<typename T, typename Compare>
void quick_sort_helper_1(std::vector<T> &vector, size_t left, size_t right, Compare comp = std::less<T>()) {
    if(left >= right) {
        return;
    }
    else {
        T pivot = vector[left];
        size_t left_top = 0;
        size_t right_top = right - left;
        std::vector<T> result(right - left + 1,0);
        for (size_t i = left + 1; i <= right; i++) {
            if(comp(vector[i],pivot)) {
                result[left_top] = vector[i];
                left_top++;
            }
            else {
                result[right_top] = vector[i];
                right_top--;
            }
        }
        result[left_top] = pivot;
        quick_sort_helper_1(result, left, left_top-1, comp);
        quick_sort_helper_1(result, left_top + 1, right, comp);
        for (size_t j = left; j <= right; j++) {
            vector[j] = result[j - left];
        }
        return;
    }
}


template<typename T, typename Compare>
void quick_sort_extra(std::vector<T> &vector, Compare comp = std::less<T>()) {
    // TODO: implement
        quick_sort_helper_1(vector, 0, vector.size() - 1, comp);
    }


template<typename T, typename Compare>
void quick_sort_helper_2(std::vector<T> &vector, size_t left, size_t right, Compare comp = std::less<T>()) {
    size_t pivotat = 0;
    if(left >= right) {
        return;
    }
    else {
        size_t i = left;
        size_t j = right;
        while(true) {
            while(true) {
                if(i > j || comp(vector[left], vector[i])) {
                    break;
                }
                else {
                    i++;
                }
            }
            while(true) {
                if(j < i || comp(vector[j], vector[left])) {
                    break;
                }
                else {
                    j--;
                }
            }
            if(i < j) {
                T temp = vector[i];
                vector[i] = vector[j];
                vector[j] = temp;
            }
            else {
                T temp = vector[left];
                vector[left] = vector[j];
                vector[j] = temp;
                pivotat = j;
                break;
            }
        }
        cout<<"pivot at: "<<pivotat<<endl;
        for(size_t i = left ; i <= right; i++) {
            cout<<vector[i] <<" ";
        }
        cout<<endl;
    quick_sort_helper_2(vector,left, pivotat-1,comp);
    quick_sort_helper_2(vector,pivotat+1,right,comp);      
    return;  
    }
}

template<typename T, typename Compare>
void quick_sort_inplace(std::vector<T> &vector, Compare comp = std::less<T>()) {
    // TODO: implement

    quick_sort_helper_2(vector, 0, vector.size() - 1, comp);
}
#endif //VE281P1_SORT_HPP