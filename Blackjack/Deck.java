import java.util.ArrayList;
import java.util.Collections;
/**
 * Deck generates a ArrayList which represents a standard Deck
 * of 52 playing cards (no jokers) and provides methods to 
 * access and shuffle the constructed Deck.
 * 
 * @author David Gumminger
 * @version March 2018
 */
public class Deck
{
    private ArrayList<Card> deck = new ArrayList<Card>();
    
    /**
    * Creates the Deck by passing consequtive numbers between 
    * 0 and 51 to the Card class constructor. This ensures exactly
    * one of each card is created and added to the Deck. See details
    * of the Card class constructor for more information on how
    * Card creation is designed.
    * @author David Gumminger
    * @version March 2018  
    */
    public Deck ()
    {
        for (int i = 0; i < 52; i++)
        {
            deck.add(new Card(i));
        }
        //shuffle the deck before passing it back to use
        shuffleDeck();
    }
    
    /**
    * Returns a pointer to the ArrayList that contains the Cards
    * that make up this deck.
    * @return ArrayList<Card> - pointer to the Cards in the Deck
    * @author David Gumminger
    * @version March 2018  
    */
    public ArrayList<Card> getDeck()
    {
        return deck;
    }
    
    /**
    * Prints out the cards the Deck, one per line. Used mainly
    * for testing purposes
    * @author David Gumminger
    * @version March 2018  
    */
    public void printDeck(){
        for (Card c: deck)
            System.out.println(c);
    }

    /**
    * Shuffles the cards in the Deck
    * @author David Gumminger
    * @version March 2018  
    */
    public void shuffleDeck(){
        /* widely accepted that a good mix on a deck of cards 
         * requires 7 shuffles.
         */
        for(int i = 0; i < 7; i++)
            Collections.shuffle(deck);
    }
}
