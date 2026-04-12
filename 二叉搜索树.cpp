#include<iostream>
#include<queue>
using namespace std;
template<typename K,typename V>
class TreeMap{
    public:
    K key;
    V value;
    TreeMap<K,V>*left;
    TreeMap<K,V>*right;
    TreeMap(K key,V value):key(key),value(value),left(nullptr),right(nullptr){}
};