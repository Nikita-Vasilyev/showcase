
/**
 * Write a description of class PlayerTester here.
 *
 * @author (your name)
 * @version (a version number or a date)
 */
public class PlayerTester
{
    public static void main (String [] args)
    {
        Player p1 = new Player();
        Card c5 = new Card(45);
        Card c6 = new Card(55);
        p1.setChipsBet(100);
        p1.doubleDown();
        System.out.println(p1.getChipsTot());
        System.out.println(p1.getChipsBet());
        p1.getHand().addCard(c5);
        p1.getHand().addCard(c6);
        System.out.println(p1);
        System.out.println(p1.getHand());
        System.out.println(p1.getTotValue());
        System.out.println(p1.getHand().checkBlackjack());
    }
}
