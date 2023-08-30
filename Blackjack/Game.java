import java.util.Scanner;
/**
 * Write a description of class Game here.
 * This class runs blackjack
 * @author Nikita Vasilyev
 * @version 2/27/2019
 */
public class Game
{
    public static void main(String args[])
    {
        Player p1=new Player();
        Dealer d1=new Dealer();
        String cont="y";
        boolean round=true;
        Scanner s1=new Scanner(System.in);
        while (cont.equals("y")||cont.equals("Y"))
        {
            round=true;
            d1.reshuffle();
            d1.start(p1.getHand());               
            System.out.println("Your total chips: "+p1.getChipsTot());
            System.out.println("How much would you like to bet?");
            int move=s1.nextInt();
            while(move>p1.getChipsTot())
            {
                System.out.println("Not enough chips");
                System.out.println("How much would you like to bet?");
                move=s1.nextInt();
            }
            p1.setChipsTot(p1.getChipsTot()-move);
            p1.setChipsBet(move);
            System.out.println("The dealers first card is: "+d1.showFace());
            d1.makeMove();
            
            while(round)
            {
                System.out.println("\nThis is your hand: "+p1);
                System.out.println("The value of your cards is: "+p1.getTotValue());
                System.out.println("What would you like to do? Stand(s), Hit(h) or Double Down(d)");
                String move2= s1.next();
                
                while(!move2.equals("s")&&!move2.equals("h")&&!move2.equals("d"))
                {
                    System.out.println("Please enter a move (s/h/d): ");
                    move2=s1.next();
                }
                
                if(move2.equals("s"))
                {
                    round=false;
                }
                else if(move2.equals("h"))
                {
                    d1.hitPlayer(p1.getHand());
                }
                else
                {
                    if(p1.getChipsTot()<(p1.getChipsBet()))
                        System.out.println("Not enough chips");
                    else
                    {
                        p1.doubleDown();
                        d1.hitPlayer(p1.getHand());
                        System.out.println("Your total chips: "+p1.getChipsTot());
                        System.out.println("Your chips bet: "+p1.getChipsBet());
                        System.out.println("\nThis is your hand: "+p1);
                        System.out.println("The value of your cards is: "+p1.getTotValue());
                        round=false;
                    }
                }
                if(p1.getTotValue()>21)
                {
                    round=false;
                }
            }
            if(p1.getHand().checkBlackjack()==0&&d1.getHand().checkBlackjack()==-1)
            {
                System.out.println("You won with a blackjack!");
                p1.setChipsTot(p1.getChipsTot()+(int)(p1.getChipsBet()*(5/2)));
            }
            else if(p1.getHand().checkBlackjack()==-1&&d1.getHand().checkBlackjack()==0)
            {
                System.out.println("Dealer won with blackjack!");
            }
            else if(p1.getTotValue()>21)
            {
                System.out.println("Player busted!");
            }
            else if(d1.getHand().getTotValue()>21)
            {
                System.out.println("Dealer busted!");
                p1.setChipsTot(p1.getChipsTot()+(p1.getChipsBet()*2));
            }
            else if(p1.getTotValue()>d1.getHand().getTotValue())
            {
                System.out.println("You won!");
                p1.setChipsTot(p1.getChipsTot()+(p1.getChipsBet()*2));
            }
            else if(p1.getTotValue()<d1.getHand().getTotValue())
            {
                System.out.println("You lost!");
            }
            else if(p1.getTotValue()==d1.getHand().getTotValue())
            {
                System.out.println("Tie game!");
                p1.setChipsTot(p1.getChipsTot()+p1.getChipsBet());
            }

            System.out.println("\nThe dealer's cards: "+d1.getHand().toString());
            System.out.println("Your total chips: "+p1.getChipsTot());
            System.out.println("Would you like to start next round?(y/n)");
            cont=s1.next();
            d1.getHand().deleteCards();
            p1.getHand().deleteCards();
            if(p1.getChipsTot()<=0)
            {
                cont="n";
            }
        }
    }
}
