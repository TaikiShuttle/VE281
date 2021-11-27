#include<iostream>
#include"kdtree.hpp"
#include<string>
#include<vector>
using namespace std;

int main() {
    // a two dimensional tree with key to be int,int and the value of node to be string
    KDTree<std::tuple<int,int>,string> tree1;

    //first try to test the insertion function
    cout<<"first we try to insert some nodes: "<<endl;
    tree1.insert(std::tuple<int,int>(10,20), "a");
    tree1.insert(std::tuple<int,int>(5,30), "b");
    tree1.insert(std::tuple<int,int>(20,15),"c");
    tree1.insert(std::tuple<int,int>(10,49),"d");
    tree1.insert(std::tuple<int,int>(100,8),"e");

    cout<<"The tree1 is now: "<<endl;
    for (auto &item: tree1) {
        cout<<item.second<<endl;
    }

    //test the vector constructor
    cout<<"now we construct tree2 with vector"<<endl;
    std::vector<std::pair<std::tuple<int,int>,std::string>> v;
    pair a = make_pair(std::tuple<int,int>(10,20),"a");
    v.push_back(a);
    
    KDTree<std::tuple<int,int>,string> tree2(v);
    
    cout<<"Tree2 is now: "<<endl;
    for (auto &item: tree2) {
        cout<<item.second<<endl;
    }

    //then we try to delete a node
    cout<<"The following operations are done to tree1."<<endl;
    cout<<"Then we try to erase the node a(10,20):"<<endl;
    if(tree1.erase(std::tuple<int,int>(10,20))) {
        cout<<"The node a(10,20) successfully erased"<<endl;
    }
    else {
        cout<<"Something wrong happended to erase a(10,20)"<<endl;
    }
    for (auto &item: tree1) {
        cout<<item.second<<endl;
    }

    //test FindMIn and FindMax
    cout<<"Then we test the find min function and find max function:"<<endl;
    cout<<"The smallest in dim 0 is "<<tree1.findMin(0)->second<<endl;
    cout<<"The largest in dim 0 is "<<tree1.findMax(0)->second<<endl;
    cout<<"The smallest in dim 1 is "<<tree1.findMin(1)->second<<endl;
    cout<<"The largest in dim 1 is "<<tree1.findMax(1)->second<<endl;

    return 0;
}