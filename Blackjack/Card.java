
/**
 * Represents a single playing card from a deck of cards.In particular
 * the card's methods have been defined to support the implmentation
 * of a blackjack game.
 * 
 * @author David Gumminger
 * @version March 2018
 */
public class Card
{
    private String suit;
    private int value;
    private String rank;
    
    /** 
     * Takes a number between 0 and 51 and converts it to a unique
     * card from a standard 52 card deck. Note, this is NOT random.
     * The same number will always generate the same card. Correct
     * use of this constructor for a Deck would be call it 52 times
     * passing it a unique number each time. 
     * @ precondition: 0 <= card < 52
     * @ param an integer between 0 and 51
     * @author David Gumminger
     * @version March 2018  
     */
    public Card (int card)
    {
        int temp1 = card % 4;
        if (temp1 == 0)
            suit = " of Hearts";
        else if (temp1 == 1)
            suit = " of Spades";
        else if (temp1 == 2)
            suit = " of Diamonds";
        else
            suit = " of Clubs";
        temp1 = (card / 4) % 13;
        if (temp1 == 0){
            rank = "Ace";
            value = 11;}
        else if (temp1 == 1){
            rank = "Two";
            value = 2;}
        else if (temp1 == 2){
            rank = "Three";
            value = 3;}
        else if (temp1 == 3){
            rank = "Four";
            value = 4;}
        else if (temp1 == 4){
            rank = "Five";
            value = 5;}
        else if (temp1 == 5){
            rank = "Six";
            value = 6;}
        else if (temp1 == 6){
            rank = "Seven";
            value = 7;}
        else if (temp1 == 7){
            rank = "Eight";
            value = 8;}
        else if (temp1 == 8){
            rank = "Nine";
            value = 9;}
        else if (temp1 == 9){
            rank = "Ten";
            value = 10;}
        else if (temp1 == 10){
            rank = "Jack";
            value = 10;}
        else if (temp1 == 11){
            rank = "Queen";
            value = 10;}
        else{
            rank = "King";
            value = 10;}
    }
    /**
     * Determines if the given card is an Ace
     * @return True if the card is an Ace. False otherwise
     * @author David Gumminger
     * @version March 2018  
     */
    public boolean isAce()
    {
        return rank.equals("Ace");
    }
    /**
     * Produces a string that contains the card's rank and suit
     * @return string with card's rank and suit (e.g. Ace of Hearts)
     * @author David Gumminger
     * @version March 2018  
     */
    public String toString()
    {
        return rank+suit;  
    }
    /**
     * Get the cards value for a game of Blackjack
     * @return int between 1 and 11
     * @author David Gumminger
     * @version March 2018  
     */
    public int getCardValue()
    {
        return value;
    }
    /**
     * Changes an ace's value from 11 points to 1 or 1 to 11
     * @param setToOne - True if the card should be changed to
     *   a value of one. False to change the value back to 11
     * @author David Gumminger
     * @version March 2018  
     */
    public void setAceToOne(boolean setToOne)
    {
        if(isAce())
           if(setToOne)
                value = 1;
           else
                value = 11;
    }
}
