import java.util.ArrayList;
// Feel free to add any other imports
import java.lang.Math; // to Support log() operation
import java.util.*;
public class CombDriv {
   public static void main(String[] args)
   {  
      CombObjects box = new CombObjects();
      String testString = args[0];
      int testNum = Integer.parseInt(args[1]);
      ArrayList<String> arr = new ArrayList<String>();
      ArrayList<String> arr1 = new ArrayList<String>();
      ArrayList<String> arr2 = new ArrayList<String>();

      arr = box.getGrayCode(testNum);
      arr1 = box.getLexPerm(testString);
      arr2 = box.getMinChgPerm(testString);
      
      for(int i=0; i<arr.size(); i++)
      {
         System.out.printf("Gray code: %s\n" , arr.get(i));
      }
      for(int i=0; i<arr1.size(); i++)
      {
         System.out.printf("Lex Perm: %s\n" , arr.get(i));
      }
      for(int i=0; i<arr2.size(); i++)
      {
         System.out.printf("Min Change Perm: %s\n" , arr.get(i));
      }      
   }
 }