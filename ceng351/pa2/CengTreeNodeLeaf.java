import java.util.ArrayList;

import org.w3c.dom.Node;

public class CengTreeNodeLeaf extends CengTreeNode
{
    private ArrayList<CengBook> books;
    // TODO: Any extra attributes

    public CengTreeNodeLeaf(CengTreeNode parent)
    {
        super(parent);

        // TODO: Extra initializations
        this.books = new ArrayList<CengBook>();
        this.type = CengNodeType.Leaf;
    }

    // GUI Methods - Do not modify
    public int bookCount()
    {
        return books.size();
    }
    public Integer bookKeyAtIndex(Integer index)
    {
        if(index >= this.bookCount()) {
            return -1;
        } else {
            CengBook book = this.books.get(index);

            return book.getBookID();
        }
    }

    // Extra Functions
    
    public CengBook getbookatindex(int in)
    {
    	return books.get(in);
    }
    
    
    @Override
    
    public void addtonode(CengBook book, CengTree tree)
    {

    	int i=0;
        while (i < this.books.size() && this.books.get(i).getBookID() < book.getBookID()) i++; 
        this.books.add(i, book);
        
        if(books.size()>2*this.order)
        {
        	CengTreeNodeLeaf yenileaf= new CengTreeNodeLeaf(getParent());
        	int sayi=0;
        	for(int j=this.order; j<this.bookCount(); j++)
        	{
        		yenileaf.books.add(this.books.get(j));
        		sayi++;
        	}
        	while(sayi>0)
        	{
        		sayi--;
        		books.remove((int) order);
        	}
        	
        	if(getParent()==null)
        	{
        		CengTreeNodeInternal yeniroot= new CengTreeNodeInternal(null);
        		
        		yeniroot.ilkcocuk(this);
        		yeniroot.addchildandkey(yenileaf, yenileaf.books.get(0).getBookID(), tree);
        		tree.root=yeniroot;
        		this.setParent(yeniroot);
        		yenileaf.setParent(yeniroot);
        		
        		
        		
        		
        	}
        	else
        	{
        		( (CengTreeNodeInternal) getParent()).addchildandkey(yenileaf, yenileaf.books.get(0).getBookID(), tree);
        		
        	}
        	
        	
        	
        	
        	
        }
        
        
    }
    
    @Override
    
    public void printnode(int tabsay)
    {
    	for(int j=0; j<tabsay; j++)
    	{
    		System.out.printf("\t");
    	}
    	System.out.println("<data>");
    	
    	for(int i=0;i<books.size();i++)
    	{
        	for(int j=0; j<tabsay; j++)
        	{
        		System.out.printf("\t");
        	}
        	System.out.print("<record>");
        	System.out.print(books.get(i).fullName());
        	System.out.println("</record>");


        	

    	}
    	
    	for(int j=0; j<tabsay; j++)
    	{
    		System.out.printf("\t");
    	}

    	
    	System.out.println("</data>");

    }

        

}
