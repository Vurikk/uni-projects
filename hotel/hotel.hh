/* Class Hotel
 * ----------
 * COMP.CS.110 FALL 2025
 * ----------
 * Class for describing a hotel. Only a single object created from this
 * class. Implements all commands in hotel program.
 *
 * Note: Students need change this class to implement commands missing
 * in the template code.
 * */
#ifndef HOTEL_HH
#define HOTEL_HH

#include "person.hh"
#include <vector>
#include <map>

using namespace std;
using Params = const vector<string>&;

struct Room{
    int room_num;
    int size;
    int current_visitors = 0;
};

class Hotel
{
public:
    /**
     * @brief Hotel
     */
    Hotel();

    /**
      * @brief destructor
      */
    ~Hotel();

    /**
     * @brief init
     * Initializes a hotel, i.e. fills it with rooms found from the input file
     */
    bool init();

    /**
     * @brief set_date
     * @param params vector containing parameters of the corresponding command
     */
    void set_date(Params params);

    /**
     * @brief advance_date
     * @param params vector containing parameters of the corresponding command
     * Advances the current date with the given number of days.
     */
    void advance_date(Params params);

    /**
     * @brief print_rooms
     */
    void print_rooms(Params params);

    /**
     * @brief add_room adds a room to the rooms_ vector list.
     * @param room_num
     * @param size is the max amount of visitors for a room
     */
    void add_room(int room_num, int size);

    /**
     * @brief book
     * @param params vector containing parameters of the corresponding command
     * Adds a guest in the hotel and creates a new visit.
     * If the guest given as a parameter has never visited the hotel earlier,
     * creates a new person pointer, otherwise just adds an existing
     * person in the newly created visit.
     */
    void book(Params params);

    /**
     * @brief leave
     * @param params vector containing parameters of the corresponding command
     * Removes the guest given as a parameter from the hotel, and closes
     * guest's visits. However, the guest still remains in all_visits_.
     */
    void leave(Params params);

    /**
     * @brief print_guest_info
     * @param params vector containing parameters of the corresponding command
     * Prints the given guest's all visits.
     */
    void print_guest_info(Params params);

    /**
     * @brief print_all_visits
     * Prints all guests visited the hotel at some time, i.e. all
     * current and earlier guests.
     * More precisely, prints each guest's id and visiting period
     */
    void print_all_visits(Params);

    /**
     * @brief print_current_visits
     * Prints the current guests' ids and room numbers.
     */
    void print_current_visits(Params params);

    /**
     * @brief print_honor_guests
     * Prints the ids of those guests that have visited the hotel most often.
     */
    void print_honor_guests(Params);



private:
    // helper function to get the first room by its requested size
    int get_first_room_by_size(int size);
    // all rooms in the hotel
    vector<Room> rooms_;
    // current hotel guests
    map<string, Person> all_guests_;

};

#endif // HOTEL_HH
