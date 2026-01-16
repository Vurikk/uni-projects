#ifndef EVENT_HH
#define EVENT_HH

#include "date.hh"
#include "time.hh"

#include <string>

class Event
{
public:
    // Constructor
    Event(Date date, std::string name_, Time::Minutes start,
        Time::Minutes end, std::string description);

    // getters
    const Date& date() const;
    const std::string& name() const;
    const std::string& description() const;
    Time::Minutes start() const;
    Time::Minutes end() const;

    // sets event date to new date
    void set_date(const Date& new_date);

private:
    Date date_;
    std::string name_;
    Time::Minutes start_;
    Time::Minutes end_;
    std::string description_;
};

#endif // EVENT_HH