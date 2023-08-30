import java.awt.Graphics;
import javax.imageio.ImageIO;
import java.awt.image.BufferedImage;
import java.io.File;

/**
 * Write a description of class Pepperoni here.
 *
 * @author (your name)
 * @version (a version number or a date)
 */
public class Pepperoni extends Topping
{
    /**
     * Constructor for objects of class greenPepper
     */
    static BufferedImage pepperoni;
    public Pepperoni(String n)
    {
        // initialise instance variables
        super(n);
    
    }
    public Pepperoni(String n, String c, int a)
    {
        // initialise instance variables
        super(n, c, a);

    }
    public void placeTopping(Graphics g, int x, int y)
    {        
                try 
        {
            pepperoni= ImageIO.read(new File("Pepperoni.png"));
        }
        catch(Exception E)
        {
            System.out.println("Error loading image");
        }
        g.drawImage(pepperoni, x, y, null);
    }
}
