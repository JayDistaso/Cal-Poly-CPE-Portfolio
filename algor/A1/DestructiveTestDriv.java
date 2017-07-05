import java.util.ArrayList;
// Feel free to add any other imports
import java.lang.Math; // to Support log() operation
import java.util.*;
public class DestructiveTestDriv {
   public static void main(String[] args)
   {
      ArrayList<Integer> a = new ArrayList<Integer>();
      int one = Integer.parseInt(args[0]);
      int two = Integer.parseInt(args[1]);
      DestructiveTest box = new DestructiveTest();
      a = box.findHighestSafeRung(one,two);
      System.out.println("The height of the ladder is:");
      System.out.println(a.get(0));
      System.out.println("The actual highest safe rung is:");
      System.out.println(a.get(1));
      System.out.println("The highest safe rung determined by this algorithm is:");
      System.out.println(a.get(2));
      System.out.println("The rung where the first device broke (-1 if this device never broke):");
      System.out.println(a.get(3));
      System.out.println("The rung where the second device broke (-1 if this device never broke):");
      System.out.println(a.get(4));
      System.out.println("The total number of drops required to find highest safe rung by this algorithm was:");
      System.out.println(a.get(5));     
   }
 }