import javax.swing.JFrame;
import javax.swing.JComponent;
import java.awt.Graphics; 
import javax.imageio.ImageIO;
import java.awt.image.BufferedImage;
import java.io.File;
import java.awt.MouseInfo;
import java.awt.event.MouseEvent;
import java.awt.event.MouseAdapter;
import java.util.ArrayList;
import java.util.Collections;
import java.util.LinkedList;
import java.util.List;
import java.awt.Color;
/**
 * Write a description of class pizzaMaker here.
 *
 * @author (your name)
 * @version (a version number or a date)
 */
public class pizzaMaker extends JComponent 
{
    static BufferedImage pizzaImage;
    static BufferedImage table;
    static BufferedImage button;
    static int x1;
    static int y1;
    static int count=0;
    static ArrayList<Topping> tops;
    static ArrayList<Topping> allToppings;
    static int startx=500;
    static int starty=0;      
    static int clickCount=0;
    /**
     * Constructor for pizzaMaker. Selects the clicked topping and 
     * show it at upper left hand corner. Then, brings the topping to 
     * where user clicked.
     */
    public pizzaMaker()
    {
        tops=new ArrayList<Topping>();
        allToppings = new ArrayList<Topping>();
        if(clickCount==0)
        {
            allToppings.add(new Tomatoes("Tomato"));
            allToppings.add(new Olives("Olive"));
            allToppings.add(new Pepperoni("Pepperoni"));
            allToppings.add(new Onion("Onion"));
            allToppings.add(new Sausage("Sausage"));
            allToppings.add(new Jalapeno("Jalapeno"));
            allToppings.add(new Pineapple("Pineapple"));
            allToppings.add(new Bacon("Bacon"));
            allToppings.add(new Chicken("Chicken"));
            allToppings.add(new greenPepper("GreenPepper"));
            for (Topping t: allToppings)
            {
                t.setC(startx, starty);
                starty += 100;
                if (starty >= 400)
                {
                    starty = 0;               
                    startx += 100;
                }
            }
            clickCount++;
        }
        addMouseListener(new MouseAdapter()
        {
            public void mousePressed(MouseEvent e)
            {
                x1=e.getX();
                y1=e.getY();                               
                if(count==0)
                {
                    for (Topping t: allToppings)
                    {
                        if ((x1>t.getX()&&x1<t.getX()+100)&&(y1>t.getY()&&y1<t.getY()+100))
                        {
                            if (t instanceof Onion)
                                tops.add(new Onion("Onion"));
                            if (t instanceof Sausage)
                                tops.add(new Sausage("Sausage"));
                            if (t instanceof Tomatoes)
                                tops.add(new Tomatoes("Tomatoes"));
                            if (t instanceof Jalapeno)
                                tops.add(new Jalapeno("Jalapeno"));
                            if (t instanceof Pineapple)
                                tops.add(new Pineapple("Pineapple"));
                            if (t instanceof Bacon)
                                tops.add(new Bacon("Bacon"));
                            if (t instanceof Olives)
                                tops.add(new Olives("Olives"));
                            if (t instanceof Chicken)
                                tops.add(new Chicken("Chicken"));
                            if (t instanceof Pepperoni)
                                tops.add(new Pepperoni("Pepperoni"));
                            if (t instanceof greenPepper)
                                tops.add(new greenPepper("greenPepper"));
                        }
                    }
                    count ++;
                }
                else if(count ==1)
                {
                    tops.get(tops.size()-1).setC(x1-20, y1-20);
                    count = 0;
                }
            }
        });
    }

    /**
     * Main code of this class. Downloads the image of table, base pizza, 
     * and red button. Makes pizzaMaker object to start the program.
     * Then, set up a new server to send the image back.
     */
    public static void main(String[] args) throws ClassNotFoundException
    {
        Server server = new Server(5001, 1);
        
        Audio.playAudio();
        try
        {
            pizzaImage=ImageIO.read(new File("Cheese.png"));
        }
        catch(Exception E)
        {
            System.out.println("error loading image");
        }
        try
        {
            table=ImageIO.read(new File("Table.jpg"));
        }
        catch(Exception E)
        {
            System.out.println("error loading image");
        }
        try
        {
            button=ImageIO.read(new File("redButton.png"));
        }
        catch(Exception E)
        {
            System.out.println("error loading image");
        }
        JFrame frame=new JFrame();
        pizzaMaker comp=new pizzaMaker();
        frame.add(comp);
        frame.setSize(1000, 500);
        frame.setVisible(true);
        boolean bool = true;
        while(bool)
        {
            frame.repaint();
            if(x1 > 900 && y1 > 400)
            {
                bool = false;
            }
        }
        Server serve = new Server (5002, 0);
    }

    /**
     * Paints the background (table, base pizza, and red button
     * to kill the program) and every toppings.
     */
    public void paintComponent(Graphics g)
    {
        g.drawImage(table, 0, 0, null);
        g.drawImage(pizzaImage, 0, -30, null);
        g.drawImage(button,900,400,null);
        g.setColor(Color.black);
        g.fillRect(90, 0, 5, 95);
        g.fillRect(0, 90, 95, 5);
        g.setColor(new Color(109, 165, 255));
        g.fillRect(0, 0, 90, 90);
        for(Topping t:tops)
        {
            t.placeTopping(g, t.getX(), t.getY());
        }
        for(Topping t: allToppings)
        {
            t.placeTopping(g, t.getX(), t.getY());
        }
    }
    /**
     * returns arrayList of toppings 
     */
    public static ArrayList<Topping> getTops()
    {
        return tops;
    }
}