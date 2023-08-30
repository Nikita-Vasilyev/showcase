import java.util.ArrayList;
/**
 * Write a description of class Hand here.
 *
 * @author (your name)
 * @version (a version number or a date)
 */
public class Hand
{
    private ArrayList <Card> cards = new ArrayList <Card>();
    /**
     *  Default constructor since the value depends upon what cards are dealt to
     *  the player and the dealer
     */

    public Hand()
    {

    }

    /**
     *  This method should calculate the total value of the cards in the players
     *  hand
     *  @return void
     */
    public int totValue()
    {
        int totValue = 0;
        for (int x = 0; x<cards.size(); x++)
        {
            totValue += cards.get(x).getCardValue();
        }
        return totValue;
    }

    /**
     * This method will return the total value of the hand inside of the deck 
     * of either the player or the dealer
     * @return int 
     */
    public int getTotValue()
    {
        return totValue();
    }

    /**
     * This method will determine if the total value of the hand in the first 
     *  hand is blackjack and if it is then it will  This numeric value will be interpreted in the game class
     * @return int - If blackjack return 0. If not blackjack returns -1
     */
    public int checkBlackjack()
    {
        if(totValue()==21 && cards.size()==2)
        {
            return 0;
        }
        else
        {
            return -1;
        }
    }

    /**
     * This method will check for any aces inside of the hand if the total 
     * value is above 21 and convert it from 11 to 1
     */
    public void checkAces()
    {
        if (totValue()>21)
        {
            for(int i=0; i<cards.size();i++ )
            {
                if(cards.get(i).getCardValue()==11)
                {
                    cards.get(i).setAceToOne(true);
                    i=cards.size();
                }
            }
        }
    }
    /**
     * This method would add a card to the hand of either the player 
     */
    public void addCard(Card acr)
    {
        cards.add(acr);
        checkAces();
      
    }
    /**
     * @return the hand 
     */
    public ArrayList<Card> getHand()
    {
        return cards;
    }
    /**
     * Removes the cards in the hand  
     */
    public void deleteCards()
    {
        for (int i=cards.size()-1; i>=0; i--)
        {
            cards.remove(i);
        }
    }
    /**
     * @return prints the cards  
     */
    public String toString()
    {
        return cards.toString();
    }
}