#include <iostream>

using namespace std;

// This C++ code implements a Time class that keeps track of the hours, minutes, and seconds. It calculates the
// number of seconds past midnight, the difference in the two times given past minute, and then takes a user input
// of the number of seconds and returns the two new times with the user's number of seconds

//-----------------
// Time Class
//-----------------

// Class Declaration
class Time
{
private:
    int hours;
    int minutes;
    int seconds;
public:
    Time(int, int, int); // constructor for user input for hours, minutes, and seconds
    Time(int = 0); // constructor for # of seconds past midnight -- if no value --> 0
    void show(); // prints hours, minutes, and seconds
    int secondsPastMidnight(); // number of seconds past midnight
    int operator>(Time);  // time1 is later than time2 --> 1 (if not --> call secondsPastMidnight)
    Time operator-(Time); // time1 is later than time2 --> difference in times (else: all mem var = 0)
};

// Class implementation - constructor that takes in three arguments
Time::Time(int hr, int min, int sec)
{
    int total = sec + (min*60) + (hr*60*60); // calculates the total seconds past midnight
    // checks to make sure the time2 is not negative
    if (total < 0)
    {
        hours = 0;
        minutes = 0;
        seconds = 0;
    }
    else
    {
        // ensures that the time does not surpass 23 hours
        if ((total / 3600) > 23)
        {
            // takes the remainder from dividing by 24 and stores into hours
            hours = (total / 3600) % 24;
        }
        else
        {
            hours = total / 3600;
        }
        // calculates minutes and seconds based on the total seconds past midnight
        minutes = (total % 3600) / 60;
        seconds = total % 60;
    }
}

// constructor that takes in one argument (seconds)
Time::Time(int secPastMid)
{
    // calls the first constructor
    // we put 0 for hours and minutes as the only input is seconds
    *this = Time(0, 0, secPastMid);
}

// getter
void Time::show()
{
    // the if and else statements check to see if the hours, minutes, and seconds are one or two digits
    // if one digit, add a space in front of the number; otherwise no space
    if (hours < 10)
    {
        cout << " " << hours << " h : ";
    }
    else
    {
        cout << hours << " h : ";
    }
    if (minutes < 10)
    {
        cout << " " << minutes << " m : ";
    }
    else
    {
        cout << minutes << " m : ";
    }

    if (seconds < 10)
    {
        cout << " " << seconds << " s";
    }
    else
    {
        cout << seconds << " s";
    }
}

// returns an int that calculates the seconds past midnight
int Time::secondsPastMidnight()
{
    return (seconds + (minutes*60) + (hours*60*60));
}

// defines the operator >
// time1 is defined and timeOnTheRight is time2 in this case (doesn't depend on the names ex: time1 and time2)
int Time::operator>(Time timeOnTheRight)
{
    if (secondsPastMidnight() > timeOnTheRight.secondsPastMidnight())
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

// returns a Time object that takes the difference between time1 and time2
Time Time::operator-(Time timeOnTheRight)
{
    return Time(secondsPastMidnight() - timeOnTheRight.secondsPastMidnight());
}


int main()
{
    // declares the variables for our inputs
    int hr, min, sec, num_seconds;
    // this declaration is for our operator-
    Time dif_time;
    cout << "Enter a time (h m s) without parentheses: ";
    cin >> hr >> min >> sec;
    // creates a new object time1
    Time time1(hr, min, sec);
    time1.show();

    cout << "\nEnter another time (h m s) without parentheses: ";
    cin >> hr >> min >> sec;
    Time time2(hr, min, sec);
    time2.show();
    cout << "\nEnter a number of seconds: ";
    cin >> num_seconds;

    cout << "\n1st time is " << time1.secondsPastMidnight() << " seconds past midnight.";
    cout << "\n2nd time is " << time2.secondsPastMidnight() << " seconds past midnight.";

    // uses the operator- in order to get the difference in times
    if (time1 > time2)
    {
        dif_time = time1 - time2;
    }
    else
    {
        dif_time = time2 - time1;
    }

    cout << "\n\nDifference in times is ";
    dif_time.show();
    cout << "\nwhich is " << dif_time.secondsPastMidnight() << " seconds past midnight.";

    Time first_plus(num_seconds + time1.secondsPastMidnight());
    cout << "\n\n1st time plus " << num_seconds << " seconds is ";
    first_plus.show();

    Time second_plus(num_seconds + time2.secondsPastMidnight());;
    cout << "\n2nd time plus " << num_seconds << " seconds is ";
    second_plus.show();
    cout << "\n";
}
