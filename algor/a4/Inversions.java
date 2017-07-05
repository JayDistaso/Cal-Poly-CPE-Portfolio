import java.util.ArrayList;
// Feel free to add any other imports
import java.lang.Math; 
import java.util.*;
public class Inversions
{
   //vars here
   
   public int invCounter(int[] ranking) //Takes array, sorts, and returns inversions
   {
      return MergeSort(ranking,0, ranking.length); //calls merge, which returns inversions
   }
   
   private int MergeSort(int [] arr, int low, int high) //recursive mergesort
   {
      if(low == (high -1))
      {
         return 0; //done here, return 0 to be added
      }
         int middle = (low + high) /2;
         int inver = 0; 
         inver += MergeSort(arr,low,middle); //sort left half, return inver and add
         inver += MergeSort(arr,middle, high); //sort right half, return inver
         inver += mergeParts(arr, low,middle,high); //merge the parts
         return inver; //returns number of inversions
   }
   private int mergeParts(int[] arr, int low, int middle, int high) //Merges parts + counts inversions
   {
      int[] temp = new int[arr.length];
      int inv =0;
      
      int i=low; //i is the element we are putting into sorted array
      int lowbound=low; //left list index
      int highbound=middle; //right list indedx
      for(i=low; i<high; i++)//step through all elements passed
      {
          if (highbound >= high || lowbound < middle && arr[lowbound] <= arr[highbound]) //if  right index is done then add from left list
          {// left index has yet to pass middle and the left side is less than right side 
              temp[i]  = arr[lowbound]; //Put element into temp array
              lowbound++; //increment left counter
          } 
          else 
          {
              inv = inv + (middle - lowbound); //increment counter by how many elements didnt make it
              temp[i]  = arr[highbound]; //insert into temp array
              highbound++; //increment right index
          } 
      }
      System.arraycopy(temp, low, arr, low, high - low); //Copies sorted temps into source, unsorted arr
      return inv; //returns new inversions
   }
   
}