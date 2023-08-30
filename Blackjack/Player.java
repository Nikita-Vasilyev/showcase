/**
 * Write a description of class Player here.
 * acts as the player for the game of blackjack
 * @author Nikita Vasilyev
 * @version Feb 13 */
public class Player
{
    private int chipsTot; 
    private int chipsBet;
    private Hand h1;
    /**
     * instantiates a hand for the player and sets total chips
     */
    public Player()
    {
        h1=new Hand();
        chipsTot=1000;
    }
    /**
     * @return void
     * doubles the amount of chips and adds one card to player deck

     */
    public void doubleDown()
    {
         chipsTot-=chipsBet;
         chipsBet*=2;
    }
    /**
     * @return int, the amount of total chips 
     * 
     */
    public int getChipsTot()
    {
        return chipsTot;
    }
    /**
     * @return int, the amount of chips bet
     * 
     */
    public int getChipsBet()
    {
        return chipsBet;
    }
    /**
     * @return void
     * sets the amount of total chips 
     */
    public void setChipsTot(int c)
    {
        chipsTot=c;
    }
    /**
     * @return void
     * @param int, the amount of chips bet 
     * sets the chips bet on this turn 
     */
    public void setChipsBet(int c)
    {
        chipsBet=c;
    }
    /**
     * @return the player's hand
     */
    public Hand getHand()
    {
        return h1;
    }
    /** 
     * @return the players hand 
     */
    public String toString()
    {
        return h1.toString();
    }
    /**
       * This method will return the total value of the hand inside of the deck 
       * of either the player or the dealer
       */
      public int getTotValue()
      {
          return h1.getTotValue();
      }
}