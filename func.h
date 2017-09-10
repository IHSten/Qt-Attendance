/*******************************************************************************************
 * Created by Ian Steneker using free QtCreator and Designer, as well as Visual Studio 2017
 * Compiled in MinGW32
 * For use at the John Cooper School
 * Summer 2017
********************************************************************************************/

#ifndef FUNC_H
#define FUNC_H

#endif // FUNC_H

#include <iostream>
#include <ctime>
using namespace std;

class Student
{
public:
    string name;
    int returnNumber()
    {
        return number;
    }
    Student();
    Student(string name, int number);
private:
    int number;

};

string printTimeDate()
{
    time_t now = time(0);
    tm* localtm = localtime(&now);
    string output = asctime(localtm);
    return output;
}

string returnMonth()
{
    time_t now = time(0);
    tm* localtm = localtime(&now);
    string extraction = asctime(localtm);
    string month = extraction.substr(4, 3);
    return month;
}

int returnYear()
{
    time_t now = time(0);
    tm* localtm = localtime(&now);
    string extraction = asctime(localtm);
    int year = stoi(extraction.substr(22,2));
    return year;
}

int compareMonth() //Helps determine if a student is eligible to go to lunch
{
    string m = returnMonth();
    if (m == "Jan" || m == "Feb" || m == "Mar" || m == "Apr" || m == "May" || m == "Jun")
    {
        cout << "Jan-Jun";
        return 1;
    }
    else if (m == "Jul" || m == "Aug" || m == "Sep" || m == "Oct" || m == "Nov" || m == "Dec")
    {
        cout << "Jul-Dec";
        return 2;
    }
    else
    {
        return 3;
    }
}
