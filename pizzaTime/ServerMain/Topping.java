import java.util.Scanner;
import java.awt.Graphics;
import java.awt.Color;
import java.io.Serializable; 
/**
 * Write a description of interface Topping here.
 *
 * @author (your name)
 * @version (a version number or a date)
 */
public abstract  class Topping implements Serializable
{
    private String name;
    private String coverage;
    private int amount;
    private double cost = 0.5;
    private int x;
    private int y;
    /**
     * Constructor for Topping, parameter is the name of the Topping.
     */
    public Topping(String n)
    {
        name = n;
        Scanner scan = new Scanner(System.in);
    }
    /**
     * Another Constructor for Topping, parameters are name of the Topping, 
     * coverage on the Pizza, and amount of it.
     */
    public Topping(String n, String c, int a)
    {
        name=n; coverage=c; amount=a;
    }
    /**
     * takes the inputs and sets the coordinate of topping to that
     */
    public void setC(int x1, int y1)
    {
        x=x1;
        y=y1;
    }
    /**
     * returns the x coordinate of topping
     */
    public int getX()
    {
        return x;
    }
    /**
     * returns the y coordinate of topping 
     */
    public int getY()
    {
        return y;
    }
    /**
     * returns the price of the topping
     */
    public double getPrice()
    {
        return cost;
    }
    /**
     * returns the name of the topping
     */
    public String getName()
    {
        return name;
    }
    /**
     * prints out informations about topping including the name, coverage,
     * and amount.
     */
    public String toString()
    {
        return name + " " + coverage + " " + amount;
    }
    /**
     * abstract method which places toppings at wherever the user 
     */
    public abstract void placeTopping(Graphics g, int x2, int y2);
}
