/**
 * CPE 349 Lab 6: Dynamic Programming with rod cutting
 * Date: 2/11/16
 * @author Justin Distaso
 */

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.util.Scanner;
import java.io.*;
import java.util.ArrayList;
import java.util.Collections;
import java.util.*;
public class MaxValRod{//

   public static void MaxValRod(String path) throws FileNotFoundException
   {
      FileInputStream in = new FileInputStream(new File(path)); //get file
      Scanner sc = new Scanner(in);
      int numProbs = sc.nextInt();
      int length;
      int[] probarr;
      int i=0;
      for(i = 1; i<numProbs+1; i++) //one for each problem
      {
         length = sc.nextInt(); //gets new length of rod
         probarr = new int[length+1];
         probarr[0] = 0;
         for(int ii =1; ii<=length; ii++) //fills array for this problem
         {
            probarr[ii] = sc.nextInt();
         }
         System.out.printf("Case %d:\n", i);
         solveMaxValRod(length,probarr); //solves problem
      }
   }
   private static void solveMaxValRod(int length, int[] costArr)
   {
      int i=1;
      int[] MaxValArr = new int[length+1];
      MaxValArr[0] = 0;
      int[][] Aarr = new int[length+1][length+1];//answer of steps array
      //for solution n, with x amounts of each length l [n][l]=x
      int temp;
      int max = 0;
      while(i <= length)
      {
        temp=costArr[i]; //check cost not cutting
            if(max<temp)
            {
               max=temp; //update new max if needed
               Aarr[i][i] = 1;//optimal solution to length i is 1 of length i
            }
         for(int x=1; x<i; x++) //total compares is 1+(n-1) = n
         {
            temp = costArr[i-x] + MaxValArr[x]; //price(n-1) + M(1), price(n-2)+M(2)... etc
            if(max<temp)
            {
               max=temp; //see if that is new max
               for(int iii = 0; iii<length+1; iii++)
               {
                  Aarr[i][iii] = Aarr[x][iii]; //The answer to this problem is same as last one
               }//plus one more addition
               Aarr[i][i-x] += 1;
            }
         }
         MaxValArr[i] = max;
         System.out.printf("total for length %d    =  %d\n", i, max);
         max = 0;
         i++;
      }
      System.out.printf("\n");
      for(int y=0; y<= length; y++) //This will print the answer
      {
         if(!(Aarr[length][y] == 0)) //If the number of rods in solution for this length is NOT 0
         {
            System.out.printf(" # rods of length %d   = %d\n", y,Aarr[length][y]);
         }
      }
      System.out.printf("\n");
   }
}