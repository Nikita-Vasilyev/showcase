import java.awt.Graphics;
import javax.imageio.ImageIO;
import java.awt.image.BufferedImage;
import java.io.File;


public class Olives extends Topping
{
    static BufferedImage olive;
    /**
     * Constructor for objects of class Jalapeno
     */
    public Olives(String n)
    {
        // initialise instance variables
        super(n);
        
   
    }
    public Olives(String n, String c, int a)
    {
        // initialise instance variables
        super(n, c, a);

    
    }

    public void placeTopping(Graphics g, int x, int y)
    {        
       try 
        {
            olive= ImageIO.read(new File("olives.png"));
        }
        catch(Exception E)
        {
            System.out.println("Error loading image");
        }
        g.drawImage(olive, x, y, null);
    }
}
