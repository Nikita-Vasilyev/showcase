import java.awt.Graphics;
import javax.imageio.ImageIO;
import java.awt.image.BufferedImage;
import java.io.File;
/**
 * Write a description of class Jalapeno here.
 *
 * @author (your name)
 * @version (a version number or a date)
 */
public class Chicken extends Topping
{
    static BufferedImage chicken;
    /**
     * Constructor for objects of class Jalapeno
     */
    public Chicken(String n)
    {
        // initialise instance variables
        super(n);

    }
    public Chicken(String n, String c, int a)
    {
        // initialise instance variables
        super(n, c, a);
        
    }
    public void placeTopping(Graphics g, int x, int y)
    {        
        try 
        {
            chicken= ImageIO.read(new File("chicken.png"));
        }
        catch(Exception E)
        {
            System.out.println("Error loading image");
        }
        g.drawImage(chicken, x, y, null);
    }
}
