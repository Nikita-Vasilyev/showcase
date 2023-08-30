import java.util.ArrayList;
import java.util.Collections;
/**
 * Shoe models the behavior of a casino-style shoe that holds and
 * allows access to multiple Decks of Cards which have been 
 * combined into a single larger set of Cards.
 * 
 * @author David Gumminger 
 * @version March 2018
 */
public class Shoe
{
    private int numDecksInShoe;
    private ArrayList<Card> theShoe;
    private ArrayList<Card> usedCards;   //discard pile

    /**
     * Creates a Shoe composed of the specified number of Decks.
     * Though not explicitly checked, a typical Shoe would hold
     * between four and seven complete Decks. 
     * @param - int n, the number of Decks to be placed in the Shoe
     * @author David Gumminger
     * @version March 2018  
     */
    public Shoe (int n)
    {
        numDecksInShoe = n;
        theShoe = new ArrayList<Card>();
        usedCards =new ArrayList<Card>();
        
        //adds the specified number of decks one at a time
        for (int i = 0; i < numDecksInShoe; i++)
        {
            // create a deck
            Deck deck = new Deck();
            // get the ArrayList containing the shuffled deck
            ArrayList<Card> oneDeck = deck.getDeck();
            // add the cards to the shoe
            for (Card c: oneDeck)
                theShoe.add(c);
        }
        //Once all cards added, shuffle the Shoe so it is ready
        //to use
        shuffleShoe();
    }

    /**
     * Shuffles the Shoe. Begins by placing any Cards from the
     * discard pile and adding them back to the main Shoe. All
     * Aces additionally have their values reset to eleven.
     * @author David Gumminger
     * @version March 2018
     */
    public void shuffleShoe()
    {
        
        // Put any discarded Cards back into the deck
        while (usedCards.size() > 0)
        {
            Card c = usedCards.remove(0);
            // set the card back to a value of 11 if it's an Ace
            c.setAceToOne(false);
            theShoe.add(c);
        }
        /* widely accepted that a good mix on a deck of cards 
         * requires 7 shuffles.
         */
        
        System.out.println("Shuffling...");
        for(int i = 0; i < 7; i++)
            Collections.shuffle(theShoe);
            
    }

    /**
    * Returns the Card from the top of the Shoe. This Card is also
    * added to the discard pile. While this doesn't exactly mimic
    * real-world behavior, it makes clean-up after each Hand more
    * efficient.
    * @author David Gumminger
    * @version March 2018  
    */
    public Card dealCard()
    {
        Card dealtCard = theShoe.remove(0);
        // add copy of card to discards for later reshuffling
        usedCards.add(dealtCard);
        return dealtCard;
    }
    
    /**
    * Prints out the cards the Shoe, one per line. Used mainly
    * for testing purposes
    * @author David Gumminger
    * @version March 2018  
    */
    public void printShoe()
    {
        for (Card c: theShoe)
            System.out.println(c);
    }

    /**
    * Returns the number of Cards remaining in the Shoe. Used
    * primarily to check for when the Shoe needs to be reshuffled
    * @author David Gumminger
    * @version March 2018  
    */
    public int getNumCardsInShoe()
    {
        return theShoe.size();
    }
}
