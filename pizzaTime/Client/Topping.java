import java.util.Scanner;
import java.awt.Graphics;
import java.awt.Color;
import java.io.Serializable; 
/**
 * Topping class already java doc'd in the server (same class)
 * */
public abstract  class Topping implements Serializable
{
    private String name;
    private String coverage;
    private int amount;
    private double cost = 0.5;
    private int x;
    private int y;
    public Topping(String n)
    {
        name = n;
        Scanner scan = new Scanner(System.in);
    }
    public Topping(String n, String c, int a)
    {
        name=n; coverage=c; amount=a;
    }
    public void setC(int x1, int y1)
    {
        x=x1;
        y=y1;
    }
    public int getX()
    {
        return x;
    }
    public int getY()
    {
        return y;
    }
    public double getPrice()
    {
        return cost;
    }
    public String getName()
    {
        return name;
    }
    public String toString()
    {
        return name + " " + coverage + " " + amount;
    }
    public abstract void placeTopping(Graphics g, int x2, int y2);
}
