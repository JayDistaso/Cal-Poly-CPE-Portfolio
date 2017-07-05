import java.util.ArrayList;
// Feel free to add any other imports
import java.lang.Math; 
import java.util.*;
public class WgtIntSchedulerDriver {
public static void main(String[] args)
   {
     WgtIntScheduler box = new WgtIntScheduler();

      int[] stime ={4,3,2,10,7};
      int[] ftime = {7,10,6,13,9};
      int[] weight = {6,6,5,2,8};
       
      int[] list = box.getOptSet(stime,ftime,weight);
      for(int i=0; i<list.length; i++)
      {
         System.out.printf("%d ",list[i]);
      }
      System.out.printf("\n");

      int[] stime2 ={1,1,1,1,1};
      int[] ftime2 = {2,2,2,2,2};
      int[] weight2 = {9,99,999,0,-1};
       
      list = box.getOptSet(stime2,ftime2,weight2);
      for(int i=0; i<list.length; i++)
      {
         System.out.printf("%d ",list[i]);
      }
   }
}