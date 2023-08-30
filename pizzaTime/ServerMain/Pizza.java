import java.util.ArrayList;
/**
 * Write a description of class Pizza here.
 *
 * @author (your name)
 * @version (a version number or a date)
 */
public class Pizza
{
    private String size;
    private double price;
    private int slices;
    private ArrayList<Topping> t = new ArrayList<Topping>();
    
    /**
     * Constructor for Pizza, parameter is size of the Pizza.
     */
    public Pizza (String s)
    {
        size = s;
    }
    
    /**
     * Determines the number of slices based on the size of the Pizza.
     */
    public void slicePizza ()
    {
        if (size.equals("L"))
        {
            slices = 10;
        }
        else if (size.equals("M"))
        {
            slices = 8;
        }
        else
        {
            slices = 6;
        }
    }
    
    /**
     * Adds the Topping to the ArrayList of Toppings.
     * @parameter: new Topping that will be on the Pizza.
     */
    public void addInfo (Topping s)
    {
        t.add(s);
    }
    
    /**
     * Determines the price of Pizza based on Toppings and the size of the Pizza.
     */
    public void determinePrice ()
    {
        if (size.equals("L"))
        {
            price = 7.0;
        }
        else if (size.equals("M"))
        {
            price = 5.0;
        }
        else
        {
            price = 3.0;
        }
        for (Topping a: t)
        {
            price += a.getPrice();
        }
    }
    
    /**
     * Prints out all the information about the Pizza.
     */
    public String toString ()
    {
        ArrayList<String> top = new ArrayList<String>();
        for (Topping a: t)
        {
            top.add(a.getName());
        }
        return "Pizza size: " + size + "\nToppings: " + top + "\nPrice: " + price;
    }
    
    /**
     * Returns the ArraList of Toppings.
     */
    public ArrayList<Topping> getTopping ()
    {
        return t;
    }
}
