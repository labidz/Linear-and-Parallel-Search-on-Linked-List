#include<iostream>
#include <iomanip>
#include <thread>
#include <vector>
#include "common.h"
#include "unordered_map"
#include <future>


int ck = 0, n=0;
bool stop_thread = false;

void search(Node *start, Node *end, int key, Node &result) {
    if (stop_thread) return;

    for (Node *current = start; current != end; current = current->next) {
        if (current->data == key) {
            result = *current;
            ck = 1;
            stop_thread = true;
            return;
        }
    }
}



/*
 * Cache
 */

std::unordered_map<int, std::vector<Node *>> chunk_cache;
std::unordered_map<int, Node *> search_cache;



void parallelSearch(int key, unsigned int num_threads) {
    auto start_time = start_tick();
    stop_thread = false;
    ck = 0;
    auto c_result = search_cache.find(key);
    if (c_result != search_cache.end()) {
        ck = 1;
        std::cout << "Found a match in cache:: k: " << c_result->first << std::endl;
        end_tick(start_time);
        return;
    }

    auto chunk_size = (n + num_threads - 1) / num_threads;
    std::vector<std::future<void>> futures;
    std::vector<Node> results(num_threads);

    // Launch the threads to search for the key in each chunk
    Node* start = head;
    for (int i = 0; i < num_threads; i++) {
        auto c_ptr = chunk_cache.find(i);
        if (c_ptr != chunk_cache.end()) {
            futures.emplace_back(std::async(std::launch::async, search, c_ptr->second[0], c_ptr->second[1], key, std::ref(results[i])));
        } else {
            Node* Nend = start;
            for (int j = 0; j < chunk_size && Nend != nullptr; j++) {
                Nend = Nend->next;
            }
            futures.emplace_back(std::async(std::launch::async, search, start, Nend, key, std::ref(results[i])));
            // Cache chunk boundaries
            std::vector<Node*> tmp;
            tmp.push_back(start);
            tmp.push_back(Nend);
            chunk_cache.insert(std::make_pair(i, tmp));
            start = Nend;
        }
    }

    // Wait for all threads to finish
    for (auto& future : futures) {
        future.wait();
    }

    // Check the results
    for (int i = 0; i < num_threads; i++) {
        if (results[i].data == key) {
            search_cache.insert(std::make_pair(key, &results[i]));
            ck = 1;
            std::cout << "Found a match:: k: " << key << " v: " << results[i].data << std::endl;
            break;
        }
    }

    if (ck == 0) {
        std::cout << "Not Found" << std::endl;
    }

    end_tick(start_time);
}


int main(int argc, char **argv) {
    int t;
    std::cin>>n;
    for(int i=0;i<n-1;i++){
        //int a; std::cin>>a;
        t = rand()%10;
        insertEnd(t);
    }
    insertEnd(10);

    unsigned int num_threads = std::thread::hardware_concurrency();

    parallelSearch(10, num_threads);
    parallelSearch(10, num_threads);

    return 0;
}
