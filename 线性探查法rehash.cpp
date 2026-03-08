#include <iostream>
#include <vector>
#include <list>
#include <stdexcept>

template<typename K, typename V>
class MyLinearProbingHashMap1 {
private:
    struct KVNode {
        K key;
        V val;

        KVNode(K key, V val) : key(key), val(val) {
        }
    };

    std::vector<KVNode *> table;
    int size_;
    static const int INIT_CAP = 4;

public:
    MyLinearProbingHashMap1() : MyLinearProbingHashMap1(INIT_CAP) {
    }

    MyLinearProbingHashMap1(int initCapacity) : size_(0) {
        table.resize(initCapacity, nullptr);
    }

    // **** 增/改 ****
    void put(K key, V val) {
        // 我们把负载因子默认设为 0.75，超过则扩容
        if (size_ >= table.size() * 0.75) {
            resize(table.size() * 2);
        }

        int index = getKeyIndex(key);
        // key 已存在，修改对应的 val
        if (table[index] != nullptr) {
            table[index]->val = val;
            return;
        }

        // key 不存在，在空位插入
        table[index] = new KVNode(key, val);
        size_++;
    }

    // **** 删 ****
    // 删除 key 和对应的 val
    void remove(K key) {
        // 缩容，当负载因子小于 0.125 时，缩容
        if (size_ <= table.size() / 8) {
            resize(table.size() / 4);
        }

        int index = getKeyIndex(key);
        if (table[index] == nullptr) {
            // key 不存在，不需要 remove
            return;
        }

        // 开始 remove
        delete table[index];
        table[index] = nullptr;
        size_--;
        // 保持元素连续性，进行 rehash
        index = (index + 1) % table.size();
        while (table[index] != nullptr) {
            KVNode *entry = table[index];
            table[index] = nullptr;
            size_--;
            put(entry->key, entry->val);
            delete entry;
            index = (index + 1) % table.size();
        }
    }

    // **** 查 ****
    // 返回 key 对应的 val，如果 key 不存在，则返回 null
    V get(K key) {
        int index = getKeyIndex(key);
        if (table[index] == nullptr) {
            return V(); // 返回默认值
        }
        return table[index]->val;
    }

    // 返回所有 key（顺序不固定）
    std::list<K> keys() {
        std::list<K> keys;
        for (KVNode *entry: table) {
            if (entry != nullptr) {
                keys.push_back(entry->key);
            }
        }
        return keys;
    }

    // **** 其他工具函数 ****
    int size() const {
        return size_;
    }

private:
    // 哈希函数，将键映射到 table 的索引
    int hash(K key) {
        return (std::hash<K>()(key) & 0x7fffffff) % table.size();
    }

    // 对 key 进行线性探查，返回一个索引
    int getKeyIndex(K key) {
        int index;
        for (index = hash(key); table[index] != nullptr; index = (index + 1) % table.size()) {
            if (table[index]->key == key) {
                return index;
            }
        }
        return index;
    }

    void resize(int newCap) {
        MyLinearProbingHashMap1<K, V> newMap(newCap);
        for (KVNode *entry: table) {
            if (entry != nullptr) {
                newMap.put(entry->key, entry->val);
            }
        }
        table = std::move(newMap.table);
    }
};

int main() {
    MyLinearProbingHashMap1<int, int> map;
    map.put(1, 1);
    map.put(2, 2);
    map.put(10, 10);
    map.put(20, 20);
    map.put(30, 30);
    map.put(3, 3);
    std::cout << map.get(1) << std::endl; // 1
    std::cout << map.get(2) << std::endl; // 2
    std::cout << map.get(20) << std::endl; // 20

    map.put(1, 100);
    std::cout << map.get(1) << std::endl; // 100

    map.remove(20);
    std::cout << map.get(20) << std::endl; // 0 (null)
    std::cout << map.get(30) << std::endl; // 30

    return 0;
}