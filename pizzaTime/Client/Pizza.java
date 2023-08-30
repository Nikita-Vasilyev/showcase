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
    
    public Pizza (String s)
    {
        size = s;
    }
    
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
    
    public void addInfo (Topping s)
    {
        t.add(s);
    }
    
    public double determinePrice ()
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
        return price;
    }
    
    public String toString ()
    {
        ArrayList<String> top = new ArrayList<String>();
        for (Topping a: t)
        {
            top.add(a.getName());
        }
        return "Pizza Information\n" + "Pizza size: " + size + "\nToppings: " + top + "\nPrice: " + price;
    }
    
    public ArrayList<Topping> getTopping ()
    {
        return t;
    }
}
