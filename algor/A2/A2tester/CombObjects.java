/*
By Justin Distaso for CPE 349 due 1/20/16
*/

import java.util.*;
class CombObjects
{//start of class  
   public ArrayList<String> getGrayCode(int n) //Makes graycode permutations for size n
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
   public ArrayList<String> getLexPerm (String str) //Returns all permutations in lex order
   {
       ArrayList<String> arr = new ArrayList<String>();
       if(str == null)
       {
          return arr;
       }
       arr = permutation(str);
       Collections.sort(arr.subList(0,arr.size()));
       return arr;  
   }
   public ArrayList<String> permutation(String str) //This one gets called recursivly
   {
      ArrayList<String> ret = new ArrayList<String>();
      if(str.length() == 1) {ret.add(str);}
      else
      {
       String last = str.substring(str.length() -1); //The last char
       String rest = str.substring(0,str.length() -1); //All but the last char
       ret = merge(permutation(rest), last); //merges the cleaved char with the rest, after the rest has been ran through this operation
      }
      return ret;
   }
   public ArrayList<String> merge(ArrayList<String> rest, String last) 
   {
       ArrayList<String> ret = new ArrayList<String>();
        for (String s : rest) 
        { // Loop through all the string in the list
            for (int i = 0; i <= s.length(); ++i) 
            {
                String perms = new StringBuffer(s).insert(i, last).toString(); // For each string, insert the last character to all possible postions
                ret.add(perms);                                             // and add them to the new list
            }
        }
      return ret;
   }
   public ArrayList<String> getMinChgPerm (String str)
   {
       ArrayList<String> ret = new ArrayList<String>();
       ArrayList<String> perms = new ArrayList<String>();
       if(str == null)
       {
          return ret;
       }
       String last = str.substring(str.length() -1); //last char
       String rest = str.substring(0,str.length() -1); //str with last char cleaved
       perms = getMinChgPermHelper(rest); //gets all permutations of string without last char
       String uncut;
       String recut;
       boolean rtl = true; //This will set direction of insertion (right to left or not)
       for(int i = 0; i <= perms.size() - 1 ; i++) //Outer loop, iterates over permutations of rest
       {
         if(rtl)
         {  
            for(int j=str.length() -1 ; j>=0 ; j--) //iterates once for num of charters, this is also the number of chars added before adding removed
            {
               uncut = perms.get(i); //the permutation we are working with
               if(j==0)
               {
                  recut = ""; //the last one is letter + perm
               }
               else
               {   
                   recut = uncut.substring(0,j); //the others are part + removed + rest, this is the first part
                   uncut = uncut.substring(j); // This is the rest
               }                
               recut+=last; //attach the letter
               recut+=uncut; //attach the rest
               ret.add(recut); //add it to the list
               recut=""; //clear recut for next iteration 
            } //done with that permutation 
            rtl=false; //next time around we will go left to right
         }
         else
         {//This else block is for going left to right
            for(int j=0 ; j<=str.length() -1 ; j++) //iterates once for num of charters, this is also the number of chars added before adding removed
            {
               uncut = perms.get(i); //the permutation we are working with
               if(j==0)
               {
                  recut = ""; //the last one is letter + perm
               }
               else
               {   
                   recut = uncut.substring(0,j); //the others are part + removed + rest, this is the first part
                   uncut = uncut.substring(j); // This is the rest
                   
               }
               recut+=last; //attach the letter
               recut+=uncut; //attach the rest
               ret.add(recut); //add it to the list
               recut=""; //clear recut for next iteration 
            } //done with that permutation 
            rtl=true;
         }
       } //end of for loop
       return ret;
   }
   
  public ArrayList<String> getMinChgPermHelper (String str)
   {
       ArrayList<String> ret = new ArrayList<String>();
       ArrayList<String> perms = new ArrayList<String>();
       if(str == null)
       {
          return ret;
       }
       String last = str.substring(str.length() -1); //last char
       String rest = str.substring(0,str.length() -1); //str with last char cleaved
       perms = getLexPerm(rest); //gets all permutations of string without last char
       String uncut;
       String recut;
       boolean rtl = true; //This will set direction of insertion (right to left or not)
       for(int i = 0; i <= perms.size() - 1 ; i++) //Outer loop, iterates over permutations of rest
       {
         if(rtl)
         {  
            for(int j=str.length() -1 ; j>=0 ; j--) //iterates once for num of charters, this is also the number of chars added before adding removed
            {
               uncut = perms.get(i); //the permutation we are working with
               if(j==0)
               {
                  recut = ""; //the last one is letter + perm
               }
               else
               {   
                   recut = uncut.substring(0,j); //the others are part + removed + rest, this is the first part
                   uncut = uncut.substring(j); // This is the rest
               }                
               recut+=last; //attach the letter
               recut+=uncut; //attach the rest
               ret.add(recut); //add it to the list
               recut=""; //clear recut for next iteration 
            } //done with that permutation 
            rtl=false; //next time around we will go left to right
         }
         else
         {//This else block is for going left to right
            for(int j=0 ; j<=str.length() -1 ; j++) //iterates once for num of charters, this is also the number of chars added before adding removed
            {
               uncut = perms.get(i); //the permutation we are working with
               if(j==0)
               {
                  recut = ""; //the last one is letter + perm
               }
               else
               {   
                   recut = uncut.substring(0,j); //the others are part + removed + rest, this is the first part
                   uncut = uncut.substring(j); // This is the rest
                   
               }
               recut+=last; //attach the letter
               recut+=uncut; //attach the rest
               ret.add(recut); //add it to the list
               recut=""; //clear recut for next iteration 
            } //done with that permutation 
            rtl=true;
         }
       } //end of for loop
       return ret;
   }
}//end of class