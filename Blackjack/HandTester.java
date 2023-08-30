
/**
 * Write a description of class HandTester here.
 *
 * @author (your name)
 * @version (a version number or a date)
 */
public class HandTester
{
   public static void main (String [] args)
   {
       Card c1 = new Card(0);
       Card c2 = new Card(40);
       Card c3 = new Card(4);
       
       System.out.println(c1);
       System.out.println(c2);
       System.out.println(c3);
       
       Hand hand1 = new Hand();

       
       
       hand1.addCard(c1);

       hand1.addCard(c2);
       hand1.addCard(c3);
       System.out.println("Value is:"+hand1.getTotValue());
       //System.out.println(hand1.checkBlackjack());
       
   
       
       
       System.out.println(hand1);
    }
}
