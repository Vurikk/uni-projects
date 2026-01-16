#ifndef VISIT_HH
#define VISIT_HH

#include "date.hh"
#include <string>
#include <memory>

class Visit
{
public:
    Visit(Date start_date, int room_num);
    ~Visit();

    /**
     * @brief print_time
     * Prints the booking and end time of the visit
     */
    void print_time();

    /**
     * @brief leave
     * @param leave_date the date when the guest left the hotel
     * A function that manages guest leaving from the hotel.
     */
    void leave(Date leave_date);

    /**
     * @brief room_number
     * @return room_number_
     */
    int room_number() const;

    /**
     * @brief next_ pointer to the next visit of a guest
     */
    std::shared_ptr<Visit> next_;

private:
    // time when guest arrived
    Date start_;
    // timere
    Date end_;

    // the room number of a visit
    int room_number_;
};

#endif // VISIT_HH