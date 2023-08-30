
import java.awt.Graphics;
import javax.imageio.ImageIO;
import java.awt.image.BufferedImage;
import java.io.File;

/**
 * Write a description of class Tomatoes here.
 *
 * @author (your name)
 * @version (a version number or a date)
 */
public class Tomatoes extends Topping
{
    // instance variables - replace the example below with your own
    static BufferedImage Tomato;
    /**
     * Constructor for objects of class Tomatoes
     */
    public Tomatoes(String n)
    {
        // initialise instance variables
        super(n);
    }
    public Tomatoes(String n, String c, int a)
    {
        // initialise instance variables
        super(n, c, a);
        
    }

    public void placeTopping(Graphics g, int x, int y)
    {        
        try 
        {
            Tomato= ImageIO.read(new File("Tomato.png"));
        }
        catch(Exception E)
        {
            System.out.println("Error loading image");
        }
        g.drawImage(Tomato, x, y, null);
    }
}
