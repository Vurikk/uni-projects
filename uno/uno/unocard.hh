/*
 * This class handles the logic of cards.
 * It checks if next card can be played on top
 * of current card. It also stores card
 * type and color.
 */

#ifndef UNOCARD_HH
#define UNOCARD_HH

// Forward declaration of Uno and Player to avoid circular dependencies.
class Uno;
struct Player;

enum class CardType {
    ZERO,
    ONE,
    TWO,
    THREE,
    FOUR,
    FIVE,
    SIX,
    SEVEN,
    EIGHT,
    NINE,

    TAKE_TWO,
    SKIP,
    REVERSE,
    TAKE_FOUR,
    WILD
};

enum class CardColor {
    YELLOW,
    RED,
    BLUE,
    GREEN,
    BLACK
};


class UnoCard
{
public:
    UnoCard(CardColor p_color, CardType p_type);
    virtual ~UnoCard() = default;

    /**
     * @brief play Play this card in a game of Uno.
     * @param game The game of Uno this card is being played in.
     * @param player The player that played this card.
     */
    void play(Uno& game);

    /**
     * @brief can_play_on Determine if a given card can be played
     *   on this one.
     * @param new_card The card to play on this.
     * @return True if card is ok, false otherwise.
     */
    bool can_play_on(UnoCard const& new_card) const;

    // Getters for card color and type.
    CardColor color() const;
    CardType type() const;

private:    
    CardColor color_;
    CardType type_;
};

#endif // UNOCARD_HH
