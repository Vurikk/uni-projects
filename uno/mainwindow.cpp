#include "mainwindow.hh"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    setup_ui();

    connect(play_button_, &QPushButton::clicked,
            this, &MainWindow::on_play_clicked);
    connect(draw_button_, &QPushButton::clicked,
            this, &MainWindow::on_draw_clicked);
    connect(new_game_button_, &QPushButton::clicked,
            this, &MainWindow::on_new_game_clicked);
    connect(exit_button_, &QPushButton::clicked,
            this, &MainWindow::close);

}

// A function that gets triggered when play cards button
// is clicked
void MainWindow::on_play_clicked()
{
    // return if game is not running
    if (!game_.is_game_ongoing())
    {
        info_label_->setText("Start the game first!");
        return;
    }
    // return if cards are not selected
    if (selected_cards_.empty())
    {
        info_label_->setText("No card(s) selected!");
        return;
    }

    // check if cards can be played
    if (game_.play_cards(selected_cards_))
    {
        clear_selection();
        update_ui();
    }
    else
    {
        info_label_->setText("Invalid card(s).");
    }
}

// Function gets called when new game button is clicked
void MainWindow::on_new_game_clicked()
{
    // gets players amount from the spinbox
    int players_count = player_count_spin_box_->value();
    // get amount of starting hands
    int starting_hand_size = hand_size_spin_box_->value();

    // start game with players amount and hand size
    game_.start_game(players_count, starting_hand_size);

    // enable play and draw buttons
    play_button_->setEnabled(true);
    draw_button_->setEnabled(true);

    // update UI elements
    update_ui();
    update_hand();
}

// Function gets called when draw card button is clicked
void MainWindow::on_draw_clicked()
{
    // check if game is running
    if (!game_.is_game_ongoing()) {
        return;
    }

    // draw card
    game_.draw_card();

    // update visuals
    clear_selection();
    update_ui();
}

// function that updates UI
void MainWindow::update_ui()
{
    // get player and update its hand
    int playerNum = game_.get_current_player()->number;
    update_last_played_card();
    update_hand();

    // win condition check
    if(game_.winner() != nullptr)
    {
        info_label_->setText("Player " + QString::number(playerNum) + " won the game!");
        play_button_->setEnabled(false);
        draw_button_->setEnabled(false);
    }
    else
    {
        info_label_->setText("Player " + QString::number(playerNum) + "'s move.");
    }
}

// a function that resets vector of selected cards
void MainWindow::clear_selection()
{
    selected_cards_.clear();
}

// update visuals of the last played card (near deck)
void MainWindow::update_last_played_card()
{
    // return if game is not running
    if (!game_.is_game_ongoing())
    {
        last_played_card_->clear();
        return;
    }

    // get previously played card data
    UnoCardPtr prev_card = game_.get_previous_card();
    if (!prev_card)
        return;

    // load picture of all cards
    QPixmap full_picture;
    full_picture.load(":/unocardsheet.png");

    const int N_OF_COLUMNS = 15;
    const int N_OF_ROWS = 5;

    int card_width = full_picture.width() / N_OF_COLUMNS;
    int card_height = full_picture.height() / N_OF_ROWS;

    // convert enum to int and calculate coordinates
    int card_x = static_cast<int>(prev_card->type())  * card_width;
    int card_y = static_cast<int>(prev_card->color()) * card_height;

    // crop the image and save it to image variable
    QPixmap image = full_picture.copy(card_x, card_y, card_width, card_height);

    // scale image to match other images on the screen
    QPixmap scaled = image.scaled(
        card_width / 2,
        card_height / 2,
        Qt::KeepAspectRatio,
        Qt::SmoothTransformation
        );

    // update the visual of the last played card label
    last_played_card_->setPixmap(scaled);
    last_played_card_->setFixedSize(card_width_/2, card_height_/2);
}

// function that sets the UI on init
void MainWindow::setup_ui()
{
    QWidget* central = new QWidget(this);
    setCentralWidget(central);

    // the layout of the whole screen
    QVBoxLayout* mainLayout = new QVBoxLayout(central);
    mainLayout->setContentsMargins(8, 8, 8, 8);
    mainLayout->setSpacing(8);

    // controls/settings
    QHBoxLayout* controls_layout = new QHBoxLayout;
    QVBoxLayout* settings_layout = new QVBoxLayout;

    // player amount settings
    QHBoxLayout* player_amount_settings_ = new QHBoxLayout;
    // label
    QLabel* players_amount_label = new QLabel("Players amount:", central);

    // player amount spinbox
    player_count_spin_box_ = new QSpinBox(central);
    player_count_spin_box_->setRange(2, 6);
    player_count_spin_box_->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    // new game button
    new_game_button_ = new QPushButton("New Game", central);
    new_game_button_->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    // add to player settings
    player_amount_settings_->addWidget(players_amount_label);
    player_amount_settings_->addWidget(player_count_spin_box_);

    // starting hand settings
    QHBoxLayout* player_hand_size_settings = new QHBoxLayout;
    // info text
    QLabel* hand_size_label = new QLabel("Starting hand:", central);

    // spinbox for hand size
    hand_size_spin_box_ = new QSpinBox(central);
    hand_size_spin_box_->setRange(1, 20);
    hand_size_spin_box_->setValue(7);
    hand_size_spin_box_->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    // add to starting hand layout
    player_hand_size_settings->addWidget(hand_size_label);
    player_hand_size_settings->addWidget(hand_size_spin_box_);

    // add players amount and hand size to settings layout
    settings_layout->addLayout(player_amount_settings_);
    settings_layout->addLayout(player_hand_size_settings);
    settings_layout->addWidget(new_game_button_);

    // exit btn
    exit_button_ = new QPushButton("Exit", central);

    // setup settings layout
    controls_layout->addLayout(settings_layout);
    controls_layout->addStretch(1);
    controls_layout->addWidget(exit_button_);

    // info textlabel
    info_label_ = new QLabel(central);
    info_label_->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    // played card and deck
    QHBoxLayout* middle_layout = new QHBoxLayout;
    draw_button_ = new QPushButton("", central);
    load_deck_image();

    last_played_card_ = new QLabel(central);
    last_played_card_->setFixedSize(card_width_ / 2, card_height_ / 2);
    last_played_card_->setAlignment(Qt::AlignCenter);

    middle_layout->addWidget(draw_button_);
    middle_layout->addWidget(last_played_card_);

    // play cards btn
    play_button_ = new QPushButton("Play cards", central);
    play_button_->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    // disable buttons before game starts
    play_button_->setEnabled(false);
    draw_button_->setEnabled(false);

    // hand
    hand_container_ = new QWidget(central);
    hand_layout_ = new QHBoxLayout(hand_container_);
    hand_layout_->setSpacing(4);

    // add items to main layout
    mainLayout->addLayout(controls_layout, 0);
    mainLayout->addWidget(info_label_, 2, Qt::AlignHCenter);
    mainLayout->addLayout(middle_layout, 2);
    mainLayout->addWidget(play_button_, 3, Qt::AlignHCenter);
    mainLayout->addWidget(hand_container_, 4);

    // change window title and size
    setWindowTitle("Uno");
    resize(700, 500);
}

// function to load deck image
void MainWindow::load_deck_image()
{
    // load image
    QPixmap full_picture;
    full_picture.load(":/unocardsheet.png");

    // calculate the card width and height
    const int N_OF_COLUMNS = 15;
    const int N_OF_ROWS = 5;
    card_width_ = full_picture.width() / N_OF_COLUMNS;
    card_height_ = full_picture.height() / N_OF_ROWS;

    // use coordinates for the needed card (12,4)
    int card_x = 12 * card_width_;
    int card_y = 4 * card_height_;

    deck_image_ = full_picture.copy(card_x, card_y, card_width_, card_height_);

    // update draw_button visuals
    draw_button_->setIcon(QIcon(deck_image_));
    draw_button_->setIconSize(QSize(card_width_/2, card_height_/2));
    draw_button_->setFixedSize(card_width_/2, card_height_/2);
}

// function that is triggered when card is clicked
void MainWindow::on_card_clicked(CardWidget* cardWidget)
{
    UnoCardPtr card = cardWidget->get_card();

    // if the card is selected add it to selected cards
    if (cardWidget->is_selected())
    {
        selected_cards_.push_back(card);
    }
    // remove card from selection if card is not selected
    else
    {
        auto iter = std::find(selected_cards_.begin(),
                            selected_cards_.end(), card);

        // if iterator exists, remove the card
        if (iter != selected_cards_.end())
        {
            selected_cards_.erase(iter);
        }
    }

    // if there are cards selected yet write an additional check to
    // only allow selection of same type cards
    if (!selected_cards_.empty())
    {
        // get the type of the card that was the first selection
        CardType card_type = selected_cards_.front()->type();

        // compare currently clicked card with previous cards
        if (card->type() != card_type && cardWidget->is_selected())
        {
            // if card doesnt match the type then unselect it and remove
            // from selected cards
            cardWidget->set_selected(false);
            cardWidget->update_visuals();

            // remove the card from selected vector
            auto iter = std::find(selected_cards_.begin(),
                                selected_cards_.end(),
                                card);
            if (iter != selected_cards_.end())
            {
                selected_cards_.erase(iter);
            }
        }
    }
}

// update hand visuals
void MainWindow::update_hand()
{
    // delete previous cards
    clear_hand_ui();

    // if game is not running return
    if (!game_.is_game_ongoing())
    {
        return;
    }

    PlayerPtr current = game_.get_current_player();
    // set ui for cards, using current players cards
    for (const UnoCardPtr& card : current->hand)
    {
        // create new ui card
        CardWidget* ui_card = new CardWidget(hand_container_);
        ui_card->set_card(card);

        // connect the buttong to card widgets signal
        connect(ui_card, &CardWidget::click_signal,
               this, &MainWindow::on_card_clicked);

        // add the card to hand layout
        hand_layout_->addWidget(ui_card);
    }
}

void MainWindow::clear_hand_ui()
{
    // delete old widgets
    QLayoutItem* item;
    // loop until there are old cards in hand
    while ((item = hand_layout_->takeAt(0)) != nullptr)
    {
        QWidget* old_widget = item->widget();
        if (old_widget)
        {
            old_widget->deleteLater();
        }
        delete item;
    }
    // clear selected cards
    selected_cards_.clear();
}
