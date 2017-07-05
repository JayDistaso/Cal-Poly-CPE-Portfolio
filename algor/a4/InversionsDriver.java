import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.util.Scanner;
import java.io.*;
import java.util.ArrayList;
import java.util.Collections;
import java.util.*;
public class InversionsDriver
{
   public static void main(String[] args) throws FileNotFoundException
   {
      FileInputStream in = new FileInputStream(new File(args[0])); //get file
      Scanner sc = new Scanner(in);
      int[] arr = new int[1000]; //array we will pass to inversion counter
      int i=0;
      while (sc.hasNextInt()) //Reads in Numbers 
      {
         arr[i] = sc.nextInt();
         i++;
      }
      int[] array = new int[i]; //New array that fits exactly the number of elements
      for(int j=0; j<i; j++)
      {
         array[j] = arr[j]; //copies ints over, cuts out default 0's
      }
      Inversions box = new Inversions(); //object
      int invers = box.invCounter(array); //Should return number of inversions
      System.out.printf("The number of inversions returned is %d", invers);
   }
}