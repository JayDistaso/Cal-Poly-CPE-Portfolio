/**
 * CPE 349 Seq Align
 * Date: 2/28/16
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
public class EditDistance{

   public void editDistance(String path) throws FileNotFoundException
   {
      FileInputStream in = new FileInputStream(new File(path)); //get file
      Scanner sc = new Scanner(in); //make scanner object
      String seq1 = sc.nextLine(); //first sequence
      String seq2 = sc.nextLine(); //second sequence
      
      
      int l1 = seq1.length(); //lengths for easy access
      int l2 = seq2.length();
      
      int[][] ans = new int[l1+1][l2+1]; //matrix that holds answers for dynamic programming
     
      //TABLE FILL
      //Initial conditon for table, checks if last chars are equal to eachother
      ans[0][0] = 0;
      ans[1][1] = (seq1.charAt(0) == seq2.charAt(0)) ? 0 : 1; //start at 1,1 for easy seq ref numbers
      //[row][col]
      //fills up zero row with 2 higher each time from top corner, we know its +2
      //since one string is getting longer and longer than the other so we are inserting gaps
      for(int j = 1; j<=l2; j++)
      {
         ans[0][j] = 2 + ans[0][j-1];
      }
     //fills up zero col with 2 higher each time from top corner, we know its +2
      //since one string is getting longer and longer than the other so we are inserting gaps

      for(int i = 1; i<=l1; i++)
      {
         ans[i][0] = 2 + ans[i-1][0];
      }

      for(int i = 1; i<= l1; i++) //step through rows (outer)
      {
         for(int j = 1; j<= l2; j++) //step through cols (inner)
         {
         if(i==1 && j==1)//we already did this one
         {j=2;}
            //If chars are same this is 0+solution to one char less, if different its 1+solution to one char less
            //String starts at 0, table at 1
            int replace = ((seq1.charAt(i-1) == seq2.charAt(j-1)) ? 0 : 1) + ans[i-1][j-1]; 
            
            //The penalty for a gap is two, and you could insert gap in first or second sequence
            //You have to check optimal solution for having only processed one of the chars 
            int gap1 = 2+ans[i-1][j]; //gap in seq2 (delete)
            int gap2 = 2+ans[i][j-1]; //gap in seq1 (insert)
            //Find optimal solution by checking three possibilites, take min
            int min=Math.min(gap1,gap2);
            min=Math.min(min,replace);
            
            ans[i][j] = min; //fill in ans matrix
                 
         }
      }
      
      //DONE WITH TABLE FILL
      //PRINT ANSWER
      //Print ans, which is in [l1][l2] aka all of both seqs
   System.out.printf("Edit distance = %d\n", ans[l1][l2]);
      //Code to output table
   //where we are in ans array
   int p1 = l1;
   int p2 = l2;
   //where we are in strings
   int s1=l1-1;
   int s2=l2-1;
   String ans1 = "";//seq 1 output
   String ans2 = "";//seq2 output
   String ans3 = "";//number output
   while(p1>=1 && p2>=1)
   {//start while
      int up=ans[p1-1][p2]; //delete (gap in seq 2)
      int left=ans[p1][p2-1]; //insert (gap in seq 1)
      int diag=ans[p1-1][p2-1]; //replace
      if(diag<=up && diag <= left)
      {//diag is min
         ans1 = ans1.concat(Character.toString(seq1.charAt(s1)));
         ans2 = ans2.concat(Character.toString(seq2.charAt(s2)));
         if(seq1.charAt(s1) == seq2.charAt(s2))
         {
            ans3=ans3.concat("0");
         }
         else
         {
            ans3=ans3.concat("1");
         }
         s1--;
         s2--;
         p1--;
         p2--;     
      }
      else if(left<=up && left <= diag)
      {//left is min
            ans1 = ans1.concat("-");
            ans2 = ans2.concat(Character.toString(seq2.charAt(s2)));
            ans3=ans3.concat("2");
            s2--;
            p2--;
      }
      else if(up<=left && up<=diag)
      {
        //up is min
            ans2 = ans2.concat("-");
            ans1 = ans1.concat(Character.toString(seq1.charAt(s1)));
            ans3=ans3.concat("2");
            s1--;
            p1--;
      }
      else
      {//shouldnt go here
        System.out.printf("YOU DO NOT BELONG HERE");     
      }
       //if one string is done but other is not. Finish the other off
      if(s1<0 && s2 >= 0)
      {
        while(s2!=-1)
        {
            ans2 = ans2.concat(Character.toString(seq2.charAt(s2)));
            s2--;
        }
      }
      else if(s1>= 0 && s2<0)
      {
        while(s1!=-1)
        {
            ans1 = ans1.concat(Character.toString(seq1.charAt(s1)));
            s1--;
        }
      }
   }//end while
       //Output answer
   
   for(int i=ans1.length()-1; i>=0; i--)
   {
      System.out.printf("%c ", ans1.charAt(i));
      System.out.printf("%c ", ans2.charAt(i));
      System.out.printf("%c\n", ans3.charAt(i));
   }
    System.out.printf("Edit distance = %d\n", ans[l1][l2]);
   
      //DONE WITH TRACEBACK

   //Code to output table
   /*for(int i=0; i<l1;i++)
   {
      for(int j=0; j<l2; j++)
      {
         System.out.printf("%d ", ans[i][j]);
      }
      System.out.printf("\n");
   }*/
   }//END OF METHOD
}//end of class