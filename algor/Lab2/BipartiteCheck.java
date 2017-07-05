import java.util.*;
class BipartiteCheck
{//start of class
   public int[] color = new int[101]; //This array will hold verts colors, true=1 false=2
   public static boolean[] visited = new boolean[101]; //array of visited verts
   public GraphStart graph = new GraphStart();
   public boolean biflag=true;
   public boolean check(GraphStart bigraph){ //class that checks if input graph is Bipartite, the main function of this class
   
      graph = bigraph;
      
      for(int i=0; i<100;i++) //cycle through all vertices
      {
        if(graph.edges[i].size() != 0)
           {
           bfs(i);
           } //calls bfs for all nodes that exsist
                 //If it finds a new component it will paint it 1, and will paint all its family accordingly
      }

      return biflag; //returns if the graph is Bipartite

   }
   public void bfs(int rootNode)
   {
      // BFS uses Queue data structure

      LinkedList<Integer> q = new LinkedList<Integer>(); //a q for the bfs

      if(visited[rootNode] != true)
      {
         q.add(rootNode); //we only need to perform a bfs if we havent already visited this node (since otherwise it is connected)
         visited[rootNode] = true; //We have visited this node AND it is part of a new component
         color[rootNode] = 1; //The first node in a collection will be color 1 (arbitrary)
      }
      while(!q.isEmpty()) //while the q isnt empty
      {
         int n, child;
         n = (q.peek()).intValue(); //value of node we are looking at 
         child = getUnvisitedChildNode(n); //gets an unvisited child
         if (child != -1) //if there is an unvisited child node
         {
            visited[child] = true; //marks it visited
            paint(child,color[n]); //Attempts to paint the child the correct color, passes the color of parent doing the painting
            q.add(child); //adds child to q for bfs
         }
         else
         {
            q.remove(); //we are done with this node
         }
      }
   }
   
   public int getUnvisitedChildNode(int n) //grabs an unvisted child node for bfs
   {
      int j;
      for ( j = 0; j < graph.edges[n].size(); j++ ) //j will sweep through all the verts connected to node its checking (n)
      {
	       if ( !visited[graph.edges[n].get(j)] ) //checks if child has been visited 
          {
              return(graph.edges[n].get(j)); //if it hasnt then returns the value of node that ahsnt been visited
          }
      }
      return(-1);
   }
   
   public void paint(int child, int pColor)//takes the vert # of child and color parent is already painted
   {
      for(int i=0; i<graph.edges[child].size(); i++) //check all neighbors of child to see if they are color we want to paint child
      {
         if((color[graph.edges[child].get(i)] != 0) && (color[graph.edges[child].get(i)] != pColor)) 
         {
            biflag = false; //If an unpainted nodes neghbors arent unpainted or the same as its parent then it is not Bipartite
                            //Basically checks if a neighbor is already the color we are trying to paint it                         
         }
      }
      if(pColor==1) //paints the child the opposite of its parent
      {
         color[child]=2; 
      }
      else if(pColor==2)
      {
         color[child]=1;
      }
      else
      {
       System.out.printf("Something is wrong!"); //this should never happen, parents should aready be painted
      }
   }
   	
      void print_graph()	
      { 
		   for(int i=1;i<=graph.nvertices;i++)   
         {
		   	System.out.printf("%d: ",i);
            System.out.printf("The color value for this node is %d", color[i]);
			   System.out.printf("\n");
	   	}
			System.out.printf("\n");      
	   }

}