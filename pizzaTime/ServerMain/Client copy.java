import java.net.*; 
import java.io.*; 
import java.util.Scanner;
import java.util.ArrayList;
import java.io.Serializable;
public class Client implements Serializable
{ 
    // initialize socket and input output streams 
    private Socket socket; 
    private DataInputStream  input; 
    private DataOutputStream out; 
    private ObjectOutputStream obOut;
    private transient ArrayList<Topping> arr;
    // constructor to put ip address and port 
    public Client(String address, int port, String x, ArrayList<Topping> p) 
    { 
        // establish a connection 
        arr = new ArrayList<Topping>();
        for (Topping t : p)
        {
            arr.add(t);
        }
        try
        { 
            socket = new Socket(address, port); 
            System.out.println("Connected"); 

            // takes input from terminal 
            input  = new DataInputStream(System.in); 

            // sends output to the socket 
            out = new DataOutputStream(socket.getOutputStream());
            obOut = new ObjectOutputStream(socket.getOutputStream());
        } 
        catch(UnknownHostException u) 
        { 
            System.out.println(u); 
        } 
        catch(IOException i) 
        { 
            System.out.println(i); 
        } 

        // string to read message from input 
        String line = ""; 

        // keep reading until "Over" is input 
        int count = 0;
        while (!line.equals("Close")) 
        { 
            try
            { 
                while (count == 0)
                {
                    //out.writeUTF(x);
                    obOut.writeObject(arr);
                    //obOut.writeObject(x);
                    count++;
                }
                line = input.readLine();
            } 
            catch(IOException i) 
            { 
                System.out.println(i); 
            } 
        } 

        // close the connection 
        try
        { 
            input.close(); 
            out.close();
            
            obOut.close();
            socket.close(); 
        } 
        catch(IOException i) 
        { 
            System.out.println(i); 
        } 
    } 

    public static void main(String args[]) 
    { 

        Scanner scan = new Scanner(System.in);
        System.out.println("What do you want the size to be?(S, M, or L)");
        String size = scan.next();
        Pizza p = new Pizza(size);
        String name = "";
        String ans = "yes";
        String coverage = "";
        int amount;
        System.out.println("Available toppings:\n-Tomatoes\t-Onion  \t-Olives\n-Green Pepper\t-Jalapeno \t-Pineapple\n-Chicken\t-Bacon  \t-Pepperoni");
        while (ans.equals("yes") || ans.equals("Yes"))
        {
            System.out.println("What toppings do you want?");
            name = scan.next();
            System.out.println("Do you want topping on whole Pizza or Half of Pizza?(W/H)");
            coverage = scan.next();
            if (coverage.equals("H") || coverage.equals("Half"))
            {
                System.out.println("Which side of Pizza do want this topping to be placed?(L/R)");
                coverage = scan.next();
            }
            System.out.println("On a scale of 1-4, how much do you want (4 being the most)");
            amount = scan.nextInt();
            if (name.equals("Tomatoes"))
            {
                Tomatoes toma = new Tomatoes(name, coverage, amount);
                p.addInfo(toma);
            }
            if (name.equals("Bacon"))
            {
                Bacon baco = new Bacon(name, coverage, amount);
                p.addInfo(baco);
            }
            if (name.equals("Onion"))
            {
                Onion onio = new Onion(name, coverage, amount);
                p.addInfo(onio);
            }
            if (name.equals("Olives"))
            {
                Olives oliv = new Olives(name, coverage, amount);
                p.addInfo(oliv);
            }
            if (name.equals("Green Pepper"))
            {
                greenPepper grpe = new greenPepper(name, coverage, amount);
                p.addInfo(grpe);
            }
            if (name.equals("Jalapeno"))
            {
                Jalapeno jala = new Jalapeno(name, coverage, amount);
                p.addInfo(jala);
            }
            if (name.equals("Pineapple"))
            {
                Pineapple pine = new Pineapple(name, coverage, amount);
                p.addInfo(pine);
            }
            if (name.equals("Chicken"))
            {
                Chicken chic = new Chicken(name, coverage, amount);
                p.addInfo(chic);
            }
            if (name.equals("Pepperoni"))
            {
                Pepperoni pepp = new Pepperoni(name, coverage, amount);
                p.addInfo(pepp);
            }

            System.out.println("Do you want more toppings?");
            ans = scan.next();
        }
        p.slicePizza();
        p.determinePrice();
        System.out.println(p.toString());
        String toppingList = "";
        for (Topping t: p.getTopping())
        {
            toppingList += t.toString() + "\n"; 
        }
        Client c = new Client("localhost", 5001, toppingList, p.getTopping());
    }
}
