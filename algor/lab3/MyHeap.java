/**
* @Name: Heap
* Provides generic implementation of a binary heap
* @version CPE_349
* @date 1/14/15
* @author Justin Distaso jdistaso@calpoly.edu
***
BinHeap adapted from program personally written for CPE_103
***
*/
import java.util.*;
public class MyHeap
{
   public int[] arr; //array that holds the heap
   //left child = index *2
   //right child = index*2 +1
   //parent = index/2
   int size; //number of elements in heap
   public MyHeap() //makes a binheap with size 50
   {
      arr  = new int[50];
      size=0;
      arr[0]=99999999; //To prevent values from being swapped with arr[0]
   }
   
   public MyHeap(int param) //constructor that takes size of array
   {
     arr  = new int[param];
     size=0; 
     arr[0]=99999999;
   }
   public boolean buildHeap(int[] array) //builds heap with given array
   {
      if(array.length > (arr.length-size))
      {
         return false;
      }
      else
      {
         for(int i=0; i<array.length; i++)
         {
            insert(array[i]);
         }
         return true;
      }
   }
   public void printHeap() //prints contents of heap
   {
      String temp=" ";
      for(int i=1;i<=size;i++)
      {
            temp+=arr[i];
            temp+=" ";
      }
      System.out.printf("Heap Contents\n%s\n",temp);
   }
   public boolean insert(int num)
   {
      if(size==arr.length-1)
      {
         return false;
      }
      size++;
      arr[size] = num;
      int pos = size;
      while(arr[pos] > arr[pos/2]) //while child is less than parent, drift up basically.
      {
         swap(pos,pos/2); //swap child and parent
         pos=pos/2;
      } //will exit when child child is not more than parent (ie a max heap)
      return true;
   } 
   private void swap(int one,int two)
   {
        int tmp;
        tmp = arr[one];
        arr[one] = arr[two];
        arr[two] = tmp;
   }
   public int findMax()
   {
      return arr[1];
   }
   public int deleteMax()
   {
      int temp = arr[1];
      arr[1] = arr[size];
      size--;
      driftDown(1);
      return temp;
   }
   public void driftDown(int pos) //drifts node down to it's correct place
   { 
     if (!(pos >= (size / 2)  &&  pos <= size)) //if it's not a leaf
     { 
       if (arr[pos] < arr[pos*2]  ||  arr[pos] < arr[(pos*2)+1]) //if its less then it's children
       {
          if (arr[pos*2] > arr[(pos*2)+1]) //if left child is greater than right child
          {
             swap(pos, pos*2); //swap node with its left child
             driftDown(pos*2); //make sure the node is in correct place
          }
          else //case where right child is greater
          {
             swap(pos, (pos*2)+1); //swap parent with its right child
             driftDown((pos*2)+1); //make sire the node is in correct place
          }
       }
     }
   }
   public boolean isEmpty() //checks if heap is empty
   {
      return size==0;
   }
   public boolean isFull() //checks if heap is empty
   {
      return size==arr.length;
   }
   public int getHeapSize() //returns size
   {
      return size;
   }
   public int getHeapCap()
   {
      return arr.length;
   }
   public int[] heapSortIncreasing(int[] array)
   {
      Arrays.sort(array);
      return array;
   }
}