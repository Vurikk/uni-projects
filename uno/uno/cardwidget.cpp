#include "cardwidget.hh"
CardWidget::CardWidget(QWidget *parent):
    QPushButton(parent)
{
    // connect the clicked QPushButton signal to
    // on click function
    connect(this, &QPushButton::clicked,
            this, &CardWidget::on_click);
}
CardWidget::~CardWidget()
{

}

void CardWidget::set_card(UnoCardPtr card)
{
    card_ = card;
    update_visuals();
}
void CardWidget::set_selected(bool selected)
{
    selected_ = selected;
}
bool CardWidget::is_selected() const
{
    return selected_;
}

void CardWidget::on_click()
{
    // select/unselect
    set_selected(!selected_);

    update_visuals();

    // send a QT signal
    emit click_signal(this);
}

void CardWidget::update_visuals()
{
    if (!card_)
        return;

    QPixmap full_picture;
    full_picture.load(":/unocardsheet.png");

    const int N_OF_COLUMNS = 15;
    const int N_OF_ROWS = 5;

    int card_width = full_picture.width() / N_OF_COLUMNS;
    int card_height = full_picture.height() / N_OF_ROWS;

    // convert enum value to its index and multiply by the width and height
    int card_x = static_cast<int>(card_->type()) * card_width;
    int card_y = static_cast<int>(card_->color()) * card_height;

    image_ = full_picture.copy(card_x, card_y, card_width, card_height);

    // to the card widget btn set image and size
    setIcon(QIcon(image_));

    // border to the card on select
    if (selected_)
    {
        setIconSize(QSize(card_width/2 + 10, card_height/2 + 10));
        setFixedSize(card_width/2 + 13, card_height/2 + 13);
        setStyleSheet("border: 3px solid white;");
    } else
    {
        setIconSize(QSize(card_width/2, card_height/2));
        setFixedSize(card_width/2 + 13, card_height/2 + 13);
        setStyleSheet("border: 0px solid black;");
    }
}

UnoCardPtr CardWidget::get_card() const
{
    return card_;
}
