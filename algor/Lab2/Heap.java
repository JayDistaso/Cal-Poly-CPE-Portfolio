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
public class Heap <T extends Comparable<? super T>>
{
   public static class MyException extends RuntimeException
   {
      public MyException() //generic constructor
      {
         super();
      }
      public MyException(String msg) //constructor that takes a message
      {
         super(msg);
      }
   }
   private T[] arr; //array
   int size; //number of elements
   public Heap() //makes a binheap with size 100
   {
      arr  = (T[]) new Comparable [100];
      size=0;
   }
   public Heap(int param) //constructor that takes size of array
   {
     arr  = (T[]) new Comparable [param];
     size=0; 
   }
   public void insert(T item) //inserts item into array
   {
       if(size==arr.length-1)
       {
         resize();
       } 
       int hole=size;
       while((hole !=0) && (arr[(hole-1)/2].compareTo(item)>0))
       {
         arr[hole]=arr[(hole-1)/2];
         hole=(hole-1)/2;
       }
       arr[hole]=item;
       size++;
   }
   private void resize() //resizes array
   {
      T[] temp=arr;
      temp  = (T[]) new Comparable [arr.length*2];
      for(int i=0;i<arr.length;i++)
      {
         temp[i]=arr[i];
      }
      arr=temp;
   }
   public T deleteMin() //deletes min element in array with compareto
   {
      if(isEmpty())
      {
         throw new MyException("ERROR: The heap is empty");
      }
      T temp = arr[0];
      T item= arr[size-1];
      int hole=0;
      int trav = trav(hole,item);
      size--;
      while(trav!=-1)
      {
         arr[hole]=arr[trav];
         hole=trav;
         trav=trav(hole,item);
      }
      arr[hole]=item;
      return temp;
   }
  private int trav (int hole, T item) //helps traverse heap for lowest element and fixing heap after delete
  {
      int trav = -1;
      if((hole*2+1)<size) 
      {
         if((hole*2+2)>=size) 
         {
            if(arr[hole*2+1].compareTo(item) < 0) 
            {
               trav = hole*2+1;
            }
         }
         else 
         {
            if (arr[hole*2+1].compareTo(arr[hole*2+2]) < 0) 
            {
               if (arr[hole*2+1].compareTo(item) < 0)
               {
                  trav = hole*2+1;
               }
            }
            else 
            {
               if (arr[hole*2+2].compareTo(item) < 0)
               {
                  trav = hole*2+2;
               }
            }
         }
      } 
      return trav;   
   }
   public boolean isEmpty() //checks if heap is empty
   {
      return size==0;
   }
   public int size() //returns size
   {
      return size;
   }
   public String toString() //turns contents of heap into string
   {
      String temp=" ";
      for(int i=0;i<arr.length;i++)
      {
         if(arr[i]!=null)
         {
            temp+=arr[i];
            temp+=" ";
         }
      }
      return temp;
   } 
}