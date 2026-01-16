#include "event.hh"


Event::Event(
    Date date, string name, Time::Minutes start,
    Time::Minutes end, std::string description):
    date_(date), name_(name), start_(start),
    end_(end), description_(description)
{
}

// getters
const Date& Event::date() const
{
    return date_;
}
const std::string& Event::name() const
{
    return name_;
}
const std::string& Event::description() const
{
    return description_;
}
Time::Minutes Event::start() const
{
    return start_;
}
Time::Minutes Event::end() const
{
    return end_;
}

// sets event date to new_date
void Event::set_date(const Date& new_date)
{
    date_ = new_date;
}
