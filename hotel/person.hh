#ifndef PERSON_HH
#define PERSON_HH

#include <string>
#include <memory>
#include "visit.hh"
#include "date.hh"


class Person
{
public:
    Person(std::string name);

    /**
     * @brief make_visit Creates a new visit
     * @param visit shared pointer to the Visit class
     */
    void make_visit(std::shared_ptr<Visit>& visit);

    /**
     * @brief leave
     * @param leave_date the date when guest left the hotel
     * A function that handles guest leaving from the hotel
     */
    void leave(Date leave_date);

    /**
     * @brief print
     * Function that handles printing data about the guest.
     */
    void print() const;

    /**
     * @brief visits
     * @return total_visits_
     * Returns total visits of the guest
     */
    int visits() const;


private:
    // guests name
    std::string name_;
    bool staying = false;
    int total_visits_ = 0;

    // pointer to the last visit of the guest
    std::shared_ptr<Visit> last_visit_ = nullptr;

    // pointer to the first visit of the guest
    std::shared_ptr<Visit> first_visit_ = nullptr;

};

#endif // PERSON_HH
