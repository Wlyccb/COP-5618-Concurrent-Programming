//
//  threadLocal_test.cpp
//  concurren
//
//  Created by 王俪越 on 16/9/30.
//  Copyright © 2016年 uf. All rights reserved.
//

#include <iostream>
#include <atomic>
#include <thread>
#include <mutex>
#include <vector>
#include <algorithm>
#include <exception>
#include <list>
#include "threadLocal.h"

std::mutex m;
//static std::atomic<int> sequencer(0);
static cop5618::threadLocal<int> table;

int test_table_0(int num_threads);
int test_table_1(int num_threads);
int test_table_2(int num_threads);


int test_threadLocal() {
    int num_errs_0 = test_table_0(10);
    int num_errs_1 = test_table_1(10);
    int num_errs_2 = test_table_2(10);
    std::cout<<"test_0 has "<<num_errs_0<<" error."<<std::endl;
    std::cout<<"test_1 has "<<num_errs_1<<" error."<<std::endl;
    std::cout<<"test_2 has "<<num_errs_2<<" error."<<std::endl;
    std::cout<<"total error is "<<num_errs_0+num_errs_1+num_errs_2<<std::endl;
    return num_errs_0+num_errs_1+num_errs_2;
}

struct uninitialize: public std::exception{
    virtual const char* what() const throw(){
        return "not initialize";
    }
};

// test set() for int
int test_table_0(const int num_threads) {
    std::vector<std::thread> threads;
    std::vector<std::thread::id> ids;
    cop5618::threadLocal<int> table;
    //std::atomic<int> sequencer(0);
    std::vector<int> sequencer(num_threads);
    for (int i=0;i<num_threads;i++) sequencer[i] = i;
    for (int t = 0; t < num_threads; t++) {
        threads.push_back(
                          std::thread (
                                       [&sequencer, &table,t]() mutable->void {
                                           table.set(sequencer[t]);
                                           
                                       }));
        ids.push_back(threads[t].get_id());
    }
    for (int i=0;i<10;i++) {
        threads[i].join();
    }
    //std::for_each(threads.begin(), threads.end(),
                  //std::mem_fn(&std::thread::join));
    
    int expected_size = num_threads;
    if (expected_size != table.size()) {
        std::cout << "queue size not correct. expected_size=" << expected_size
        << ", size=" << table.size() << std::endl;
        return 1;
    }
    
    std::unordered_map<std::thread::id, int>& hash_table = table.get_backing_table();
    int err = 0;
    for (int i=0;i<threads.size();i++) {
        //std::cout<<ids[i]<<" "<<hash_table[ids[i]]<<std::endl;
        if (hash_table[ids[i]] == i) continue;
        else err++;
    }
    return err;
}



//test set() for char
int test_table_1(const int num_threads) {
    std::vector<std::thread> threads;
    std::vector<std::thread::id> ids;
    cop5618::threadLocal<char> table;
    
    //std::atomic<char> sequencer('a');
    std::vector<char> sequencer(num_threads);
    for (int i=0;i<num_threads;i++) sequencer[i] = char(i+'a');
    
    for (int t = 0; t < num_threads;t++) {
        threads.push_back(
                          std::thread (
                                       [&sequencer, &table, t]() mutable->void {
                                           table.set(sequencer[t]);
                                       }));
        ids.push_back(threads[t].get_id());
        
    }
    
    std::for_each(threads.begin(), threads.end(),
                  std::mem_fn(&std::thread::join));
    
    int err = 0;
    std::unordered_map<std::thread::id, char>& hash_table = table.get_backing_table();
    for (int i=0;i<ids.size();i++) {
        //std::cout<<ids[i]<<" "<<hash_table[ids[i]]<<std::endl;
        if (int(hash_table[ids[i]]-'a') == i) continue;
        else err++;
    }
    return err;
}

//test set() and remove() for int
//test uninitialize exception
int test_table_2(const int num_threads) {
    std::vector<std::thread> threads;
    std::vector<std::thread::id> ids;
    cop5618::threadLocal<int> table;
    
    std::vector<int> sequencer(num_threads);
    std::vector<int> values(num_threads);
    for (int i=0;i<num_threads;i++) sequencer[i] = i;
    
    
    for (int t = 0; t < num_threads; t++) {
        threads.push_back(
                          std::thread (
                                       [&sequencer, &table, t,&values]() mutable->void {
                                           try {
                                               table.set(sequencer[t]);
                                               if (t>4) table.remove();
                                               values[t]=table.get();
                                           }
                                           catch (std::exception& e) {
                                               //if uninitialize, record -1
                                               values[t] = -1;
                                               m.lock();
                                               std::cout << "exception" <<std::endl;
                                               m.unlock();
                                           }
                                       }));
        ids.push_back(threads[t].get_id());
        
    }
    
    std::for_each(threads.begin(), threads.end(),
                  std::mem_fn(&std::thread::join));
    
    int err = 0;
    for (int i=0;i<values.size();i++) {
        if (values[i] == i || values[i] == -1) continue;
        else err++;
    }
    return err;
}


