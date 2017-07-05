import java.util.ArrayList;
// Feel free to add any other imports
import java.lang.Math; // to Support log() operation
import java.util.*;
public class TestMyHeap {
public static void main(String[] args)
   {
      MyHeap box =new MyHeap();
      box.insert(1);
      box.insert(3);
      box.insert(2);
      box.insert(4);
      box.insert(5);
      box.printHeap();
        System.out.printf("getHeapSize() = %d\n", box.getHeapSize());
      System.out.printf("find max = %d\n", box.findMax());
      System.out.printf("delete max = %d\n", box.deleteMax());
      System.out.printf("isfull = %b\n", box.isFull());
      System.out.printf("is empty = %b\n", box.isEmpty());
      System.out.printf("getHeapCap = %d\n", box.getHeapCap());
      System.out.printf("getHeapSize() = %d\n", box.getHeapSize());
      box.printHeap();
   }
}
