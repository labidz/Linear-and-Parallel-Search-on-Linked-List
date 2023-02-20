#include<iostream>
#include <chrono>
#include <iomanip>
#include <thread>
#include <vector>

int ck = 0, n=0;
struct Node{
    long long data;
    Node *next;
};
Node *head = NULL;

void insertEnd(long long x){
    Node *ptr = new Node();
    ptr->data=x;
    if(head==NULL)
        head=ptr;
    else{
        Node *temp = head;
        while(temp->next!=NULL){
            temp=temp->next;
        }
        temp->next=ptr;
        ptr->next=NULL;
    }
}

void search(Node* start, Node* end, int key, int& result) {
    for (Node* current = start; current != end; current = current->next) {
        if (current->data == key) {
            result = current->data;
            std::cout<<"Found"<<std::endl;
            ck = 1;
            return;
        }
    }
}

void parallelSearch(int key, int num_threads) {
    auto start_time = std::chrono::high_resolution_clock::now();
    int chunk_size = (n + num_threads - 1) / num_threads;
    std::vector<std::thread> threads(num_threads);
    int result = -1;

    // Launch the threads to search for the key in each chunk
    Node* start = head;
    for (int i = 0; i < num_threads; i++) {
        Node* end = start;
        for (int j = 0; j < chunk_size && end != nullptr; j++) {
            end = end->next;
        }
        threads[i] = std::thread(search, start, end, key, std::ref(result));
        start = end;
    }
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
    std::cout << "Execution time: " << std::fixed << std::setprecision(2) << duration.count() << " ms\n";
    // Wait for all threads to finish
    for (int i = 0; i < num_threads; i++) {
        threads[i].join();
    }
    if(ck == 0) std::cout<<"Not Found"<<std::endl;
}

int main(int argc, char **argv) {
    int t;
    std::cin>>n;
    for(long long i=0;i<n-1;i++){
        //int a; std::cin>>a;
        t = rand()%100000;
        insertEnd(t);
    }
    insertEnd(100001);

    int num_threads = std::thread::hardware_concurrency();

    parallelSearch(100001, num_threads);

    return 0;
}
