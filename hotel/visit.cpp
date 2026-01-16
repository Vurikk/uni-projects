#include "visit.hh"
#include "date.hh"
#include <iostream>
using namespace std;

Visit::Visit(Date start_date, int room_num)
    : next_(nullptr),
    start_(start_date),
    room_number_(room_num)
{
}

Visit::~Visit()
{
}
/*
 * Prints visit time in format start - end
 */
void Visit::print_time()
{
    start_.print();
    cout << " - ";
    if(!end_.is_default())
        end_.print();
}

/*
 * Sets the end_ date to leave date
 */
void Visit::leave(Date leave_date)
{
    end_ = leave_date;
}
/*
 * A getter function for room number value.
 */
int Visit::room_number() const
{
    return room_number_;
}
