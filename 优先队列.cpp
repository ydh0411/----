#include <iostream>
#include <vector>
#include <functional>  // 比较函数对象
#include <stdexcept>   // 异常处理
using namespace std;
template<typename T>
class MyPriorityQueue {
private:
    vector<T> heap;
    int size;
    function<bool(const T&, const T&)> comparator;  // 比较函数对象
    int parent(int index) {
        return (index - 1) / 2;
    }
    int leftChild(int index) {
        return 2 * index + 1;
    }
    int rightChild(int index) {
        return 2 * index + 2;
    }
    void swap(int i, int j) {
        std::swap(heap[i], heap[j]);
    }
    void resize(int capacity) {
        if (capacity > heap.size()) {
            heap.resize(capacity);
        }
    }
    void swim(int index) {
        while (index > 0 && comparator(heap[index], heap[parent(index)])) {
            swap(index, parent(index));
            index = parent(index);
        }
    }
    void sink(int index) {
        while (leftChild(index) < size) {  // 只需检查左孩子
            int target = index;
            if (leftChild(index) < size && comparator(heap[leftChild(index)], heap[target])) {
                target = leftChild(index);
            }
            if (rightChild(index) < size && comparator(heap[rightChild(index)], heap[target])) {
                target = rightChild(index);
            }
            if (target == index) {
                break;
            }
            swap(index, target);
            index = target;
        }
    }
public:
    MyPriorityQueue(int capacity, function<bool(const T&, const T&)> comparator)
        : heap(capacity > 0 ? capacity : 1), size(0), comparator(move(comparator)) {}  // 至少容量为1
    
    int getSize() const {
        return size;
    }
    
    bool isEmpty() const {
        return size == 0;
    }
    
    const T& peek() const {
        if (isEmpty()) {
            throw runtime_error("Priority Queue is empty");
        }
        return heap[0];
    }
    void push(const T& value) {
        if (size == heap.size()) {
            resize(heap.size() * 2);  // 容量翻倍
        }
        heap[size] = value;
        swim(size);  // 上浮新元素
        size++;
    }
    T pop() {
        if (isEmpty()) {
            throw runtime_error("Priority Queue is empty");
        }
        T top = heap[0];
        swap(0, size - 1);  // 将最后一个元素与堆顶交换
        size--;             // 减小堆大小
        sink(0);            // 下沉新堆顶元素
        
        // 当元素数量降至容量的1/4时，缩小容量为一半
        if (size > 0 && size == heap.size() / 4) {
            resize(heap.size() / 2);
        }
        return top;
    }
};
int main() {
    // 创建小顶堆（最小元素在堆顶）
    MyPriorityQueue<int> minHeap(10, [](const int& a, const int& b) {
        return a < b;
    });
    
    minHeap.push(5);
    minHeap.push(3);
    minHeap.push(8);
    minHeap.push(1);
    minHeap.push(9);
    minHeap.push(2);
    
    cout << "小顶堆弹出顺序: ";
    while (!minHeap.isEmpty()) {
        cout << minHeap.pop() << " ";  // 输出：1 2 3 5 8 9
    }
    cout << endl;
    
    // 创建大顶堆（最大元素在堆顶）
    MyPriorityQueue<int> maxHeap(10, [](const int& a, const int& b) {
        return a > b;
    });
    
    maxHeap.push(5);
    maxHeap.push(3);
    maxHeap.push(8);
    maxHeap.push(1);
    
    cout << "大顶堆弹出顺序: ";
    while (!maxHeap.isEmpty()) {
        cout << maxHeap.pop() << " ";  // 输出：8 5 3 1
    }
    cout << endl;
    
    return 0;
}