#include <QtTest>
#include "../calendar.hh"
#include "../date.hh"
#include "../time.hh"
#include "../event.hh"
#include <memory>

// add necessary includes here

class calendar_test : public QObject
{
    Q_OBJECT

public:
    calendar_test();
    ~calendar_test();

private slots:

    // Test 1: change date
    void changeDate_changesChosenDate();

    // Test 2: create & delete events
    void create_and_delete_event();

    // Test 3: testing calendar functionality with inputs (data-driven)
    void calendar_testing_data();
    void calendar_testing();

private:
    std::shared_ptr<Calendar> calendar_;
};

calendar_test::calendar_test() {}

calendar_test::~calendar_test() {}

// Test 1
void calendar_test::changeDate_changesChosenDate()
{
    // create a new calendar
    calendar_.reset();
    calendar_ = make_shared<Calendar>();

    // create dates
    Date date1(1, 1, 2000);
    Date date2(13, 9, 2024);

    // change calendars date to d1 and compate with d1
    calendar_->change_date(date1);   // or whatever your function returns
    QVERIFY2(calendar_->chosen_date() == date1, "Method date is wrong when comparing it to date1");

    // change calendars date to d1 and compate with d1
    calendar_->change_date(date2);
    QVERIFY2(calendar_->chosen_date() == date2, "Method date is wrong when comparing it to date2");

    // if both succeed, means that change_date works correctly
}

// Test 2
void calendar_test::create_and_delete_event()
{
    // create a new calendar
    calendar_.reset();
    calendar_ = make_shared<Calendar>();

    Date date1(1, 1, 2000);
    Time::Minutes start_time(120);
    Time::Minutes end_time(160);

    calendar_->change_date(date1);
    // check if day has events
    QCOMPARE(calendar_->events_count(date1), 0);

    // add event with wrong parameters (end time > start time)
    // this should print (Error: wrong times) in the console (comes from the calendar.hh)
    calendar_->add_event("Test event", end_time, start_time, "This is a test event");
    // check event amount. Should be 0
    QCOMPARE(calendar_->events_count(date1), 0);

    // add event with correct parameters
    bool event_added = calendar_->add_event("Test event", start_time, end_time, "This is a test event");
    QVERIFY2(event_added, "Event could not be added");
    // compare events amount. Should be 1
    QCOMPARE(calendar_->events_count(date1), 1);

    // delete event
    bool deleted = calendar_->delete_event(1);
    QVERIFY2(deleted, "Event could not be deleted");
    QVERIFY2(calendar_->events_count(date1) == 0,
             "Event count after deletion is not 0! Event deletion failed.");
}

// Test 3
void calendar_test::calendar_testing_data()
{
    // create a new calendar
    calendar_.reset();
    calendar_ = std::make_shared<Calendar>();


    QTest::addColumn<QString>("event_name");
    QTest::addColumn<int>("time1");
    QTest::addColumn<int>("time2");
    QTest::addColumn<QString>("event_description");
    QTest::addColumn<int>("expected_amount_of_events");
    // the expected result of event (correct/incorrect)
    QTest::addColumn<bool>("successful");


    // create first event Event(name, time1, time2, description)
    QTest::newRow("Create first event")
        << QString("Test event")
        << 600
        << 660
        << QString("this is just a test")
        << 1
        << true;

    // create second event starting earlier
    QTest::newRow("Create second event that should become the earliest event")
        << QString("Earlier event")
        << 500
        << 800
        << QString("this event should be the first one")
        << 2
        << true;


    // create third event with incorrect parameters
    // this should print (Error: wrong times) in the console (comes from the calendar.hh)
    QTest::newRow("Create third event which has incorrect parameters")
        << QString("My birthday")
        << 100
        << 10
        << QString("this event is incorrect")
        << 2
        << false;
}
void calendar_test::calendar_testing()
{
    Date d(1,1,1999);
    calendar_->change_date(d);

    QFETCH(QString, event_name);
    QFETCH(int, time1);
    QFETCH(int, time2);
    QFETCH(QString, event_description);
    QFETCH(int, expected_amount_of_events);
    QFETCH(bool, successful);

    // create an event
    bool event_added = calendar_->add_event(event_name.toStdString(),
                                            Time::Minutes(time1),
                                            Time::Minutes(time2),
                                            event_description.toStdString());

    // check if event was created as required
    QVERIFY2(event_added == successful, "Event creation was incorrect.");
    // compare events amount to the expected amount
    QCOMPARE(calendar_->events_count(calendar_->chosen_date()), expected_amount_of_events);

}

QTEST_APPLESS_MAIN(calendar_test)

#include "tst_calendar_test.moc"
