#include "calendar.hh"
#include <iostream>
#include <iomanip>
#include <algorithm>


using namespace std;

Calendar::Calendar()
{

}

/* A helper function that sorts events by time.
 * Returns true if event1 should come before event2.
 */
bool compare_events_by_time(const shared_ptr<Event>& event1, const shared_ptr<Event>& event2)
{
    if (event1->start() != event2->start()) {
        return event1->start() < event2->start();
    }
    return event1->end() < event2->end();
}
/* A helper function that sorts events for a given date. */
void Calendar::sort_events(Date date)
{
    map<Date, vector<shared_ptr<Event>>>::iterator iter = events_.find(date);
    // get hold of events in todays date
    vector<shared_ptr<Event>>& events_today = iter->second;

    // sorting events by their time
    sort(events_today.begin(), events_today.end(), compare_events_by_time);
}

/* Constructs and adds an event for the current date.
 * Returns false if times are invalid or start >= end.
 */
bool Calendar::add_event(std::string name, Time::Minutes start, Time::Minutes end,
               std::string description)
{
    // Invalid input handling
    if(start >= end)
    {
        cout << "Error: invalid time(s)." << endl;;
        return false;
    }
    if(!Time::is_valid(start) || !Time::is_valid(end))
    {
        cout << "Error: invalid time(s)." << endl;;
        return false;
    }

    // create an instance of an event and add it
    shared_ptr new_event = make_shared<Event>(
        chosen_date_, name, start, end, description
        );
    events_[chosen_date_].push_back(new_event);

    // sort events
    sort_events(chosen_date_);
    return true;
}
bool Calendar::add_event(std::string name, Time::Minutes start, Time::Minutes end,
                         std::string description, Date date_for_event)
{
    // Invalid input handling
    if(start >= end)
    {
        cout << "Error: invalid time(s)." << endl;;
        return false;
    }
    if(!Time::is_valid(start) || !Time::is_valid(end))
    {
        cout << "Error: invalid time(s)." << endl;;
        return false;
    }

    // create an instance of an event and add it
    shared_ptr new_event = make_shared<Event>(
        chosen_date_, name, start, end, description
        );
    events_[date_for_event].push_back(new_event);

    // sort events
    sort_events(date_for_event);
    return true;
}


/* Changes the currently chosen date. */
void Calendar::change_date(Date new_date)
{
    // check if new_date is not valid
    if(!Date::is_valid_date(chosen_date_.day(), chosen_date_.month(), chosen_date_.year()))
    {
        // return the function and print error message
        cout << "Error: incorrect parameter." << endl;
        return;
    }

    // if the new date is valid set chosen date to new date
    chosen_date_ = new_date;
}

/* Changes the currently chosen date's month, keeping day/year where possible. */
bool Calendar::change_month(int month)
{
    // check if new date is not valid
    if(!Date::is_valid_date(chosen_date_.day(), month, chosen_date_.year()))
    {
        // the day is outside of new month. Update it
        int month_last_day = Date::days_in_month(month, chosen_date_.year());
        Date new_date(month_last_day, month, chosen_date_.year());
        chosen_date_ = new_date;
        return true;
    }

    // create a new date where only month is changed
    Date new_date(chosen_date_.day(), month, chosen_date_.year());

    // if the new date is valid set chosen date to new date
    chosen_date_ = new_date;
    return true;
}

/* Changes the currently chosen date's day, within the same month/year. */
bool Calendar::change_day(int day)
{
    // check if new date is not valid
    if(!Date::is_valid_date(day, chosen_date_.month(), chosen_date_.year()))
    {
        // return the function and print error message
        return false;
    }

    // create a new date where only day is changed
    Date new_date(day, chosen_date_.month(), chosen_date_.year());

    // if the new date is valid set chosen date to new date
    chosen_date_ = new_date;
    return true;
}

/* Prints out a calendar view of the current month. */
void Calendar::print_month()
{
    cout << "  Mo  Tu  We  Th  Fr  Sa  Su\n";

    // get first day of month
    Date first_day_date(1, chosen_date_.month(), chosen_date_.year());
    int first_day = first_day_date.day_of_week();
    // sunday = 0
    if(first_day <= 0)
    {
        first_day = 7;
    }

    // offset to the first day
    for (int i = 0; i < first_day - 1; ++i) {
        cout << std::string(4, ' ');
    }

    // helper int to set correct layout for the calendar.
    int current_week_day = first_day - 1;
    if(current_week_day <= 0)
    {
        current_week_day = 6;
    }

    // print days
    for (int day = 1; day <= Date::days_in_month(chosen_date_.month(), chosen_date_.year()); ++day)
    {
        bool printed = false;

        // is used for correct width setting.
        int width = day >= 10 ? 2 : 3;

        // check if day is current day
        if(day == chosen_date_.day())
        {
            cout << setw(width)<< '^' << day;
            printed = true;
        }
        // check if day has events
        else
        {
            // check if the day has events
            Date current_date(day, chosen_date_.month(), chosen_date_.year());
            map<Date, vector<shared_ptr<Event>>>::iterator iter = events_.find(current_date);
            if (iter != events_.end() && !iter->second.empty())
            {
                cout  << setw(width)<< '*' << day;
                printed = true;
            }
        }

        if(!printed)
            cout << setw(4) << day;

        // if reached sunday then end the line and go to monday
        current_week_day++;
        if(current_week_day == 7)
        {
            current_week_day = 0;
            cout << endl;
        }
    }

    // if month didnt end on sunday add one extra line
    if(current_week_day != 0)
        cout << endl;
}



/* Prints out the schedule for the current date's events. */
void Calendar::print_day()
{
    // finds events for currently chosen date
    map<Date, vector<shared_ptr<Event>>>::iterator iter = events_.find(chosen_date_);
    if (iter == events_.end() || iter->second.empty()) {
        cout << "No events for the day." << endl;
        return;
    }

    // now we can get todays events from iterator
    vector<shared_ptr<Event>> events_today = iter->second;

    int current_event = 1;
    for(const shared_ptr<Event>& event : events_today)
    {
        cout << '(' << current_event << ") " <<
            Time::to_string(event->start()) << " - " <<
            Time::to_string(event->end()) <<
            " \"" << event->name() << "\"" << endl;
        current_event++;
    }
}


/* Prints details of the i:th event of the selected day. */
bool Calendar::print_event(int i)
{
    // early exit
    if(i <= 0)
        return false;

    map<Date, vector<shared_ptr<Event>>>::iterator iter = events_.find(chosen_date_);
    // no events in the day
    if (iter == events_.end() || iter->second.empty()) {
        return false;
    }

    const vector<shared_ptr<Event>>& events_today = iter->second;

    // check if i is inside the events vector
    if(i > (int)size(events_today))
        return false;

    const shared_ptr<Event>& event = events_today.at(i - 1);
    cout << '\"' << event->name() << "\" from " <<
        Time::to_string(event->start()) << " to " <<
        Time::to_string(event->end()) << endl <<
        "  " << event->description() << endl;
    return true;
}


/* Deletes the i:th event of the selected day. */
bool Calendar::delete_event(int i)
{
    // early exit
    if(i <= 0)
        return false;

    map<Date, vector<shared_ptr<Event>>>::iterator iter = events_.find(chosen_date_);
    // no events in the day
    if (iter == events_.end() || iter->second.empty()) {
        return false;
    }

    vector<shared_ptr<Event>>& events_today = iter->second;

    // check if i is inside the events vector
    if(i > (int)size(events_today))
        return false;

    // delete event from events
    events_today.erase(events_today.begin() + (i - 1));

    //additionaly if there are no more events in the day, remove the whole Date from map
    if (events_today.empty()) {
        events_.erase(iter);
    }

    return true;
}


/* Moves the i:th event of the selected day to a new date. */
bool Calendar::move_event(int i, Date new_date)
{
    // early exit
    if(i <= 0)
        return false;

    map<Date, vector<shared_ptr<Event>>>::iterator iter = events_.find(chosen_date_);
    // no events in the day
    if (iter == events_.end() || iter->second.empty()) {
        return false;
    }

    const vector<shared_ptr<Event>>& events_today = iter->second;

    // check if i is inside the events vector
    if(i > (int)size(events_today))
        return false;

    const shared_ptr<Event>& event = events_today.at(i - 1);

    // check if time is valid
    if(!Date::is_valid_date(new_date.day(), new_date.month(), new_date.year()))
    {
        return false;
    }

    // set new date to the event
    event->set_date(new_date);

    // add event to the moved date
    add_event(event->name(), event->start(), event->end(), event->description(), new_date);

    // delete event from the current day
    delete_event(i);


    return true;
}


/* Getter for the currently chosen date. */
Date Calendar::chosen_date()
{
    return chosen_date_;
}
/* Getter for the events count. */

int Calendar::events_count(Date date)
{
    map<Date, vector<shared_ptr<Event>>>::iterator iter = events_.find(date);
    // no events in the day
    if (iter == events_.end() || iter->second.empty()) {
        return 0;
    }

    const vector<shared_ptr<Event>>& events_today = iter->second;
    return (int)size(events_today);
}
