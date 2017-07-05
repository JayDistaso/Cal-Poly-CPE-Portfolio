/**
 * DestTesterClass for CPE 349 Lab 1 Destructive Testing
 * 
 * @author Justin Distaso
 * CalPoly UserId: 008476165
 * 
 */

import java.util.ArrayList;
// Feel free to add any other imports
import java.lang.Math; // to Support log() operation
import java.util.*; //to support double to int, not sure if needed

public class DestructiveTest {
   /**
    * @return An ArrayList<Integer> that has these in the following order:
    * 0 : height of the ladder (input parameter)
    * 1 : actual highest safe run (input parameter)
    * 2 : Highest safe rung determined by this algorithm
    * 3 : Rung where the first test device broke
    * 4 : Rung where the second test device broke
    * 5 : Total number of drops required to find the highest safe rung*
    */
   public static ArrayList<Integer> findHighestSafeRung(int height, int safe) {
      ArrayList<Integer> result = new ArrayList<Integer>();
      
      result.add(height); // Adding Height of Ladder
      result.add(safe);   // Adding Safe Rung of Ladder
      
      int highestSafe = 1;
      int firstBroke = -1;
      int secondBroke = -1;
      boolean flag=true;
      int totalDrops=0;
      int lntest=((int)Math.log(height)); //equal to ln(height), will be the first spot we search
      int test = lntest; //this is the rung we are testing
      int mult = 1;
      if(height==1) //special case where there is only one rung
      {
         result.add(1);
         result.add(-1);
         result.add(-1);
         result.add(0);
         return result;
      }
        while(firstBroke == -1 && flag) //testing of first device 
        { 
         if(testRung(test,safe)==false) //if the test fails
         {
            firstBroke=test; //where it broke
         }
         else //if the test passes
         {
            highestSafe=test; //found a new safe spot 
         }
         totalDrops++; 
         if(highestSafe==height) //if we have tested all the rungs
         {
            flag=false; //break loop
         }
         mult++; 
         test = lntest*mult; //test moves on to the next multiple of lntest
         if(test>height)
         {
            test=height; //we can't test higher than the height of ladder, so we'll test the very top
         }
        }
      
       test=highestSafe+1; //Start testing from last known safe spot
      
       while(secondBroke == -1 && flag) //Testing of the second device
       {//

         if(test==firstBroke || highestSafe==height) //We have tested everything we need to at this point 
         {
            result.add(highestSafe);
            result.add(firstBroke);
            result.add(secondBroke);
            result.add(totalDrops);
            return result;
         }
         if(testRung(test,safe)==false) //if the test fails
         {
            secondBroke=test; //where it broke
            result.add(highestSafe);
            result.add(firstBroke);
            result.add(secondBroke);
            result.add(totalDrops);
            return result;
         }
         else //if the test passes
         {
            highestSafe=test; //found a new safe spot 
         }
         totalDrops++;
         test++; //Since we only have one device we only move up one at a time
       }
      result.add(highestSafe);
      result.add(firstBroke);
      result.add(secondBroke);
      result.add(totalDrops);
      return result;
   }
   public static boolean testRung(int testing, int givenSafe)
	{
		if(testing>givenSafe)
      { return false; }
     
      return true;
	}
}