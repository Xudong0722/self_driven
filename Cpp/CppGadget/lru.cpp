#include <iostream>
#include <unordered_map>
#include <cassert>
//LRU cache
//get/put O(1)
//刚加入队列的需要放在头部，刚被访问的需要放在头部
//加入队列时，如果队列满了，就删除尾部的元素

//使用双向链表+哈希表

template<typename T>
struct Node{
    int key;
    T value;
    Node* prev{nullptr};
    Node* next{nullptr};

    Node() = default;
    Node(int k, T v)
      : key(k), value(std::move(v)){}
};

template<typename T>
class LRUCache{
public:
    LRUCache(int cap = 3)
      : capacity(cap) {
        assert(cap > 1);
        head = new Node<T>();
        tail = new Node<T>();
        head->next = tail;
        tail->prev = head;
    }

    ~LRUCache() {
        auto cur = head;
        while(cur) {
            head = cur->next;
            delete cur;
            cur = head;
        }
    }

    std::pair<bool, T> get(int key) {
        auto it = cache.find(key);
        if(it == cache.end()) {
            return {false, T{}};
        }

        Node<T>* node = it->second;
        moveToHead(node);
    
        return {true, node->value};
    }

    void put(int key, const T& val) {
        auto it = cache.find(key);
        if(it != cache.end()) {
            //update the value and move to head
            Node<T>* node = it->second;
            node->value = val;
            moveToHead(node);
            return ;
        }
        if(cache.size() >= capacity) {
            //Remove the least recently used item
            Node<T>* lru = tail->prev;
            cache.erase(lru->key);
            tail->prev = lru->prev;
            lru->prev->next = tail;
            delete lru;
            lru = nullptr;
        }
        Node<T>* new_node = new Node<T>(key, val);
        cache[key] = new_node;
        insertToHead(new_node);
        return;
    }

    void moveToHead(Node<T>*& node){
        // Move the accessed node to the head
        node->prev->next = node->next;
        node->next->prev = node->prev;

        head->next->prev = node;
        node->next = head->next;
        head->next = node;
        node->prev = head;
    }

    void insertToHead(Node<T>*& node) {
        //Insert a new node to the head;
        head->next->prev = node;
        node->next = head->next;
        head->next = node;
        node->prev = head;
    }
private:
    int capacity{0};
    Node<T>* head{nullptr};
    Node<T>* tail{nullptr};
    std::unordered_map<int, Node<T>*> cache;
};

int main() {
    LRUCache<int> cache(2);

    cache.put(1, 1);
    cache.put(2, 2);
    auto res = cache.get(1);
    std::cout << "get(1) : " << res.first << " " << res.second << std::endl; // 1
    cache.put(3, 3);
    cache.put(4, 4);
    res = cache.get(2);
    std::cout << "get(2) : " << res.first << " " << res.second << std::endl; // 0
    cache.put(5, 5);
    res = cache.get(3);
    std::cout << "get(3) : " << res.first << " " << res.second << std::endl; // 0

    res = cache.get(4);
    std::cout << "get(4) : " << res.first << " " << res.second << std::endl; // 4
    return 0;
}


/*
TODO:
1.容量的单位应该是字节，而不是元素个数
2.线程安全

*/