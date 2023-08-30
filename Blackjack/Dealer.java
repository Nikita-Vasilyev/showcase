import java.util.ArrayList;
/**
 * Write a description of class Dealer here.
 *
 * @author Aashish Chaubal
 * @version 1 Feb 2019
 * Dealer begins by revealing their hole-card. From this point on, 
 * all cards should be visible to the player(s) 
Dealer must take a hit if their hand total is 16 or less.
Dealer always stands if their hand total is 17 or greater 
The round ends once the dealer stands, or as soon as the dealer's hand goes 
over 21 points (i.e. the dealer "busts")

 */
public class Dealer
{
    private Shoe d1 = new Shoe(5);
    private Hand hand2;
    /**
     * Constructor for Dealer class
     * @author Aashish Chaubal
     * @version Feb 2019
     */
    public Dealer()
    {
        hand2=new Hand();
    }
    
    /**
     * The start() method starts the game by dealing the cards 
     * @author Aashish Chaubal
     * @version Feb 2019
     */
    public void start(Hand p1)
    {
        p1.addCard(d1.dealCard());
        hand2.addCard(d1.dealCard());
        p1.addCard(d1.dealCard());
        hand2.addCard(d1.dealCard());
    }
    
    /**
     * The hitPlayer() method adds a card to the player when they take a hit
     * @author Aashish Chaubal
     * @version Feb 2019
     */
    
    public void hitPlayer(Hand p1)
    {
        p1.addCard(d1.dealCard());
    }
    
    /**
     * the makeMove() method allows the dealer to either hit or stand
     * @author Aashish Chaubal
     * @version Feb 2019
     */
    public void makeMove()
    {
        
        if(hand2.totValue() <= 16)
        {
            hand2.addCard(d1.dealCard());
        }
    }
    
    /**
     * calling back the shuffle method from the deck class when cards go less 
     * than 75 in the shoe
     * @author Aashish Chaubal
     * @version Feb 2019
     */
    public void reshuffle()
    {
        //Shoe d1 = new Shoe(5);
        if(d1.getNumCardsInShoe() < 75) 
        {
            d1.shuffleShoe();
        }
        
    }
    
        /**
     * Shows the face card of the dealer
     * @author Aashish Chaubal
     * @version Feb 2019
     */
    public Card showFace()
    {
        return hand2.getHand().get(0);
        
    }
    
            /**
     * Shows the face card of the dealer
     * @author Aashish Chaubal
     * @version Feb 2019
     */
    public Hand getHand()
    {
        return hand2;
    }
}