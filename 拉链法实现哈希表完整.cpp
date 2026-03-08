#include<iostream>//输入输出流
#include<list>//链表
#include<vector>//向量
#include<stdexcept>//标准异常处理
#include<algorithm>//算法库
#include<string>//string>//字符串
#include<memory>//智能指针
template<typename K,typename V>//模板类定义
class MyChainHashmap{//链式哈希表类
    struct KVNode{//键值节点结构体
        K key;
        V value;
        KVNode(K key,V value):key(key),value(value){};//构造函数,初始化成员变量
    };
    std::vector<std::list<KVNode>>table;//哈希表，使用链表解决冲突
    //每个bucket是一个链表，存储KVNode节点,最底层是一个二维数组
    int size_;//哈希表大小
    static constexpr int INIT_SIZE=16;//初始哈希表大小
    int hash(K key){
        return (std::hash<K>{}(key)&0x7fffffff)%table.size();//使用std::hash计算哈希值，并取模运算
        //&0x7fffffff是为了确保哈希值为非负数,因为std::hash可能返回负数
    }
    void resize(int newCap){
        newCap=std::max(newCap,1);//保证新的容量至少为1
        MyChainHashmap<K,V> newMap(newCap);//创建新的哈希表
        for(auto& list : table){
            for(auto& node : list){
               newMap.table[newMap.hash(node.key)].emplace_back(node.key,node.value);//重新哈希并插入到新的哈希表中
               newMap.size_++;
            }
        }
        table.swap(newMap.table);//交换哈希表
        size_=newMap.size_;//更新大小
    }
    public:
    MyChainHashmap():MyChainHashmap(INIT_SIZE){};//默认构造函数，使用初始大小
    explicit MyChainHashmap(int capacity){
        size_=0;
        int initcapacity=std::max(capacity,1);
        table.resize(initcapacity);//初始化哈希表大小

    }
    void put(K key,V value){
        auto&list=table[hash(key)];
        for(auto& node : list){
            if(node.key==key){
                node.value=value;
                return;//如果找到匹配的键，更新对应的值并返回
            }
        }
        list.emplace_back(key,value);//如果没有找到匹配的键，插入新的键值对，存到链表末尾
        size_++;
        if(size_>=table.size()*0.75){
            resize(table.size()*2);//如果负载因子超过0.75，进行扩容
        }
    }
    void remove(K key){
        auto&list=table[hash(key)];
        for(auto it=list.begin();it!=list.end();++it){//it表示链表的迭代器
            if(it->key==key){
                list.erase(it);//链表的erase方法删除指定位置的元素
                size_--;
                if(size_<=table.size()*0.125){
                    resize(table.size()/4);//如果负载因子低于0.125，进行缩容
                }
                return;//如果找到匹配的键，删除对应的键值对并返回
            }
        }
    }
    V get(K key){
        const auto&list=table[hash(key)];
        for(const auto&node:list){
            if(node.key==key){
                return node.value;//如果找到匹配的键，返回对应的值
            }
        }
        return nullptr;//如果没有找到匹配的键，返回nullptr
    }
    std::list<K> keys(){
        std::list<K>keys;
        for(const auto&list:table){
            for(const auto&node:list){
                keys.push_back(node.key);
            }
        }
        return keys;
    }
    int size()const{
        return size_;
    }


};
int main() {
    MyChainHashmap<int, std::shared_ptr<int> > map;
    map.put(1, std::make_shared<int>(1));
    map.put(2, std::make_shared<int>(2));
    map.put(3, std::make_shared<int>(3));
    std::cout << *map.get(1) << std::endl; // 1
    std::cout << *map.get(2) << std::endl; // 2

    map.put(1, std::make_shared<int>(100));
    std::cout << *map.get(1) << std::endl; // 100

    map.remove(2);
    std::cout << (map.get(2) == nullptr ? "null" : std::to_string(*map.get(2))) << std::endl; // null

    for (const auto key: map.keys()) {
        std::cout << key << " ";
    }
    // [1, 3]（顺序可能不同）
    std::cout << std::endl;

    map.remove(1);
    map.remove(2);
    map.remove(3);
    std::cout << (map.get(1) == nullptr ? "null" : std::to_string(*map.get(1))) << std::endl; // null

    return 0;
}