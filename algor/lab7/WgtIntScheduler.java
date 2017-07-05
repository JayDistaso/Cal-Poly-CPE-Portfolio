/**
 * CPE 349 Lab 7: Knapsack With Unlimited Copies and Weighted Interval Scheduling
 * Date: 2/18/16
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
public class WgtIntScheduler{

   static int[] getOptSet (int[] stime, int[] ftime, int[] weight)
   {
      int numJobs = stime.length; //num of jobs
      Pair pair = new Pair(0,0,0,0); //base case
      ArrayList<Pair> list = new ArrayList<Pair>(); //list to hold jobs
      list.add(pair); //adds 0 to job table
      for(int i=0; i<numJobs; i++) //copy values into jobs table
      {
         pair= new Pair(ftime[i],stime[i],weight[i],i+1); //makes new object of job and adds to list
         list.add(pair);
      }
      Collections.sort(list); //sort by finish time
      //compute p
      int p[] = new int[numJobs+1]; //array to hold p[]
      int m[] = new int[numJobs+1]; //array to holdanswer
      m[0]=0; //base cases
      p[0]=0;
      for(int i=1; i<=numJobs; i++)//p index we are checking for
      {
         loop:
         {
         for(int j=numJobs; j>0; j--)
         {
            if(list.get(i).stime >= list.get(j).ftime)//if start time is later than jobs before finish (aka compatable)
            {
               p[i]=j;
               break loop;//ret j
            }
         }
         p[i]=0; //no compatable jobs
         }
      }
      for(int j=1; j<=numJobs; j++)//this gets optimal solution
      {
         m[j]=Math.max(list.get(j).weight + m[p[j]] , m[j-1]);
      }

      //traceback
      ArrayList<Integer> ret = new ArrayList<Integer>(numJobs);
      ret = traceback(list,m,p,numJobs,ret);
      int[] ans = new int[ret.size()];
      for(int i=0;i<ret.size();i++)
      {
         ans[i]=ret.get(i); //put arraylist into array
      }
     Arrays.sort(ans); //sort the jobs we use
      return ans;
   }
   public static ArrayList<Integer> traceback(ArrayList<Pair> list, int[] m, int[] p, int j, ArrayList<Integer> ret) //recursive traceback
   {//all details are passed through fucntion
    if(j==0)
    {
    }
    else if(list.get(j).weight + m[p[j]] > m[j-1])
    {
      ret.add(list.get(j).Oindex); //adds answer to array, have to get presorted index
      traceback(list,m,p,p[j],ret); //where we need to check now
    }
    else
    {
      traceback(list,m,p,j-1,ret);
    }
    return ret;
   }
  //class to keep track of jobs
   public static class Pair implements Comparable<Pair> 
   {
    
    public final int stime;
    public final int ftime;
    public final int weight;
    public final int Oindex; //to keep track of index before we sort it

    public Pair(int ftime, int stime, int weight, int Oindex) {
        this.stime = stime;
        this.ftime = ftime;
        this.weight = weight;
        this.Oindex = Oindex;
    }

    //@Override
    public int compareTo(Pair other) {
        return Integer.valueOf(this.ftime).compareTo(other.ftime);
    }
   }
   
}