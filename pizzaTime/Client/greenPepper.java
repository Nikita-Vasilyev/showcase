import java.awt.Graphics;
import javax.imageio.ImageIO;
import java.awt.image.BufferedImage;
import java.io.File;

/**
 * Write a description of class greenPepper here.
 *
 * @author (your name)
 * @version (a version number or a date)
 */
public class greenPepper extends Topping
{
    static BufferedImage greenPepper;
    /**
     * Constructor for objects of class greenPepper
     */
    public greenPepper(String n)
    {
        // initialise instance variables
        super(n);
    }
    public greenPepper(String n, String c, int a)
    {
        // initialise instance variables
        super(n, c, a);
       
    }
    public void placeTopping(Graphics g, int x, int y)
    {        
         try 
        {
            greenPepper= ImageIO.read(new File("greenPepper.png"));
        }
        catch(Exception E)
        {
            System.out.println("Error loading image");
        }
        g.drawImage(greenPepper, x, y, null);
    }
    
}
