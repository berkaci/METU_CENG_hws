import java.util.ArrayList;

public class CengTree
{
    public CengTreeNode root;
    // Any extra attributes...

    public CengTree(Integer order)
    {
        CengTreeNode.order = order;
        // TODO: Initialize the class
        this.root = new CengTreeNodeLeaf(null);

    }

    public void addBook(CengBook book)
    {
    	root.addtonode(book, this);
        // TODO: Insert Book to Tree
    }

    public ArrayList<CengTreeNode> searchBook(Integer bookID)
    {
        ArrayList<CengTreeNode> visited = new ArrayList<CengTreeNode>();
        CengTreeNode sim=root;
        while(true)
        {
        	visited.add(sim);
        	if(sim.type==CengNodeType.Leaf)
        	{
        		CengTreeNodeLeaf simLeaf=(CengTreeNodeLeaf) sim;
            	for(int i=0; i<simLeaf.bookCount(); i++)
            	{
            		int k=0;
            		if(simLeaf.bookKeyAtIndex(i)==bookID)
            		{
            			for(;k<visited.size()-1;k++)
            			{
            		    	for(int j=0; j<k; j++)
            		    	{
            		    		System.out.printf("\t");
            		    	}
            		    	
            		    	
            		    	System.out.println("<index>");
            		    	for(int ii=0; ii<((CengTreeNodeInternal)visited.get(k)).keyCount(); ii++)
            		    	{
            		        	for(int j=0; j<k; j++)
            		        	{
            		        		System.out.printf("\t");
            		        	}
            		        	System.out.println(((CengTreeNodeInternal)visited.get(k)).keyAtIndex(ii));

            		    	}
            		    	
            		    	for(int j=0; j<k; j++)
            		    	{
            		    		System.out.printf("\t");
            		    	}
            		    	System.out.println("</index>");


            			}
            			
                    	for(int j=0; j<k; j++)
                    	{
                    		System.out.printf("\t");
                    	}
                    	System.out.print("<record>");
                    	System.out.print(simLeaf.getbookatindex(i).fullName());
                    	System.out.println("</record>");

            			
            			return visited;
            		}
            	}
            	System.out.print("Could not find ");
            	System.out.print(bookID);
            	System.out.println(".");
            	return null;

        	}
        	else
        	{
        		CengTreeNodeInternal simInt=(CengTreeNodeInternal) sim;
            	int i=0;
                while (i < simInt.keyCount() && simInt.keyAtIndex(i) <= bookID) i++; 
                sim=simInt.getAllChildren().get(i);
        		
        		
        		
        	}
        }
        
        
        
        // TODO: Search within whole Tree, return visited nodes.
        // Return null if not found.


    }

    public void printTree()
    {
        // TODO: Print the whole tree to console
    	root.printnode(0);
    }

    // Any extra functions...
}
