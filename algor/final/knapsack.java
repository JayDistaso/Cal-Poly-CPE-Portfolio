/**
 * CPE 349 knapsack FINAL PROJECT
 * Date: 3/3/16
 * @author Justin Distaso
 */

//imports
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.util.Scanner;
import java.io.*;
import java.util.ArrayList;
import java.util.Collections;
import java.util.*;

public class knapsack
{//start class
   public static int numItems; //number of items in knapsack
   public static int capacity; //capacity of knpasack
   public static ArrayList<Pair> list = new ArrayList<Pair>(); //list to hold items
   public static ArrayList<Integer> bestItems = new ArrayList<Integer>(); //list to hold best items found in branch
   public static int checked = 0; //fun statistic for number of bounds checked

   //method for reading files and making list with all the items including values, capacity, etc.
   static void init(String filename) throws FileNotFoundException
   {      
      FileInputStream in = new FileInputStream(new File(filename)); //get file
      Scanner sc = new Scanner(in); //make scanner object
      numItems = sc.nextInt(); //Number of items
      Pair pair; //Pair is a custom class that holds each item
      pair=new Pair(0,0,0);//placeholder item so index can start at 1
      list.add(pair); //list will hold all the items
      for(int i=0; i<numItems; i++) //Fill knapsack from file
      {
         int index = sc.nextInt();
         int value = sc.nextInt();
         int weight = sc.nextInt();
         pair = new Pair(index,value,weight);
         list.add(pair);
      }
      capacity = sc.nextInt(); //capacity
   }
   
   //this class will help us keep track of items after sorting
   public static class Pair implements Comparable<Pair> 
   { 
    public int weight; 
    public int value;
    public int index; //to keep track of index before we sort it
    public ArrayList<Integer> items = new ArrayList<Integer>(); //list to hold items in branch
    public int bound; //used in Branch+bound, to hold this nodes bound
    
    //Basic Constructor
    public Pair(int index, int value, int weight) {
        this.index = index;
        this.value = value;
        this.weight = weight;
    }
    
    //Copy Constructor
    public Pair(Pair p) {//copies completely, copy of list, not pointer!
        this.index = p.index;
        this.value = p.value;
        this.weight = p.weight;
        this.items = new ArrayList<Integer>(p.items);
        this.bound = p.bound;
    }
    
    //@Override
    //Sorted by value to weight ratio for greedy/best sort
    public int compareTo(Pair other) {
         int ratioA;
         int ratioB;
         if(this.weight == 0)
         {//avoid dividing by zero
            ratioA = Integer.MAX_VALUE; //we always want 0 entry to be first
         }
         else
         {
           ratioA = this.value / this.weight;
         }
         if(other.weight == 0)
         {//avoid dividing by zero
            ratioB = Integer.MAX_VALUE; //we always want 0 entry to be first
         }
         else
         {
            ratioB = other.value / other.weight;
         }
        return Integer.valueOf(ratioB).compareTo(ratioA);
    } 
   }//end of pair class
   
   static public void main(String[] args) throws FileNotFoundException	
	{ //This is main method (driver)
      //args[1] = 1 to run algor 1, else do not run,
      //args[2] = 1 to run algor 2, else do not run, 
      //etc
      //args[5] = how many seconds to run Branch+Bound
        
        String filename = args[0];
        init(filename); //inits list from file
        long st;
        if(Integer.parseInt(args[1]) == 1)
        {  
         System.out.println("Trying Brute Force... ");
         System.out.println();
         st=System.currentTimeMillis();
         bruteForce();
         System.out.printf("Time elapsed = %d\n", (System.currentTimeMillis()-st));
         System.out.println();
         System.out.println();
        }
        
        Collections.sort(list); //the rest of these algors would like this list sorted
        //by value/weight ratio
        
        if(Integer.parseInt(args[2]) == 1)
        {
         System.out.println("Trying Greedy Approach... ");
         System.out.println();
         st=System.currentTimeMillis();
         greedy();
         System.out.printf("Time elapsed = %d\n", (System.currentTimeMillis()-st));  
        }
        if(Integer.parseInt(args[3]) == 1)
        {
         System.out.printf("\n");
         System.out.println("Trying Dynamic Programming Approach... ");
         System.out.println();
         
         st=System.currentTimeMillis();
         dynamic();
         System.out.printf("Time elapsed = %d\n", (System.currentTimeMillis()-st));
        }
        
        if(Integer.parseInt(args[4]) == 1)
        {
         System.out.println("Trying Branch and Bound... ");
         System.out.println();
         st=System.currentTimeMillis();
         branchbound(Integer.parseInt(args[5]));
         System.out.printf("Time elapsed = %d\n", (System.currentTimeMillis()-st));
        }
     }
     
   static public void bruteForce()//start bruteforce
   {
      ArrayList<String> codes = getGrayCode(numItems); //Calls method to generate all gray codes
      int locweight =0;
      int locvalue=0;
      int maxweight=0;
      int maxvalue=0;
      String maxcode =""; //stores best code (item combination)
      for(int i=0; i<(int)Math.pow(2, numItems); i++)//will try all n^2 combinations
      {
         String code= codes.get(i);//get one code at a time
         for(int j=1; j<numItems+1; j++)//scan through items
         {
            if(Character.getNumericValue(code.charAt(j-1)) == 1) //item is in this combo
            {
               locweight+=list.get(j).weight;
               locvalue+=list.get(j).value;
            }
         }
          if(locvalue>maxvalue && locweight <= capacity) //new max
            {
               maxvalue = locvalue; 
               maxweight = locweight;
               maxcode = new String(code);    
            }
            locvalue=0; //reset variables
            locweight=0;
      }
      System.out.printf("Using Brute force the best feasible solution found:  Value: %d, Weight: %d\n", maxvalue, maxweight);
      System.out.println("Items: ");
      for(int i = 1; i <= numItems; i++)
      {
         if(Character.getNumericValue(maxcode.charAt(i-1)) == 1)
         {
            System.out.printf("%d, ", i);
         }
      }
   }
   
   public static ArrayList<String> getGrayCode(int n) //Makes graycode permutations for size n, from CombObjects lab
   {
      int math = (int)Math.pow(2, n);
       ArrayList<String> arr = new ArrayList<String>();
       arr.ensureCapacity(math);
       if(n<=0) //If empty
       {
          return arr;
       }
       arr.add("0");
       arr.add("1");
       int i, j;
       for(i=2; i < (Math.pow(2, n)); i=i*2) //doubles every time because the number of codes coubles on each iteration
       {
         for(j = i-1; j>=0; j--) //Copies all old codes to end
         {
            arr.add(arr.get(j));
         }
         // append 0 to the first half of duplicate codes
         for (j = 0; j < i; j++)
         {
            arr.set(j,"0" + arr.get(j));
         }
         // append 1 to the second half of duplicate codes
         for (j = i ; j < 2*i ; j++)
         {
            arr.set(j,"1" + arr.get(j));
         }
       }
       return arr;
   }
    
    static public void greedy()//start greedy
    {//List is already sorted
       int value = 0;
       int weight = 0;
       ArrayList<Integer> items = new ArrayList<Integer>(); //Stores items in answer
       int i = 0; //start at start of list
       while(i<=numItems)
       {
         if(list.get(i).weight + weight <= capacity) //if item fits
         {
            items.add(list.get(i).index); //add it to answer
            value += list.get(i).value;
            weight += list.get(i).weight;
         }
         i++;//move to next item
       }
       System.out.printf("Greedy solution (not necessarily optimal):   Value: %d, Weight: %d\n", value, weight);
       Collections.sort(items);
       System.out.printf("Items: ");
       for(i=1; i<items.size(); i++)
       {
         System.out.printf("%d, ", items.get(i)); //output, skip first one since its the zero item
       }
       System.out.printf("\n");
    }//end greedy
    
    static public void dynamic()
    {//Code adapted from https://en.wikipedia.org/wiki/Knapsack_problem#0.2F1_knapsack_problem
         int [][] ans = new int[numItems+1][capacity+1]; //2d array to hold partial solutions
         int[][] sol = new int[numItems+1][capacity+1]; //array to hold which choice we pick
         // 0 row automatically inilaized to zero
         
         //Fill Table
         for(int i=1; i<=numItems; i++)//Go through all items 
         {
            for(int j=1; j<=capacity; j++)//Go through all weights
            {
               if(list.get(i-1).weight <= j && (list.get(i-1).value + ans[i-1][j-list.get(i-1).weight] > ans[i-1][j]))//if it fits AND its a better value
               {
                     ans[i][j] = list.get(i-1).value + ans[i-1][j-list.get(i-1).weight];
                     sol[i][j] = 1; //we picked it 
               }
               else//if it doesnt fit / is a worse value
               {
                  ans[i][j] = ans[i-1][j];
                  sol[i][j] = -1;//we didnt pick it
               }
            }
         }             
         
         //TRACEBACK   
         //traceback adapted from http://www.programminglogic.com/knapsack-problem-dynamic-programming-algorithm/
         int item = numItems;
         int size = capacity;
         ArrayList<Integer> items = new ArrayList<Integer>();
         while (item>0)
         {
            if (sol[item][size]==1)
            {
               items.add(list.get(item-1).index); //holds which items we picked
               item--;
               size -= list.get(item).weight;
            }
            else
            {
               item--;
            }
         }
         int totweight = capacity-size; //calculated above
         Collections.sort(items); //Sort indexes for output
         
         //OUTPUT
         System.out.printf("Dynamic Programming solution: Value %d, Weight %d\n", ans[numItems][capacity], totweight);
         //Output items picked found in traceback above
         for(int i=0; i<items.size(); i++)
         {
            System.out.printf("%d, ", items.get(i));
         }
         System.out.printf("\n");
         System.out.printf("\n");
     }//end dynamic programming solution.
     
     public static void branchbound(int seconds)
     //Algor adapted from 
     //https://books.google.com/books?id=DAorddWEgl0C&lpg=PA233&pg=PA239#v=onepage&q&f=true
     {
         PriorityQueue<Pair> q = new PriorityQueue<Pair>(); //PQ for best first search
         int maxvalue =0;
         int finalweight=0;
         Pair v = new Pair(0,0,0);//init root of tree
         Pair u = new Pair(v);
         v.bound = bound(v); //get init bound
         q.add(new Pair(v)); //being overly cautious with pointers on advice of
         //http://stackoverflow.com/questions/7448141/implementing-branch-and-bound-for-knapsack
         //always generate new Pair, not point to old one
         boolean time = true; //this will change to false if we time out
         long endtime = System.currentTimeMillis() + seconds*(1000); //args[5] is how many second to run
         long realtime;
         while(!q.isEmpty() && time)//while q is empty
         {
            v = new Pair(q.remove());//V is node we are examining
            //u is temp node, first we check adding item
            if(v.bound > maxvalue) //if not why bother?
            {
               u.index = v.index+1; //set u to be child of v
               u.weight = v.weight + list.get(u.index).weight; //get next weight
               u.value = v.value + list.get(u.index).value; //get next value
               u.items = new ArrayList<Integer>(v.items); //copy items from v to u
               u.items.add(list.get(u.index).index);//get real index
             if(u.weight <= capacity && u.value > maxvalue)
             {//SECTION FOR UPDATING NEW BEST ANSWER
                maxvalue = u.value;
                finalweight = u.weight;
                bestItems = new ArrayList<Integer>(u.items); //this is the best set we have so far
             }
              u.bound = bound(u);
               if(u.bound > maxvalue)//if bound is above max value so far, could be better
               {
                 q.add(new Pair(u));
               }
               //now we check how it would be to not add item
                u.weight = v.weight;
                u.value = v.value;
                u.items = new ArrayList<Integer>(v.items); //copy items from v to u
                u.bound = bound(u);
               if(bound(u) > maxvalue)
               {
                q.add(new Pair(u));
               }
               realtime = System.currentTimeMillis(); //See what time it is
               if(realtime > endtime) //If its been too long its time to stop checking
               {
                  System.out.printf("Process Timed out after %d seconds, printing current best result (not necessarily optimal)\n", seconds);
                  time = false;
               }  
            }
         }
         
         //OUTPUT
         System.out.printf("Bounds checked  = %d\n", checked); //See how many bounds (for fun)
         System.out.printf("Using Branch and Bound the best feasible solution found: Value %d, Weight %d\n", maxvalue, finalweight);
         Collections.sort(bestItems);//Holds items picked
         while(!bestItems.isEmpty())
         {
            System.out.printf("%d, ",bestItems.remove(0));
         }
         System.out.printf("\n");
     }//end of branch
     
     public static int bound(Pair u)//Method for calculating bound for branch
     {//adapted from http://www.micsymposium.org/mics_2005/papers/paper102.pdf   
         checked++; //statistic
         if(u.weight >= capacity)//if combo wont fit no need to check bound
         {
            return 0;
         }
         int j; 
         int result = u.value;
            j = u.index+1;
            double frac;
            if(j <= numItems)//use fractional knapsack since bound(fractional)>=bound(!fractional)
            {
               frac = ((capacity - u.weight) * list.get(j).value) / list.get(j).weight;
               result += (int)frac + 1; //to not accidently under estimate upper bound from cleaving double to int
            }
            return result;
     }
}