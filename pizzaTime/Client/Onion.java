import java.awt.Graphics;
import javax.imageio.ImageIO;
import java.awt.image.BufferedImage;
import java.io.File;
/**
 * Write a description of class Onion here.
 *
 * @author (your name)
 * @version (a version number or a date)
 */
public class Onion extends Topping
{
    static BufferedImage onion;
    /**
     * Constructor for objects of class Onion
     */
    public Onion(String n)
    {
        // initialise instance variables
        super(n);

    }
    public Onion(String n, String c, int a)
    {
        // initialise instance variables
        super(n, c, a);

    }
    public void placeTopping(Graphics g, int x, int y)
    {        
                try 
        {
            onion = ImageIO.read(new File("Onion.png"));
        }
        catch(Exception E)
        {
            System.out.println("Error loading image");
        }
        g.drawImage(onion, x, y, null);
    }
    
}
