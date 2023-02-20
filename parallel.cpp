#include<iostream>
#include <iomanip>
#include <thread>
#include <vector>
#include <unordered_map>
#include "common.h"

int ck = 0, n = 0;
bool stop_thread = false;

void search(Node *start, Node *end, int key, Node &result) {
    if (stop_thread) return;

    for (Node *current = start; current != end; current = current->next) {
        if (current->data == key) {
            result = *current;
//            std::cout << "Found" << std::endl;
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


void parallelSearch(int key) {
    auto start_time = start_tick();
    stop_thread = false;
    ck = 0;
    auto c_result = search_cache.find(key);
    if (c_result != search_cache.end()) {
        ck = 1;
        std::cout << "Found a match in cache:: k: " << c_result->first <<std::endl;// " v: " << c_result->second->data << std::endl;
        end_tick(start_time);
        return;
    }

    unsigned int num_threads = std::thread::hardware_concurrency();
    auto chunk_size = (n + num_threads - 1) / num_threads;

    std::vector<std::thread> threads(num_threads);
    Node result;

    // Launch the threads to search for the key in each chunk
    Node *start = head;
    for (int i = 0; i < num_threads; i++) {
        auto c_ptr = chunk_cache.find(i);
        if (c_ptr != chunk_cache.end()) {
            threads[i] = std::thread(search, c_ptr->second[0], c_ptr->second[1], key, std::ref(result));
            threads[i].join();
        } else {
            Node *Nend = start;
            for (int j = 0; j < chunk_size && Nend != nullptr; j++) {
                Nend = Nend->next;
            }
            threads[i] = std::thread(search, start, Nend, key, std::ref(result));
            //Cache chunk boundaries
            std::vector<Node *> tmp;
            tmp.push_back(start);
            tmp.push_back(Nend);
            chunk_cache.insert(std::make_pair(i, tmp));
            start = Nend;
            // join thread to main thread
            threads[i].join();
        }
    }


    if (ck == 0) {
        std::cout << "Not Found" << std::endl;
    } else {
        search_cache.insert(std::make_pair(key, &result));
        std::cout << "Found a match:: k: " << key << " v: " << result.data << std::endl;

    }

    end_tick(start_time);
}

int main(int argc, char **argv) {
    std::cin >> n;
    for (long long i = 0; i < n; i++) {
        //int a; std::cin>>a;
        insertEnd(i);
    }

    while (true) {
        std::cout << "Enter Key: ";
        int key;
        std::cin >> key;
        parallelSearch(key);
    }

    return 0;
}
