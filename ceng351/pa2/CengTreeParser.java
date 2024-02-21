import java.io.IOException;
import java.util.ArrayList;
import java.io.File;
import java.io.FileNotFoundException;
import java.util.Scanner;

public class CengTreeParser
{
    public static ArrayList<CengBook> parseBooksFromFile(String filename)
    {        
    	ArrayList<CengBook> inpbooks = new ArrayList<CengBook>();

    	try {

        File text = new File(filename);
        Scanner scan = new Scanner(text);
		

        while(scan.hasNextLine())
        {
            String line = scan.nextLine();
            String[] attr = line.split("\\|");

            inpbooks.add(new CengBook(Integer.parseInt(attr[0]), attr[1], attr[2], attr[3]));
        }
       

        } catch (FileNotFoundException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
    	return inpbooks;

    }

    public static void startParsingCommandLine() throws IOException
    {
        Scanner scan = new Scanner(System.in);
        while (scan.hasNextLine()) 
        {
            String line = scan.nextLine();

            String[] attr = line.split("\\|");


            if (attr[0].equals("quit"))
            {
                break;
            }
            else if (attr[0].equals("print"))
            {
            	CengBookRunner.printTree();
            }
            else if (attr[0].equals("search"))
            {
            	CengBookRunner.searchBook(Integer.parseInt(attr[1]));
            }
            else if (attr[0].equals("add"))
            {
                CengBookRunner.addBook(new CengBook(Integer.parseInt(attr[1]), attr[2], attr[3], attr[4]));
            }
        }
    }
}