// A Java program for a Server 
import java.net.*; 
import java.io.*; 
import java.util.ArrayList;
import java.net.ServerSocket;
import java.net.Socket;
import java.io.Serializable;
public class Server extends Thread implements Serializable
{ 
    //initialize socket and input stream 
    private Socket socket; 
    private ServerSocket server;
    private DataInputStream in; 
    private InputStream input;
    private ObjectInputStream inOb;
    private ObjectOutputStream obOut;
    private ArrayList<Topping> top;
    private ArrayList<Topping>out;
    private transient ArrayList<Topping> arr;
    // constructor with port 
    /**
     * Tries to connect with a client and recieve informations about 
     * pizza ordered. Prints the recieved informations and sends 
     * the pizza image back to the client.
     */
    public Server(int port, int num) throws ClassNotFoundException
    { 
        // starts server and waits for a connection 
        arr = new ArrayList<Topping>();
        try
        { 
            server = new ServerSocket(port); 
            System.out.println("Server started"); 
  
            System.out.println("Waiting for a client ..."); 
  
            socket = server.accept(); 
            System.out.println("Client accepted"); 
  
            // takes input from the client socket 
            //in = new DataInputStream( 
            //    new BufferedInputStream(socket.getInputStream())); 
            inOb = new ObjectInputStream(socket.getInputStream());
            String line = ""; 
            obOut = new ObjectOutputStream(socket.getOutputStream());
            
            // reads message from client until "Over" is sent 
            while (num == 1) 
            { 
                try
                { 
                   top = (ArrayList<Topping>)inOb.readObject();
                    System.out.println(top.toString());
                    //line = (String)inOb.readObject();
                    //System.out.println(line);
                   num++;
                } 
                catch(IOException i) 
                { 
                    System.out.println(i); 
                } 
            } 
            while (num == 0)
            {
                for (Topping t: pizzaMaker.getTops())
                {
                    arr.add(t);
                }
                obOut.writeObject(arr);
                num--;
            }
            System.out.println("Closing connection"); 
            
            // close connection 
            inOb.close();
            obOut.close();
            //in.close();
            
            socket.close();
            
        } 
        catch(IOException i) 
        { 
            System.out.println(i); 
        } 
    } 
} 
