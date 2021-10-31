#include "hash_prime.hpp"

#include <exception>
#include <functional>
#include <vector>
#include <forward_list>
#include <iostream>

/**
 * The Hashtable class
 * The time complexity of functions are based on n and k
 * n is the size of the hashtable
 * k is the length of Key
 * @tparam Key          key type
 * @tparam Value        data type
 * @tparam Hash         function object, return the hash value of a key
 * @tparam KeyEqual     function object, return whether two keys are the same
 */
template<
        typename Key, typename Value,
        typename Hash = std::hash<Key>,
        typename KeyEqual = std::equal_to<Key>
>
class HashTable {
public:
    typedef std::pair<const Key, Value> HashNode;
    typedef std::forward_list<HashNode> HashNodeList; //The forward list is a singly linked list 
    typedef std::vector<HashNodeList> HashTableData;

    /**
     * A single directional iterator for the hashtable
     * ! DO NOT NEED TO MODIFY THIS !
     */
    class Iterator {
    private:
        typedef typename HashTableData::iterator VectorIterator; // hash table iterator
        typedef typename HashNodeList::iterator ListIterator; // linked list iterator

        const HashTable *hashTable;
        VectorIterator bucketIt;    // an iterator of the buckets
        ListIterator listItBefore;  // a before iterator of the list, here we use "before" for quick erase and insert
        bool endFlag = false;       // whether it is an end iterator

        /**
         * Increment the iterator
         * Time complexity: Amortized O(1)
         */
        void increment() {
            if (bucketIt == hashTable->buckets.end()) {
                endFlag = true;     // reach the end bucket of the iterator
                return;
            }
            auto newListItBefore = listItBefore;
            ++newListItBefore;
            if (newListItBefore != bucketIt->end()) {
                if (++newListItBefore != bucketIt->end()) {
                    // use the next element in the current forward_list
                    ++listItBefore;
                    return;
                }
            }
            while (++bucketIt != hashTable->buckets.end()) {
                if (!bucketIt->empty()) {
                    // use the first element in a new forward_list
                    listItBefore = bucketIt->before_begin();
                    return;
                }
            }
            endFlag = true;
        }

        explicit Iterator(HashTable *hashTable) : hashTable(hashTable) {
            bucketIt = hashTable->buckets.begin();
            listItBefore = bucketIt->before_begin();
            endFlag = bucketIt == hashTable->buckets.end();
        }

        Iterator(HashTable *hashTable, VectorIterator vectorIt, ListIterator listItBefore) :
                hashTable(hashTable), bucketIt(vectorIt), listItBefore(listItBefore) {
            endFlag = bucketIt == hashTable->buckets.end();
        }

    public:
        friend class HashTable;

        Iterator() = delete;

        Iterator(const Iterator &) = default;

        Iterator &operator=(const Iterator &) = default;

        Iterator &operator++() {
            increment();
            return *this;
        }

        Iterator operator++(int) {
            Iterator temp = *this;
            increment();
            return temp;
        } 
        // enable the interator++ and ++iterator

        bool operator==(const Iterator &that) const {
            if (endFlag && that.endFlag) return true;
            if (bucketIt != that.bucketIt) return false;
            return listItBefore == that.listItBefore;
        }

        bool operator!=(const Iterator &that) const {
            if (endFlag && that.endFlag) return false;
            if (bucketIt != that.bucketIt) return true;
            return listItBefore != that.listItBefore;
        }

        HashNode *operator->() {
            auto listIt = listItBefore;
            ++listIt;
            return &(*listIt);
        }

        HashNode &operator*() {
            auto listIt = listItBefore;
            ++listIt;
            return *listIt;
        }
    };

protected:                                                                  // DO NOT USE private HERE!
    static constexpr double DEFAULT_LOAD_FACTOR = 0.5;                      // default maximum load factor is 0.5
    static constexpr size_t DEFAULT_BUCKET_SIZE = HashPrime::g_a_sizes[0];  // default number of buckets is 5

    HashTableData buckets;                                                  // buckets, of singly linked lists
    typename HashTableData::iterator firstBucketIt;                         // help get begin iterator in O(1) time

    size_t tableSize;                                                       // number of elements
    double maxLoadFactor;                                                   // maximum load factor
    Hash hash;                                                              // hash function instance
    KeyEqual keyEqual;                                                      // key equal function instance

    /**
     * Time Complexity: O(k)
     * @param key
     * @param bucketSize
     * @return the hash value of key with a new bucket size
     */
    inline size_t hashKey(const Key &key, size_t bucketSize) const {
        return hash(key) % bucketSize;
    }

    /**
     * Time Complexity: O(k)
     * @param key
     * @return the hash value of key with current bucket size
     */
    inline size_t hashKey(const Key &key) const {
        return hash(key) % buckets.size();
    }

    /**
     * Find the minimum bucket size for the hashtable
     * The minimum bucket size must satisfy all of the following requirements:
     * - It is not less than (i.e. greater or equal to) the parameter bucketSize
     * - It is greater than floor(tableSize / maxLoadFactor)
     * - It is a (prime) number defined in HashPrime (hash_prime.hpp)
     * - It is minimum if satisfying all other requirements
     * Time Complexity: O(1)
     * @throw std::range_error if no such bucket size can be found
     * @param bucketSize lower bound of the new number of buckets
     */
    size_t findMinimumBucketSize(size_t bucketSize) const {
        size_t desired_tablesize = (size_t)((double)tableSize / maxLoadFactor); //floor (tablesize / maxLoadFactor)
        size_t lower_bound = (bucketSize > desired_tablesize) ? bucketSize : desired_tablesize;
        int i = 0;
        while (HashPrime::g_a_sizes[i] < lower_bound && i < HashPrime::num_distinct_sizes_64_bit) {
            i++;
        }
        if(i >= HashPrime::num_distinct_sizes_64_bit) {
            std::range_error range_error("range error!");
            throw range_error;
        }
        else {
            return HashPrime::g_a_sizes[i];
        }
    }

    // TODO: define your helper functions here if necessary


public:
    HashTable() :
            buckets(DEFAULT_BUCKET_SIZE), tableSize(0), maxLoadFactor(DEFAULT_LOAD_FACTOR),
            hash(Hash()), keyEqual(KeyEqual()) {
        firstBucketIt = buckets.end();
    }

    explicit HashTable(size_t bucketSize) :
            tableSize(0), maxLoadFactor(DEFAULT_LOAD_FACTOR),
            hash(Hash()), keyEqual(KeyEqual()) {
        bucketSize = findMinimumBucketSize(bucketSize);
        buckets.resize(bucketSize);
        firstBucketIt = buckets.end();
    }

    HashTable(const HashTable &that) {
        this->buckets = that.buckets;
        this->firstBucketIt = that.firstBucketIt;
        this->tableSize = that.tableSize;
        this->maxLoadFactor = that.maxLoadFactor;
        this->hash = that.hash;
        this->keyEqual = that.keyEqual;
    }

    HashTable &operator=(const HashTable &that) {
        this->buckets = that.buckets;
        this->firstBucketIt = that.firstBucketIt;
        this->tableSize = that.tableSize;
        this->maxLoadFactor = that.maxLoadFactor;
        this->hash = that.hash;
        this->keyEqual = that.keyEqual;
        return *this;
    };

    ~HashTable() = default;

    Iterator begin() {
        if (firstBucketIt != buckets.end()) {
            return Iterator(this, firstBucketIt, firstBucketIt->before_begin());
        }
        return end();
    }

    Iterator end() {
        return Iterator(this, buckets.end(), buckets.begin()->before_begin());
    }

    /**
     * Find whether the key exists in the hashtable
     * Time Complexity: Amortized O(k)
     * @param key
     * @return whether the key exists in the hashtable
     */
    bool contains(const Key &key) {
        return find(key) != end();
    }

    /**
     * Find the value in hashtable by key
     * If the key exists, iterator points to the corresponding value, and it.endFlag = false
     * Otherwise, iterator points to the place that the key were to be inserted, and it.endFlag = true
     * Time Complexity: Amortized O(k)
     * @param key
     * @return a pair (success, iterator of the value)
     */
    Iterator find(const Key &key) {
        size_t hashcode = hashKey(key);
        Iterator new_iterator(this, buckets.begin()+hashcode, (buckets.begin()+hashcode)->before_begin());
        Iterator probe = new_iterator;

        //here we should declare a list iterator, because the Iterator will ignore the end() and jump to next line, which causes problem
        typename Iterator::ListIterator list_it = (buckets.begin()+hashcode)->begin();
        while(list_it != (new_iterator.bucketIt)->end() && list_it->first != key) {
            ++probe;
            ++list_it;
        }
        
        //if the iterator reaches the end of the linked list, the key does not exists
        if(list_it == (new_iterator.bucketIt)->end()) {
            new_iterator.endFlag = true;
            return new_iterator;
        }

        //else the iterator reaches a same key
        else {
            new_iterator.endFlag = false;
            new_iterator.listItBefore = probe.listItBefore;
            return new_iterator;
        }
    }

    /**
     * Insert value into the hashtable according to an iterator returned by find
     * the function can be only be called if no other write actions are done to the hashtable after the find
     * If the key already exists, overwrite its value
     * firstBucketIt should be updated
     * If load factor exceeds maximum value, rehash the hashtable
     * Time Complexity: O(k)
     * @param it an iterator returned by find
     * @param key
     * @param value
     * @return whether insertion took place (return false if the key already exists)
     */
    bool insert(const Iterator &it, const Key &key, const Value &value) {
        //If the key does not exists, we insert it at the beginning
        if(it.endFlag == true) {
            (it.bucketIt)->insert_after((it.bucketIt)->before_begin(), std::pair<const Key, Value>(key, value));
            tableSize++;

            //update the firstBucketIt
            if(it.bucketIt < firstBucketIt && !(it.bucketIt)->empty()) {
                firstBucketIt = it.bucketIt;
            }

            //check whether the table needs to be rehashed
            if(loadFactor() > maxLoadFactor) {
                rehash(bucketSize()+1);
            }

            return true;
        }

        //else we find the key already in the list, we update the value
        else {
            auto ptr = it;
            ptr->second = value;
            return false;
        }
    }

    /**
     * Insert <key, value> into the hashtable
     * If the key already exists, overwrite its value
     * firstBucketIt should be updated
     * If load factor exceeds maximum value, rehash the hashtable
     * Time Complexity: Amortized O(k)
     * @param key
     * @param value
     * @return whether insertion took place (return false if the key already exists)
     */
    bool insert(const Key &key, const Value &value) {
        return insert(find(key), key, value);
    }

    /**
     * Erase the key if it exists in the hashtable, otherwise, do nothing
     * DO NOT rehash in this function
     * firstBucketIt should be updated
     * Time Complexity: Amortized O(k)
     * @param key
     * @return whether the key exists
     */
    bool erase(const Key &key) {
        Iterator victim = find(key);
        //If we find no key in the list, then we do nothing and return false
        if(victim.endFlag == true) {
            return false;
        }
        else {
            erase(victim);
            return true;
        }
    }

    /**
     * Erase the key at the input iterator
     * If the input iterator is the end iterator, do nothing and return the input iterator directly
     * firstBucketIt should be updated
     * Time Complexity: O(1)
     * @param it
     * @return the iterator after the input iterator before the erase
     */
    Iterator erase(const Iterator &it) {
        if(it.endFlag == true) {
            return it;
        }
        else {
            Iterator after_victim = it;
            after_victim++;
            (it.bucketIt)->erase_after(it.listItBefore);
            tableSize--;

            //If the linked list is totally deleted, then we set the firstBucketIt to be the iterator after it, which starts a new line
            if(it.bucketIt->empty() && it.bucketIt == firstBucketIt) {
                firstBucketIt = after_victim.bucketIt;
            }

            return after_victim;
        }
    }

    /**
     * Get the reference of value by key in the hashtable
     * If the key doesn't exist, create it first (use default constructor of Value)
     * firstBucketIt should be updated
     * If load factor exceeds maximum value, rehash the hashtable
     * Time Complexity: Amortized O(k)
     * @param key
     * @return reference of value
     */
    Value &operator[](const Key &key) { 
        Iterator result = find(key);

        //If the iterator does not exists, create it and insert it
        if(result.endFlag == true) {
            insert(result, key, Value());

            result = find(key);
        }

        return result->second;
    }

    /**
     * Rehash the hashtable according to the (hinted) number of buckets
     * The bucket size after rehash need not be same as the parameter bucketSize
     * Instead, findMinimumBucketSize is called to get the correct number
     * firstBucketIt should be updated
     * Do nothing if the bucketSize doesn't change
     * Time Complexity: O(nk)
     * @param bucketSize lower bound of the new number of buckets
     */
    void rehash(size_t bucketSize) {
        //we first build up a new hash table with new size
        size_t desiredsize = findMinimumBucketSize(bucketSize);

        //if the desired size is not different from current size we do nothing
        if(desiredsize == buckets.size()) {
            return;
        }
        // std::cout<<"desiredsize: "<<desiredsize<<std::endl;
        std::vector<HashNode> Node_list; 

        for(auto &i : buckets) {
            for(auto &j : i) {
                Node_list.push_back(j);
            }
        }
        this->buckets.clear();
        tableSize = 0;

        //resize the table
        this->buckets.resize(desiredsize);


        for(auto &i: Node_list) {
            this->insert(i.first, i.second);
        }

        for (typename HashTableData::iterator it = this->buckets.begin(); it < this->buckets.end(); it++) {
            if (!it->empty()) {
                this->firstBucketIt = it;
                break;
            }
        }
        return;
    }

    /**
     * @return the number of elements in the hashtable
     */
    size_t size() const { return tableSize; }

    /**
     * @return the number of buckets in the hashtable
     */
    size_t bucketSize() const { return buckets.size(); }

    /**
     * @return the current load factor of the hashtable
     */
    double loadFactor() const { return (double) tableSize / (double) buckets.size(); }

    /**
     * @return the maximum load factor of the hashtable
     */
    double getMaxLoadFactor() const { return maxLoadFactor; }

    /**
     * Set the max load factor
     * @throw std::range_error if the load factor is too small
     * @param loadFactor
     */
    void setMaxLoadFactor(double loadFactor) {
        if (loadFactor <= 1e-9) {
            throw std::range_error("invalid load factor!");
        }
        maxLoadFactor = loadFactor;
        rehash(buckets.size());
    }

};

