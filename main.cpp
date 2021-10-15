#include<iostream>
#include<vector>
#include<algorithm>
#include"sort.hpp"
using namespace std;

int main() {
    std::vector<int> a;
    a.push_back(3);
    a.push_back(2);
    a.push_back(4);
    a.push_back(1);
    a.push_back(2);
    std::vector<int> b = a;
    std::vector<int> c = a;
    std::vector<int> d = a;
    std::vector<int> e = a;    
    std::vector<int> f = a;
    std::less<int> les;


    //bubble sort check
    bubble_sort(a,les);
    for(int i = 0 ; i < 5; i++) {
        cout<<a[i]<<endl;
    }
    cout<<"----"<<endl;



    //insertion sort check
    insertion_sort(b,les);
    for(int i = 0 ; i < 5; i++) {
        cout<<b[i]<<endl;
    }
    cout<<"----"<<endl;    

    //selection sort check
    selection_sort(c,les);
    for(int i = 0 ; i < 5; i++) {
        cout<<c[i]<<endl;
    }
    cout<<"----"<<endl; 

    //merge sort check
    merge_sort(d,les);
    for(int i = 0 ; i < 5; i++) {
        cout<<d[i]<<endl;
    }
    cout<<"----"<<endl;    

    //quick sort extra check
    quick_sort_extra(e,les);
    for(int i = 0 ; i < 5; i++) {
        cout<<e[i]<<endl;
    }
    cout<<"----"<<endl;   

    //quick sort inplace check
    quick_sort_inplace(f,les);
    for(int i = 0 ; i < 5; i++) {
        cout<<f[i]<<endl;
    }
    cout<<"----"<<endl;       
    return 0;
}