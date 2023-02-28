//
// Created by sayem on 2/20/23.
//

#ifndef PARALLEL_SEARCH_COMMON_H
#define PARALLEL_SEARCH_COMMON_H

#include "chrono"

struct Node {
    int data;
    Node *next;
};
Node *head = nullptr;

void insertEnd(int x) {
    Node *ptr = new Node();
    ptr->data = x;
    if (head == nullptr)
        head = ptr;
    else {
        Node *temp = head;
        while (temp->next != nullptr) {
            temp = temp->next;
        }
        temp->next = ptr;
        ptr->next = nullptr;
    }
}

auto start_tick() {
    return std::chrono::high_resolution_clock::now();
}

void end_tick(std::chrono::time_point<std::chrono::system_clock> start_time) {
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
    std::cout << "Execution time: " << std::fixed << std::setprecision(3) << duration.count() << " us or "
              << std::fixed << std::setprecision(3) << duration.count() / 1000 << "ms" << std::endl;

}


#endif //PARALLEL_SEARCH_COMMON_H
