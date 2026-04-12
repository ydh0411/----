#include<iostream>
#include<vector>
#include<algorithm>
using namespace std;
//选择排序，时间复杂度O(n^2)，空间复杂度O(1)，不稳定排序
void choicesort(vector<int>&nums){
    int n=nums.size();
    int sortIndex=0;
    while(sortIndex<n){
        int minIndex=sortIndex;
        for(int i=sortIndex+1;i<n;i++){
            if(nums[i]<nums[minIndex]){
                minIndex=i;
            }
        }
        int temp=nums[sortIndex];
        nums[sortIndex]=nums[minIndex];
        nums[minIndex]=temp;
        sortIndex++;
    }
}
//冒泡排序,时间复杂度O(n^2)，空间复杂度O(1)，稳定排序
void bubblesort(vector<int>&nums){
    int n=nums.size();
    int swapped=1;
    for(int i=0;i<n-1;i++){
        swapped=0;
        for(int j=0;j<n-1-i;j++){
            if(nums[j]>nums[j+1]){
                swap(nums[j],nums[j+1]);
                swapped=1;
            }
        }
        if(swapped==0) break;
    }
}
//插入排序，时间复杂度O(n^2)，空间复杂度O(1)，稳定排序
void insertionsort(vector<int>&nums){
    int n=nums.size();
    for(int i=1;i<n;i++){
        int key=nums[i];//待插入元素
        int j=i-1;
        while(j>=0&&nums[j]>key){//将大于key的元素向右移动
            nums[j+1]=nums[j];
            j--;
        }
        nums[j+1]=key;

    }
}
//希尔排序，时间复杂度O(n^1.3)，空间复杂度O(1)，不稳定排序
void shellsort(vector<int>&nums){
    int n=nums.size();
    for(int gap=n/2;gap>0;gap/=2){
        for(int i=gap;i<n;i++){
            int key=nums[i];
            int j=i-gap;
            while(j>=0&&nums[j]>key){
                nums[j+gap]=nums[j];
                j-=gap;
            }
            nums[j+gap]=key;
        }
    }
}