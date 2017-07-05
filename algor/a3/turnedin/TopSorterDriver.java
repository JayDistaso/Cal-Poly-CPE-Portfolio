import java.util.ArrayList;
// Feel free to add any other imports
import java.lang.Math; 
import java.util.*;
public class TopSorterDriver {
public static void main(String[] args)
   {
      TopSorter box = new TopSorter();
      ArrayList<Integer> list = box.topSortGenerator(args[0]);
      for(int i=0; i<list.size(); i++)
      {
         System.out.printf("%d",list.get(i));
      }
   }
}