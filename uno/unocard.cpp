#include "unocard.hh"

#include "uno.hh" // Included here and not in header to avoid circular dependency.

// constructor
UnoCard::UnoCard(CardColor p_color, CardType p_type):
    color_(p_color), type_(p_type)
{
}

void UnoCard::play(Uno& game)
{
    switch(type_)
    {
        case CardType::SKIP:
        {
            // skip player
            game.next_player();
            break;
        }
        case CardType::REVERSE:
        {
            // change order
            game.change_playing_order();
            break;
        }
        case CardType::TAKE_TWO:
        {
            // get the next player
            PlayerPtr nextPlayer = game.get_next_player();

            // add two cards to player
            for (int i = 0; i < 2; ++i) {
                UnoCardPtr newCard = game.make_card();
                nextPlayer->hand.insert(newCard);
            }
            break;
        }
        case CardType::TAKE_FOUR:
        {
            //change color to random
            color_ = game.random_color();

            // get the next player
            PlayerPtr nextPlayer = game.get_next_player();

            // add four cards to player
            for (int i = 0; i < 4; ++i) {
                UnoCardPtr newCard = game.make_card();
                nextPlayer->hand.insert(newCard);
            }
            break;
        }
        case CardType::WILD:
        {
            // set this cards color to random
            color_ = game.random_color();
            break;
        }
        default:
            break;
    }
}

// Function that param: card can be played on top of this card
bool UnoCard::can_play_on(UnoCard const& new_card) const
{
    // if the card is black, then any color can be played
    // or if the new card is black
    if(color_ == CardColor::BLACK || new_card.color() == CardColor::BLACK)
        return true;


    // compare the colors or types. If any of them match,
    // then card can be played
    if(new_card.type() == type_ || new_card.color() == color_)
    {
        return true;
    }
    else
    {
        return false;
    }
}

// Getters for card color and type.
CardColor UnoCard::color() const
{
    return color_;
}
CardType UnoCard::type() const
{
    return type_;
}
