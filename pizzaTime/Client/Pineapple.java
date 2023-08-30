import java.awt.Graphics;
import javax.imageio.ImageIO;
import java.awt.image.BufferedImage;
import java.io.File;


public class Pineapple extends Topping
{
    static BufferedImage pineapple;
    /**
     * Constructor for objects of class Pineapple
     */
    public Pineapple(String n)
    {
        // initialise instance variables
        super(n);
        
    
    }
    public Pineapple(String n, String c, int a)
    {
        // initialise instance variables
        super(n, c, a);
       
    
    }
    public void placeTopping(Graphics g, int x, int y)
    {        
         try 
        {
            pineapple= ImageIO.read(new File("Pineapple.png"));
        }
        catch(Exception E)
        {
            System.out.println("Error loading image");
        }
        g.drawImage(pineapple, x, y, null);
    }
}
