#include<iostream>
#include<vector>
#include<list>
#include<stdexcept>
template <typename E>
class MyLinkedStack{
    private:
    std::list<E> list;//使用STL的链表作为底层数据结构
    public:
    void push( const E& element){
        list.push_back(element);//在链表尾部添加元素
    }
    E pop(){
        if(isEmpty()){
            throw std::out_of_range("栈为空，无法弹出元素");
        }
        E value=list.back();
        list.pop_back();//删除链表尾部的元素
        return value;
    }
    E peek() const{
        return list.back();//返回链表尾部的元素，但不删除
    }
    int size() const{
        return list.size();//返回链表的大小
    }
    int isEmpty() const{
        return list.empty();//判断链表是否为空
    }
    

};template <typename E>

class MyLinkedQueue{
    private:
    std::list<E> list;//使用STL的链表作为底层数据结构
    public:
    void enqueue(const E& element){
        list.push_back(element);//在链表尾部添加元素
    }
    E dequeue(){
        if(isEmpty()){
            throw std::out_of_range("队列为空，无法出队元素");
        }
        E value=list.front();
        list.pop_front();//删除链表头部的元素
        return value;
    }
    E peek()const{
        return list.front();//返回链表头部的元素，但不删除
    }
    int size()const{
        return list.size();//返回链表的大小
    }

};
int main(){
    MyLinkedStack<int> stack;
    stack.push(10);
    stack.push(20);
    stack.push(30);
    std::cout<<"栈顶元素为: "<<stack.peek()<<std::endl;
    std::cout<<"栈的大小为: "<<stack.size()<<std::endl;
    stack.pop();
    std::cout<<"弹出栈顶元素后，栈顶元素为: "<<stack.peek()<<std::endl;
    std::cout<<"弹出栈顶元素后，栈的大小为: "<<stack.size()<<std::endl;
    return 0;
}