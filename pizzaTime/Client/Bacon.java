import java.awt.Graphics;
import javax.imageio.ImageIO;
import java.awt.image.BufferedImage;
import java.io.File;



public class Bacon extends Topping
{
    static BufferedImage Bacon;
    public Bacon(String n)
    {
        // initialise instance variables
        super(n);
    }
    public Bacon(String n, String c, int a)
    {
        // initialise instance variables
        super(n, c, a);
        
    }

    public void placeTopping(Graphics g, int x, int y)
    {        
        try 
        {
            Bacon= ImageIO.read(new File("Bacon.png"));
        }
        catch(Exception E)
        {
            System.out.println("Error loading image");
        }
        g.drawImage(Bacon, x, y, null);
    }
}
