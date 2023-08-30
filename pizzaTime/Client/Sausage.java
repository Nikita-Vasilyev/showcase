import java.awt.Graphics;
import javax.imageio.ImageIO;
import java.awt.image.BufferedImage;
import java.io.File;

public class Sausage  extends Topping
{
    static BufferedImage sausage;
    /**
     * Constructor for objects of class Sausage
     */
    public Sausage(String n)
    {
        // initialise instance variables
        super(n);
    
    }
    public Sausage(String n, String c, int a)
    {
        // initialise instance variables
        super(n, c, a);
        
        
   
    }


    public void placeTopping(Graphics g, int x, int y)
    {        
        try 
        {
            sausage= ImageIO.read(new File("Sausage.png"));
        }
        catch(Exception E)
        {
            System.out.println("Error loading image");
        }
        g.drawImage(sausage, x, y, null);
    }
    }

