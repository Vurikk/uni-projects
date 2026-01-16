/*
 * Cardwidget is a class that inherits QPushButton.
 * It is used for runtime cards in the player deck.
 * Handles card clicking (sends signal on click, that
 * MainWindow class is listening to).
 */

#ifndef CARDWIDGET_HH
#define CARDWIDGET_HH
#include <QPushButton>
#include <QPixmap>
#include "unocard.hh"


using UnoCardPtr = std::shared_ptr<UnoCard>;

class CardWidget: public QPushButton
{
    Q_OBJECT
public:
    CardWidget(QWidget *parent = nullptr);
    ~CardWidget();

    /**
     * @brief set_card, sets private variable card_
     * @param card, the card to be set
     */
    void set_card(UnoCardPtr card);

    /**
     * @brief set_selected sets selection
     * @param selected, is card currently selected
     */
    void set_selected(bool selected);

    /**
     * @brief is_selected a getter function
     * @return if card is selected
     */
    bool is_selected() const;

    /**
     * @brief get_card a getter function
     * @return card
     */
    UnoCardPtr get_card() const;

    /**
     * @brief update_visuals updates card visuals.
     * Sets the correct image.
     */
    void update_visuals();


signals:
    /**
     * @brief click_signal sends a signal on click
     */
    void click_signal(CardWidget* self);

private slots:
    /**
     * @brief on_click is called when card is pressed
     */
    void on_click();

private:
    UnoCardPtr card_ = nullptr;
    bool selected_ = false;
    QPixmap image_;

};

#endif // CARDWIDGET_HH
