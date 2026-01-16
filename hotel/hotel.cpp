#include "hotel.hh"
#include "utils.hh"
#include "person.hh"
#include <iostream>
#include <fstream>
#include <algorithm>

// Error and information outputs
const string FILE_NOT_FOUND = "Error: Input file not found."s;
const string WRONG_FORMAT = "Error: Wrong format in file."s;
const string NOT_NUMERIC = "Error: Wrong type of parameters."s;
const string ALREADY_EXISTS = "Error: Already exists: "s;
const string CANT_FIND = "Error: Can't find anything matching: "s;
const string GUEST_ENTERED = "A new guest has entered."s;
const string GUEST_LEFT = "Guest left hotel, visit closed."s;
const string FULL = "Error: Can't book, no such rooms available."s;

Hotel::Hotel()
{
    //cout << "Hotel constructor" << endl;
}

Hotel::~Hotel()
{
    //cout << "Hotel destructor" << endl;
}

bool Hotel::init()
{
    cout << "Input file: ";
    string file_name = "";
    getline(cin, file_name);
    ifstream file(file_name);
    if ( not file )
    {
        cout << FILE_NOT_FOUND << endl;
        return false;
    }

    string line = "";
    while ( getline(file, line) )
    {
        vector<string> parts = utils::split(line, ';');
        if ( parts.size() != 2 )
        {
            cout << WRONG_FORMAT << endl;
            return false;
        }
        if ( not ( utils::is_numeric(parts.at(0), false) and
                   utils::is_numeric(parts.at(1), false) ) )
        {
            cout << NOT_NUMERIC << endl;
            return false;
        }
        // create rooms logic
        unsigned int amount = stoi(parts.at(1));
        for ( unsigned int i = 0; i < amount; ++i )
        {
            unsigned int size = stoi(parts.at(0));
            add_room(rooms_.size() + 1, size);
        }
    }
    return true;
}
/*
* Function that sets the date to given date.
*/
void Hotel::set_date(Params params)
{
    string day = params.at(0);
    string month = params.at(1);
    string year = params.at(2);
    if ( not utils::is_numeric(day, false) or
         not utils::is_numeric(month, false) or
         not utils::is_numeric(year, false) )
    {
        cout << NOT_NUMERIC << endl;
        return;
    }
    utils::today.set(stoi(day), stoi(month), stoi(year));
    cout << "Date has been set to ";
    utils::today.print();
    cout << endl;
}

void Hotel::advance_date(Params params)
{
    string amount = params.at(0);
    if ( not utils::is_numeric(amount, true) )
    {
        cout << NOT_NUMERIC << endl;
        return;
    }
    utils::today.advance(stoi(amount));
    cout << "New date is ";
    utils::today.print();
    cout << endl;
}

/*
 * Function that prints info about all the hotel rooms
 */
void Hotel::print_rooms(Params  /*params*/)
{
    for(const Room& room : rooms_){
        int available_spaces = room.size - room.current_visitors;
        // check if the room has spaces if no then set as "full" else  set as remaining spaces
        string availability_string =
            available_spaces == 0 ? "full" : "available for " + to_string(available_spaces)
                                             + " person(s)";
        cout
            << "Room " << room.room_num << " : for " << room.size
            << " person(s) : " << availability_string
            << endl;
    }
}
/**
 * @brief Hotel::get_first_room_by_size
 * @param size the room size requested
 * @return room index
 * A helper function to find the best room
 */
int Hotel::get_first_room_by_size(int size){
    int best_index = -1;
    for (int i = 0; i < (int)rooms_.size(); ++i) {
        const Room& room = rooms_.at(i);

        // room size doesnt mactch the requested size -> continue
        if(room.size != size)
            continue;

        int available_space = room.size - room.current_visitors;
        // no space available in the room -> continue
        if (available_space <= 0)
            continue;

        // no current best room selected. Select current one
        if (best_index == -1)
        {
            best_index = i;
        }
        else
        {
            const Room& best_room = rooms_.at(best_index);

            // if current room has more visitors in it then best room is current
            if(room.current_visitors < best_room.current_visitors)
            {
                best_index = i;
            }
            // if rooms have same amount of visitors select the one with lower room id
            else if(room.current_visitors == best_room.current_visitors &&
                     room.room_num < best_room.room_num)
            {
                best_index = i;
            }
        }
    }
    return best_index;
}
/*
 * A function that handles guest room booking.
 */
void Hotel::book(Params params)
{
    bool guest_exists = false;
    string guest_name = params.at(0);

    string room_num = params.at(1);
    // check if room number is numeric
    if(!utils::is_numeric(room_num, true))
    {
        cout << NOT_NUMERIC << endl;
        return;
    }

    // create a new person
    Person new_person = Person(guest_name);
    //check if guest already exists
    if(all_guests_.find(guest_name) != all_guests_.end())
    {
        guest_exists = true;
        // set the new person to the already existing one
        new_person = all_guests_.find(guest_name)->second;
        // check if the existing person is already staying at the hotel
        if(new_person.staying)
        {
            cout << ALREADY_EXISTS << guest_name << endl;
            return;
        }
    }

    int room_size = stoi(room_num);
    // get best room based on the size requested
    int room_index = get_first_room_by_size(room_size);
    // room doesnt exist
    if(room_index == -1)
    {
        cout << FULL << endl;
        return;
    }

    // Init a new visit
    shared_ptr<Visit> new_visit = make_shared<Visit>(utils::today, room_index);
    new_visit->next_ = nullptr;

    // Create person and assign visit
    new_person.make_visit(new_visit);
    // if guest doesnt exist, create a new person and add it to the hotel all guests
    if(!guest_exists)
        all_guests_.insert({guest_name, new_person});
    // if person exists, modify it
    else
        all_guests_.at(guest_name) = new_person;

    // Assing the used size to the room
    rooms_.at(room_index).current_visitors++;

    cout << GUEST_ENTERED << endl;
}
/*
 * A function that handles guest leaving from the hotel.
 */
void Hotel::leave(Params params)
{
    string guest_name = params.at(0);
    //check if guest already exists
    if(all_guests_.find(guest_name) == all_guests_.end())
    {
        cout << CANT_FIND << guest_name << endl;
        return;
    }

    // get hold of the guest from the hotel guests database
    Person& guest = all_guests_.find(guest_name)->second;
    guest.leave(utils::today);

    // remove the guest from the room
    int room_index = guest.last_visit_->room_number();
    rooms_.at(room_index).current_visitors--;

    cout << GUEST_LEFT << endl;

}
/*
 * Prints info about the guest and all visits
 */
void Hotel::print_guest_info(Params params)
{
    string guest_name = params.at(0);
    // check if the guest exists in the hotel guests database
    if(all_guests_.find(guest_name) == all_guests_.end())
    {
        cout << CANT_FIND << guest_name << endl;
        return;
    }
    // print guest visits
    all_guests_.find(guest_name)->second.print();
}
/*
 * Prints all the guests and their visits
 */
void Hotel::print_all_visits(Params /*params*/)
{
    // if there are no guests return and print None
    if(all_guests_.size() == 0)
    {
        cout << "None" << endl;
        return;
    }
    // go through all guests in the hotel
    for(const pair<const string, Person>& guest : all_guests_)
    {
        // print the guest name
        cout << guest.first << endl;
        // print all visits of the guest
        guest.second.print();
    }
}
/*
 * prints all current visitors in the hotel
 */
void Hotel::print_current_visits(Params /*params*/)
{
    // if there are no guests return and print None
    if(all_guests_.size() == 0)
    {
        cout << "None" << endl;
        return;
    }

    bool has_guests_staying = false;
    // go through every guest in the hotel
    for(const pair<const string, Person>& guest : all_guests_)
    {
        // check if guest is currently staying at the hotel
        if(guest.second.staying)
        {
            cout << guest.first << " is boarded in Room " << guest.second.last_visit_->room_number() + 1 << endl;
            // set to true
            has_guests_staying = true;
        }
    }
    // if no guests are staying in the hotel print None
    if(!has_guests_staying)
        cout << "None" << endl;
}
/*
 * Prints guests with the most visits.
 */
void Hotel::print_honor_guests(Params /*params*/)
{
    // check if hotel has any guests
    if(all_guests_.size() == 0)
    {
        cout << "None" << endl;
        return;
    }

    // the most visits a guest made
    int max_visits = 0;
    vector<string> honorable_guests;
    // go through each guest in the hotel
    for(const pair<const string, Person>& guest : all_guests_)
    {
        // get total amount of guest visits
        int guest_total_visits = guest.second.visits();
        if(guest_total_visits > max_visits)
        {
            // the guest has the most visits -> clear honorable guests list
            max_visits = guest_total_visits;
            honorable_guests.clear();
        }
        // add guest to honorable guests if it has the most visits
        if(guest_total_visits ==  max_visits)
            honorable_guests.push_back(guest.first);
    }

    // sort the honorable guests alphabetically
    sort(honorable_guests.begin(), honorable_guests.end());
    cout << "With " << max_visits << " visit(s), the following guest(s) get(s) honorary award:" << endl;
    // go through all the honorable guests and print their names
    for(const string& guest : honorable_guests)
    {
        cout << " * " << guest << endl;
    }

}
/*
 * Function that adds a room to the hotel
 */
void Hotel::add_room(int room_num, int size){
    Room new_room;
    new_room.room_num = room_num;
    new_room.size = size;
    new_room.current_visitors = 0;

    rooms_.push_back(new_room);
}
