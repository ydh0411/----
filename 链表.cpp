#include<string>
#include<stdexcept>
#include<iostream>
template<typename E>
class myLinkedList{
    //虚拟头结点
    struct Node{
        E val;
        Node*next;
        Node*prev;
        Node(E val):val(val),next(nullptr),prev(nullptr){}
    };
    Node*head;
    Node*tail;
    int size;
    public:
    myLinkedList(){//虚拟头结点和虚拟尾结点
        head=new Node(E());
        tail=new Node(E());
        head->next=tail;
        tail->prev=head;
        size=0;
    }
    ~myLinkedList(){//析构函数，释放所有节点的内存
        while(size>0){
            removeFirst();
        }
        delete head;
        delete tail;
    }
    void addLast(E element){
        Node*newNode=new Node(element);
        Node*temp=tail->prev;
        temp->next=newNode;//将新节点插入到temp和tail之间
        newNode->prev=temp;
        newNode->next=tail;//将新节点插入到temp和tail之间
        tail->prev=newNode;
        size++;
    }
    void addFirst(E element){
        Node*newNode=new Node(element);
        Node*temp=head->next;
        head->next=newNode;//将新节点插入到head和temp之间
        newNode->prev=head;
        newNode->next=temp;//将新节点插入到head和temp之间
        temp->prev=newNode;
        size++;
    }
    void add(int index,E element){
        checkPosition(index);
        if(index==size){
            addLast(element);
            return;
        }
        Node*p=getNode(index);//找到index位置的节点
        Node*newNode=new Node(element);
        Node*temp=p->prev;
       p->prev=newNode;
       temp->next=newNode;//将新节点插入到temp和p之间
       newNode->prev=temp;
       newNode->next=p;//将新节点插入到temp和p之间
       size++;
    }
    E removeFirst(){
        if(size<1){
            throw std::out_of_range("链表为空");
        }
        Node*temp=head->next;//找到第一个节点
        head->next=temp->next;//将第一个节点从链表中移除
        temp->next->prev=head;//将第一个节点从链表中移除
        E val=temp->val;//保存第一个节点的值
        delete temp;//释放第一个节点的内存
        size--;
        return val;
    }
    E removeLast(){
        if(size<1){
            throw std::out_of_range("链表为空");
        }
        Node*temp=tail->prev;//找到最后一个节点
        tail->prev=temp->prev;
        temp->prev->next=tail;//将最后一个节点从链表中移除
        E val=temp->val;//保存最后一个节点的值
        delete temp;//释放最后一个节点的内存
        size--;//size变小
        return val;
    }
    E remove(int index){
        checkElement(index);//检查index是否合法
        Node*x=getNode(index);//找到index位置的节点
        x->prev->next=x->next;//将index位置的节点从链表中移除
        x->next->prev=x->prev;//将index位置的节点从链表中移除
        E val=x->val;//保存index位置的节点的值
        delete x;//释放index位置的节点的内存
        size--;//size变小
        return val;
    }
    E get(int index){
        checkElement(index);
        Node*p=getNode(index);
        return p->val;
    }
    E getFirst(){
        if(size<1){
            throw std::out_of_range("链表为空");
        }
        return head->next->val;
    }
    E getLast(){
        if(size<1){
            throw std::out_of_range("链表为空");
        }
        return tail->prev->val;
    }
    E set(int index,E element){
        checkElement(index);
        Node*p=getNode(index);
        E oldval=p->val;//保存index位置的节点的原值
        p->val=element;//将index位置的节点的值修改为element
        return oldval;//返回index位置的节点的原值
    }
    //其他辅助函数
    int getSize(){
        return size;
    }
    bool isEmpty(){
        return size==0;
    }
    void display(){
        std::cout<<"size="<<size<<std::endl;
        for(Node*p=head->next;p!=tail;p=p->next){
            std::cout<<p->val<<"<-> ";

        }
        std::cout<<"null"<<std::endl;
        std::cout<<std::endl;  
    }
    private:
    Node*getNode(int index){
        Node*p=head->next;
        for(int i=0;i<index;i++){//找到index位置的节点
            p=p->next;
        }
        return p;
    }
    bool isElementIndex(int index){
        return index>=0&&index<size;
    }
    bool isPositionIndex(int index){
        return index>=0&&index<=size;
    }
    void checkElement(int index){
        if(!isElementIndex(index)){
            throw std::out_of_range("index位置不合法");
        }
    }
    void checkPosition(int index){
        if(!isPositionIndex(index)){
            throw std::out_of_range("index位置不合法");
        }
    }

};
int main(){
    myLinkedList<int>list;
    list.addLast(1);
    list.addLast(2);
    list.addLast(3);
    list.addFirst(0);
    list.add(2,10);
    list.display();
    std::cout<<list.removeFirst()<<std::endl;
    std::cout<<list.removeLast()<<std::endl;
    std::cout<<list.remove(1)<<std::endl;
    list.display();
     return 0;
}