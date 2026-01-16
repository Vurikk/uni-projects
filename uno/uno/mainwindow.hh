/*
 * This class handles the logic of game's UI.
 * It also triggers all the correct functions
 * on button presses.
 */

#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH

#include <QMainWindow>
#include <QPushButton>
#include <QHBoxLayout>
#include <QLabel>
#include <QSpinBox>
#include "cardwidget.hh"
#include "uno.hh"


class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    // These functions get called when their specific button is clicked
    void on_play_clicked();
    void on_draw_clicked();
    void on_new_game_clicked();
    void on_card_clicked(CardWidget* cardWidget);

private:
    // instance of the uno game
    Uno game_;

    QWidget* hand_container_ = nullptr;
    QHBoxLayout* hand_layout_ = nullptr;

    QPixmap deck_image_;

    // labels
    QLabel* last_played_card_ = nullptr;
    QLabel* info_label_ = nullptr;

    // buttons
    QPushButton* play_button_ = nullptr;
    QPushButton* draw_button_ = nullptr;
    QPushButton* new_game_button_ = nullptr;
    QPushButton* exit_button_ = nullptr;

    // spinboxes
    QSpinBox* player_count_spin_box_ = nullptr;
    QSpinBox* hand_size_spin_box_ = nullptr;

    // vector that contains currently selected cards
    std::vector<UnoCardPtr> selected_cards_;

    int card_width_;
    int card_height_;

    /**
     * @brief load_deck_image loads an image of a deck card
     */
    void load_deck_image();

    /**
     * @brief setup_ui is a function that initializes the UI
     */
    void setup_ui();

    /**
     * @brief update_ui is a function that updates the UI
     */
    void update_ui();

    /**
     * @brief update_hand is a function that updates hand UI
     */
    void update_hand();

    /**
     * @brief update_last_played_card is a function that
     * updates the image of previously played card
     */
    void update_last_played_card();

    /**
     * @brief clear_selection a helper function that resets
     * selected_cards_ vector.
     */
    void clear_selection();

    /**
     * @brief clear_hand_ui is a helper function that
     * deletes all cards from hand
     */
    void clear_hand_ui();
};

#endif // MAINWINDOW_HH
