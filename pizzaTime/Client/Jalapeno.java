import java.awt.Graphics;
import javax.imageio.ImageIO;
import java.awt.image.BufferedImage;
import java.io.File;


public class Jalapeno extends Topping
{
    static BufferedImage Jalapeno;
    /**
     * Constructor for objects of class Jalapeno
     */
    public Jalapeno(String n)
    {
        // initialise instance variables
        super(n);
    
    }
    public Jalapeno(String n, String c, int a)
    {
        // initialise instance variables
        super(n, c, a);
        
    }

    public void placeTopping(Graphics g, int x, int y)
    {        
        try 
        {
            Jalapeno= ImageIO.read(new File("Jalapenos.png"));
        }
        catch(Exception E)
        {
            System.out.println("Error loading image");
        }
        g.drawImage(Jalapeno, x, y, null);
    }
}
