/**
 * CPE 349 Assignment: Source Removal Topological Sort
 *
 * @author Justin Distaso
 */
// Do not use "package"
// Add any other important imports you may need
import java.util.ArrayList;
import java.util.*;

public class TopSorter {

public static GraphStart graph = new GraphStart(); //The graph object
public static LinkedList<Integer> q = new LinkedList<Integer>(); //a q for our class
public static ArrayList<Integer> ret = new ArrayList<Integer>(); //what we will be returning

   public static ArrayList<Integer> topSortGenerator(String s) 
   {
      // s is the filename for the input graph
      GraphStart graph = new GraphStart();
      try {
               graph.readfile_graph(s); //graph for this class
          } 
      catch (Exception e)
       {
         System.out.printf("FAILED TO READ FILE\n");
       }
       int temp;
       int vertices=graph.nvertices;
       boolean flag=true;
       while(flag)
       {  
         for(int i=0; i<200; i++)
          {
            if(graph.indegree[i] == 0)//If the indegree is 0
            {
               q.add(i); //Add the vert to the q
            }
          }
          if(q.size() == 0) //If we went through all verts and none had indegree=0
          {
            if(vertices != 0) //Not a DAG case
            {
              for(int j=0; j<vertices; j++)
              {
                ret.add(-1);
              }
              if(ret.get(0) == -1)//Case with no partial search
              {
               ret.clear();
              }  
            } 
            flag=false; //break out of loop
          }
          while(q.size() != 0) //Remove all elements from q
          {                    //That is all the elements with indegree = 0
               temp = q.removeFirst(); //The element we are working on, remove it from q
               ret.add(temp); //add it to return list
               while(graph.edges[temp].size() != 0)
               {
                   graph.remove_edge(temp,(graph.edges[temp].get(0)).intValue()); //Removes all edges from vert
               }//The indegrees are handled in GraphStart
               vertices--; //one less vertice
               graph.indegree[temp]=-1;
          }
       }
      return ret;
   }
}
