#include<iostream>
#include <chrono>
#include <iomanip>
#include "common.h"

int search(int key){
    auto start_time = std::chrono::high_resolution_clock::now();
    Node *temp = head;
    while(temp != NULL){
        if(temp->data == key){
            auto end_time = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
            std::cout << "Execution time: " << std::fixed << std::setprecision(2) << duration.count() << " ms\n";
            return temp->data;
        }
        else temp = temp->next;
    }

    return -1;
}

int main(int argc, char **argv) {
    long long n;std::cin>>n;
    for(long long i=0;i<n;i++){
        //int a; std::cin>>a;
        insertEnd(i);
    }

    std::cout<<"Enter Key: ";
    int key; std::cin>>key;


    int x = search(key);

    if(x==-1) std::cout<<"NotFound"<<std::endl;
    else std::cout<<"Found"<<std::endl;
    return 0;
}
