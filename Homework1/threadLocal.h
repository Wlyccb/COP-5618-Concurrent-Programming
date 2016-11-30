/*
 * threadLocal.h
 *  Start with this and add what is necessary
 */

#ifndef THREADLOCAL_H_
#define THREADLOCAL_H_

#include <iostream>
#include <unordered_map>
#include <exception>  //for std::exception
#include <mutex>      //for std::mutex and std::lock_guard

namespace cop5618 {
    
struct empty_table: public std::exception{
    virtual const char* what() const throw(){
        return "attempt to remove from empty table";
    }
};
    
struct out_of_range: public std::exception{
    virtual const char* what() const throw(){
        return "out_of_range";
    }
};
    



template <typename T>
class threadLocal {
public:
    
	threadLocal()=default;
	~threadLocal()=default;

	//disable copy, assign, move, and move assign constructors
	 threadLocal(const threadLocal&)=delete;
	 threadLocal& operator=(const threadLocal&)=delete;
	 threadLocal(threadLocal&&)=delete;
	 threadLocal& operator=(const threadLocal&&)=delete;

	 /**
	 * Returns the current thread's value.
	 * If no value has been previously set by this
	 * thread, an out_of_range exception is thrown.
	 */
	const T& get() const;


	/**
	 * Sets the value of the threadLocal for the current thread
	 * to val.
	 */
	void set(T val);

	/**
	 * Removes the current thread's value for the threadLocal
	 */
	void remove();
    
    int size() const;
    
    std::unordered_map<std::thread::id, T>& get_backing_table();

	/**
	 * Friend function.  Useful for debugging only
	 */
	template <typename U>
	friend std::ostream& operator<< (std::ostream& os, const threadLocal<U>& obj);

    
private:
    //ADD PRIVATE MEMBERS
    std::unordered_map<std::thread::id, T> hashtable;
    mutable std::mutex m;


};

//ADD DEFINITIONS
    
    
    
    template<typename T>
    const T& threadLocal<T>::get() const{
        std::lock_guard<std::mutex> lock(m);
        
        auto it = hashtable.find(std::this_thread::get_id());
        if (it == hashtable.end()) throw out_of_range();
        return it->second;
        
    }
    
    template<typename T>
    void threadLocal<T>::set(T val) {
        std::lock_guard<std::mutex> lock(m);
        hashtable[std::this_thread::get_id()] = val;
    }
    
    template<typename T>
    void threadLocal<T>::remove() {
        std::lock_guard<std::mutex> lock(m);
        if (hashtable.empty()) throw empty_table();
        auto it = hashtable.find(std::this_thread::get_id());
        if (it == hashtable.end()) throw out_of_range();
        hashtable.erase(std::this_thread::get_id());
    }
    
    template<typename T>
    int  threadLocal<T>::size() const{
        std::lock_guard<std::mutex> lock(m);
        return (int)hashtable.size();
    }
    
    template <typename T>
    std::ostream& operator<< (std::ostream& os, const threadLocal<T>& obj){
        //acquire the lock
        std::lock_guard<std::mutex> lock(obj.m);
        //iterate over the contents of the backing deque
        for (auto iter = obj.table.begin(); iter != obj.table.end(); ++iter){
            if (iter != obj.table.begin()){os << ",";}//skip preceding comma if first one
            os << *iter;
        }
        return os;
    }
    
    template<typename T>
    std::unordered_map<std::thread::id, T>& threadLocal<T>::get_backing_table(){
        return hashtable;
    }
    

} /* namespace cop5618 */

#endif /* THREADLOCAL_H_ */
