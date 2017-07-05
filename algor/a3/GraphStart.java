import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.util.Scanner;
import java.io.*;
import java.util.ArrayList;
import java.util.Collections;
import java.util.*;

// This is a very simple graph class,
// May get a compiler error due to use of array of ArrayLists

class GraphStart
{
	static final int MAXV = 200;
	static final int MAXDEGREE = 50;
	public boolean directed;
	public ArrayList<Integer>[] edges = new ArrayList[MAXV+1];
	public int degree[] = new int [MAXV+1];
	public int nvertices;
	public int nedges;
   public int indegree[] = new int [MAXV+1]; //A count of how many things are pointing to node

	GraphStart()   {                     // constructor
		nvertices = nedges = 0;
		for(int i=0;i<=MAXV;i++)  {
			degree[i] = 0;
         Arrays.fill(indegree, -1); //Fills all indegrees as -1 (DNE) so we won't get confused later
         edges[i] = new ArrayList<Integer>();
      }   
	}
		
   void readfile_graph(String filename) throws FileNotFoundException     {
      int x,y;
      FileInputStream in = new FileInputStream(new File(filename));
      Scanner sc = new Scanner(in);
      directed =  (  1 == sc.nextInt()  );      // 1 directed, anything else undirected
   	nvertices = sc.nextInt();
		int m = sc.nextInt();                     // m is the number of edges in the file
		for(int i=1;i<=m;i++)	{
			x=sc.nextInt();
			y=sc.nextInt();
			insert_edge(x,y,directed);
		}  
		                           // order edges (book convention) to ease debugging
		for(int i=1;i<=nvertices;i++)	{
			Collections.sort(edges[i]);
		}  
	}
  
	void insert_edge(int x, int y, boolean directed)	{
		                         // not worrying about capacity		         
      edges[x].add(y);
      degree[x]++; //Counts how many things our node is pointing to
      if(indegree[x] == -1)
      {
         indegree[x]=0; //X is present and as fas as we know it has 0 indegrees
      }
      if(indegree[y] == -1)
      {
         indegree[y] = 1; //The first time we have to jump from -1 to 1
      }
      else
      {
         indegree[y]++; //A new thing is pointing at this node, increment
      }
      
		if (!directed)  {       // adding "mirror" edge since not directed
         edges[y].add(x);
         degree[y]++;
			nedges++;         
      }
	}

   void remove_edge(int x, int y)  {
		if(degree[x]<0)
			System.out.println("Warning: no edge --" + x + ", " + y);
		if(edges[x].remove((Integer)y)) //added check to see if element is present
      {//won't do anything if you remove an element that DNE
		   degree[x]--;
         indegree[y]--; //removes indegree
      }
	}

	
	void print_graph()	{
      if (directed) 
         System.out.printf("Directed Graph\n"); 
      else 
         System.out.printf("Undirected Graph\n"); 
		for(int i=1;i<=nvertices;i++)   {
			System.out.printf("%d: ",i);
			for(int j=0;j<=degree[i]-1;j++)
				System.out.printf(" %d",edges[i].get(j));
			System.out.printf("\n");
		}
	}
   void print_indegrees()	{ //For helpful debugging of indegree system
    System.out.printf("Indegrees\n"); 
		for(int i=1;i<=nvertices;i++)   {
			System.out.printf("%d: ",i);
			System.out.printf(" %d",indegree[i]);
			System.out.printf("\n");
		}
	}
}