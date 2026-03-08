#include<vector>
#include<list>
#include<stdexcept>
#include<iostream>
#include<algorithm>
class Hashmap{
    struct KVNode{
        int key;
        int value;
        KVNode(int key,int value):key(key),value(value){};//构造函数,初始化成员变量
    };
    std::vector<std::list<KVNode>>table;//哈希表，使用链表解决冲突
    public:
    Hashmap(int size):table(size){};//构造函数，初始化哈希表大小
    int hash(int key){
        return key%table.size();//简单的哈希函数，取模运算

    }
    int get(int key){
        int index=hash(key);//计算哈希值
        if(table[index].empty()){
            return -1;//如果链表为空，返回-1表示未找到
        }
        for(const auto&node :table[index]){
            if(node.key==key){
                return node.value;//如果找到匹配的键，返回对应的值
            }
        }
        return -1;//如果没有找到匹配的键，返回-1表示未找到
    }
    void put(int key,int value){
        int index=hash(key);
        if(table[index].empty()){
            table[index].push_back(KVNode(key,value));//如果链表为空，直接插入新的键值对
            return;
        }
        for(auto&node:table[index]){
            if(node.key==key){
                node.value=value;//如果找到匹配的键，更新对应的值
                return;
            }
        }
        table[index].push_back(KVNode(key,value));//如果没有找到匹配的键，插入新的键值对
    }
    void remove(int key){
        auto& list=table[hash(key)];//获取对应哈希值的链表
        if(list.empty()){
            return;//如果链表为空，直接返回
        }
        list.remove_if([key](const KVNode&node){ return node.key==key; });//使用remove_if删除匹配的键值对
    }
};
