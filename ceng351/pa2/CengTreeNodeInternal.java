import java.util.ArrayList;

public class CengTreeNodeInternal extends CengTreeNode
{
    private ArrayList<Integer> keys;
    private ArrayList<CengTreeNode> children;

    public CengTreeNodeInternal(CengTreeNode parent)
    {
        super(parent);

        // TODO: Extra initializations, if necessary.
        this.keys = new ArrayList<Integer>();
        this.children = new ArrayList<CengTreeNode>();
        this.type = CengNodeType.Internal;
    }

    // GUI Methods - Do not modify
    public ArrayList<CengTreeNode> getAllChildren()
    {
        return this.children;
    }
    public Integer keyCount()
    {
        return this.keys.size();
    }
    public Integer keyAtIndex(Integer index)
    {
        if(index >= this.keyCount() || index < 0)
        {
            return -1;
        }
        else
        {
            return this.keys.get(index);
        }
    }

    // Extra Functions
    
    @Override
    
    public void addtonode(CengBook book, CengTree tree)
    {
    	int i=0;
        while (i < this.keys.size() && this.keys.get(i) <= book.getBookID()) i++; 
        this.children.get(i).addtonode(book, tree);
    }
    
    
    public void addchildandkey(CengTreeNode node, Integer key,CengTree tree)
    {
    	int i=0;
        while (i < this.keys.size() && this.keys.get(i) < key) i++; 
        this.keys.add(i, key);
        this.children.add(i+1, node);
        
        if(this.keyCount()>2*order)
        {
        	Integer yukarikey=keys.get(order);
        	int sayi=0;
        	
        	CengTreeNodeInternal yeninode= new CengTreeNodeInternal(getParent());
        	for(int j=this.order+1; j<this.keyCount(); j++)
        	{
        		yeninode.keys.add(this.keys.get(j));
        		sayi++;
        	}
        	for(int j=this.order+1; j<this.children.size(); j++)
        	{
        		yeninode.children.add(this.children.get(j));
        		this.children.get(j).setParent(yeninode);
        		
        	}
        	int sayi2=sayi;
        	sayi++;
        	while(sayi>0)
        	{
        		keys.remove((int)order);
        		sayi--;
        	}
        	sayi2++;
        	while(sayi2>0)
        	{
        		children.remove((int)order+1);
        		sayi2--;
        	}
        	
        	if(getParent()==null)
        	{
        		CengTreeNodeInternal yeniroot= new CengTreeNodeInternal(null);
        		
        		yeniroot.ilkcocuk(this);
        		yeniroot.addchildandkey(yeninode, yukarikey, tree);
        		tree.root=yeniroot;
        		this.setParent(yeniroot);
        		yeninode.setParent(yeniroot);
        		
        		
        		
        		
        	}
        	else
        	{
        		( (CengTreeNodeInternal) getParent()).addchildandkey(yeninode, yukarikey, tree);
        		
        	}

        	
        	
        }
        
        

    }
    public void ilkcocuk(CengTreeNode cocuk)
    {
    	children.add(cocuk);
    }
    
    
    @Override
    
    public void printnode(int tabsay)
    {
    	for(int j=0; j<tabsay; j++)
    	{
    		System.out.printf("\t");
    	}
    	System.out.println("<index>");
    	for(int i=0; i<keyCount(); i++)
    	{
        	for(int j=0; j<tabsay; j++)
        	{
        		System.out.printf("\t");
        	}
        	System.out.println(keys.get(i));

    	}
    	
    	for(int j=0; j<tabsay; j++)
    	{
    		System.out.printf("\t");
    	}
    	System.out.println("</index>");
    	
    	for(int i=0; i<children.size(); i++)
    	{
    		getAllChildren().get(i).printnode(tabsay+1);
    	}

    	
    }

    
    
        
    
}
