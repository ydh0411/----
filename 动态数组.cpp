#include<iostream>//输入输出
#include<vector>//动态数组
#include<stdexcept>//动态数组
template<typename E>
class MyArrayList{
    private:
        E*data;
        int size;
        int capacity;
        const int INITIAL_CAPACITY=1;
    public:
        MyArrayList(){
            this->data=new E [INITIAL_CAPACITY];//new E[INITIAL_CAPACITY]分配一个初始容量的数组
            this->size=0;
            this->capacity=INITIAL_CAPACITY;
        }
        MyArrayList(int capacity){
            this->data=new E[capacity];
            this->size=0;
            this->capacity=capacity;
        }
        void add(E element){
            if(size==capacity){
                resize(2*capacity);
            }
            data[size]=element;
            size++;
        }
        void add(int index,E element){//在指定位置插入元素
            checkPosition(index);
            if(size==capacity){
                resize(2*capacity);
            }
            for(int i=size-1;i>=index;i--){
                data[i+1]=data[i];//将元素向后移动一位
            }
            data[index]=element;
            size++;
        }
        void addfirst(E element){
            add(0,element);
        }
        //删
        E remove(){
            if(size==0){
            throw std::out_of_range("Index out of range");//抛出异常
            }
        
        if(size==capacity/4&&capacity/2!=0){
            resize(capacity/2);
        }
        E deleteVal=data[size-1];
        data[size-1]=E();
        size--;
        return deleteVal;
        }
        E remove(int index){
            checkPosition(index);
            if(size==capacity/4&&capacity/2!=0){
                resize(capacity/2);
            }
            E deleteVal=data[index];
            for(int i=index+1;i<size;i++){
                data[i-1]=data[i];//将元素向前移动一位
            }
            data[size-1]=E();
            size--;
            return deleteVal;
        }
        E removefirst(){
            return remove(0);  
        }
        //查
        E get(int index){
            checkElementIndex(index);
            return data[index];
        }
        E set(int index,E element){
            checkElementIndex(index);
            E oldval=data[index];
            data[index]=element;
            return oldval;
        }
        //其他
        int getsize(){
            return size;
        }
        int getcapacity(){
            return capacity;
        }
        void resize(int newcapacity){
            E*temp=new E[newcapacity];
            for(int i=0;i<size;i++){
                temp[i]=data[i];
            }
            delete[] data;
            data=temp;
            capacity=newcapacity;
        }
        bool iselementindex(int index){//判断索引是否合法
            return index>=0&&index<size;
        }
        bool ispositionindex(int index){//判断位置是否合法
            return index>=0&&index<=size;
        }
        void checkElementIndex(int index){
            if(!iselementindex(index)){
                throw std::out_of_range("Index out of range");
            }
        }
        void checkPosition(int index){
            if(!ispositionindex(index)){
                throw std::out_of_range("Index out of range");
            }
        }
        void display(){
            std::cout<<"size:"<<size<<" capacity:"<<capacity<<" [";
            for(int i=0;i<size;i++){
                std::cout<<data[i];
                if(i!=size-1){
                    std::cout<<",";
                }
            }
            std::cout<<"]"<<std::endl;
        }
        ~MyArrayList(){//析构函数
            delete[] data;//释放内存
        }
};
int main(){
    MyArrayList<int> list;
    list.add(1);
    list.add(2);
    list.add(3);
    list.display();
    list.add(1,4);
    list.display();
    list.removefirst();
    list.display();
    list.remove(1);
    list.display();
    list.set(0,5);
    list.display();
    return 0;
}
    

