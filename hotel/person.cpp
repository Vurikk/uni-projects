#include "person.hh"
#include <string>
#include <memory>
#include <iostream>
using namespace std;

Person::Person(string name)
{
    name_ = name;
}

/*
 * A function that handles persons new visits
 */
void Person::make_visit(shared_ptr<Visit>& visit)
{
    staying = true;
    total_visits_++;
    if(first_visit_ == nullptr)
    {
        first_visit_ = visit;
        last_visit_ = visit;
    }
    else
    {
        last_visit_->next_ = visit;
        last_visit_ = visit;
    }
}
/*
 * Function that handles guest leaving
 */
void Person::leave(Date leave_date)
{
    staying = false;
    last_visit_->leave(leave_date);
}
/*
 * Function that prints all visits of a guest and their times
 */
void Person::print() const
{
    shared_ptr<Visit> current = first_visit_;
    while(current != nullptr)
    {
        cout << "* Visit: ";
        current->print_time();
        cout << endl;
        current = current->next_;
    }
}
/*
 * Returns total visits.
 */
int Person::visits() const
{
    return total_visits_;
}
