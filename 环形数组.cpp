#include<iostream>
#include<vector>
#include<stdexcept>
#include<ostream>
template<typename E>
class CycleArray{
    std::vector<E> arr;//
    int start;
    int end;
    int count;
    void resize(int newcapacity){
        std::vector<E> newarr(newcapacity);
        for(int i=0;i<count;i++){
            newarr[i]=arr[(start+i)%arr.size()];//将原数组中的元素复制到新数组中，注意循环数组的特性
            //arr[(start+i)%arr.size()]是原数组中第i个元素的位置，start是循环数组的起始位置，%arr.size()是为了保证索引不越界
        }

        arr=std::move(newarr);//深拷贝
        start=0;
        end=count;//end指向新数组中第一个空位置
    }
    public:
    CycleArray():CycleArray(1){}//默认容量为1
    explicit CycleArray(int capacity):
    arr(capacity),start(0),end(0),count(0){}//构造函数，初始化循环数组的容量、起始位置、结束位置和元素个数

    void addFirst(E element){
        if(isFull()){
            resize(arr.size()*2);
        }
        start=(start-1+arr.size())%arr.size();//将起始位置向前移动一位，注意循环数组的特性
        //这个公式的作用是将start向前移动一位，如果start已经是0了，那么就会变成arr.size()-1，保证了循环数组的特性
        //%arr.size()是为了保证索引不越界
        arr[start]=element;//将新元素放在起始位置
        count++;//元素个数加1

    }
    void removeFirst(){
        if(isEmpty()){
            throw std::runtime_error("循环数组为空，无法删除元素");
        }
        start=(start+1)%arr.size();//将起始位置向后移动一位，注意循环数组的特性
        //这个公式的作用是将start向后移动一位，如果start已经是arr.size()-1了，那么就会变成0，保证了循环数组的特性
        //%arr.size()是为了保证索引不越界
        count--;//元素个数减1
        if(count>0&&count==arr.size()/4){
            resize(arr.size()/2);
        }//当元素个数大于0且等于数组容量的1/4时，缩小数组容量为原来的一半，避免浪费空间

    }
    void addLast(E element){
        if(isFull()){
            resize(arr.size()*2);
    }
        arr[end]=element;//将新元素放在结束位置
        end=(end+1)%arr.size();//将结束位置向后移动一位，注意循环数组的特性
        //这个公式的作用是将end向后移动一位，如果end已经是arr.size()-1了，那么就会变成0，保证了循环数组的特性
        //%arr.size()是为了保证索引不越界
        count++;//元素个数加1

    }
    void removeLast(){
        if(isEmpty()){
            throw std::runtime_error("循环数组为空，无法删除元素");
        }
        end=(end-1+arr.size())%arr.size();//将结束位置向前移动一位，注意循环数组的特性
        //这个公式的作用是将end向前移动一位，如果end已经是0了，那么就会变成arr.size()-1，保证了循环数组的特性
        //%arr.size()是为了保证索引不越界
        count--;//元素个数减1
        if(count>0&&count==arr.size()/4){
            resize(arr.size()/2);
        }
        }
        E getFirst()const{
            if(isEmpty()){
                throw std::runtime_error("循环数组为空，无法获取元素");
            }
            return arr[start];//返回起始位置的元素

        }
         E getLast()const{
            if(isEmpty()){
                throw std::runtime_error("循环数组为空，无法获取元素");
            }
            return arr[(end-1+arr.size())%arr.size()];//返回结束位置的前一个元素，注意循环数组的特性
        }
    bool isEmpty()const{
        return count==0;//当元素个数为0时，循环数组为空
    }
    bool isFull()const{
        return count==arr.size();//当元素个数等于数组容量时，循环数组为满

    }
    int size(){
        return count;//返回元素个数
    }
    
    

};
