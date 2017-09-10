/*******************************************************************************************
 * Created by Ian Steneker using free QtCreator and Designer, as well as Visual Studio 2017
 * Compiled in MinGW32
 * For use at the John Cooper School
 * Summer 2017
********************************************************************************************/

#include "attendance.h"
#include "ui_attendance.h"
#include "stdlib.h"
#include "func.h"
#include <fstream>
#include <QMessageBox>
#include <QTWidgets>
#include <Qstring>
#include <QGroupBox>
#include <QIntValidator>
#include <QApplication>
#include <QImage>

attendance::attendance(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::attendance)
{
    ui->setupUi(this);
    ui->studNum->setValidator( new QIntValidator);
    //QImage *logo = new QImage*(":/400pxwhite.png");
    //if(logo->isNull()) qDebug ("null");
}

attendance::~attendance()
{
    delete ui;
}

string appdata(getenv("APPDATA"));

//Location of CSV file that contains necessary student details - may be moved to a secure network share
string dbLocation = appdata + "\\Cplus Attendance Data\\CPlus Attendance Data\\students.csv";

//Location of saved log files - Changes daily to organize logs by date.
string logLocation = "C:/database/log - " + returnMonth() + " " + printTimeDate().substr(8,2) + " 20" + std::to_string(returnYear()) + ".txt";

//Default Constructor
Student::Student()
{
    name = "John Smith";
    number = 000000;
}

//Constructor
Student::Student(string studName, int studNumber)
{
    name = studName;
    number = studNumber;
}

//Vector that holds all student details
vector<Student> studentList;

//Fills studentList with needed info.
void intakeStudentNumbers()
{
    ifstream din;
    din.open(dbLocation);
    if (din.is_open())
    {
        string dbIntake;

        while (getline(din, dbIntake))
        {
            if (dbIntake.substr(0, 5) == "StsSt")
            {
                //cout << "File format recognized, file begins with StsSt.";
            }
            else
            {
                int numIn = stoi(dbIntake.substr(dbIntake.length()-6));
                string nameLast = dbIntake.substr(0, dbIntake.find(','));
                string nameFirst = dbIntake.substr(dbIntake.find(',') + 1, dbIntake.length()-nameLast.length()-8);
                string nameIn = nameFirst + " " + nameLast;
                Student studentIn(nameIn, numIn);
                studentList.push_back(studentIn);
            }
        }
        din.close();
    }
}

//Number that reserves the current students place in the vector - should be reinitialized every time the submit button is pressed.
int listNumber;

//Assigns listNumber to the correct value and returns true if testNum is found in the database.
bool parseStudentNumbers(vector<Student> students, int testNum)
{
    for (int i = 0; i < students.size(); i++)
    {
        int studentNum = students[i].returnNumber();
        if (studentNum == testNum)
        {
            listNumber = i;
            return true;
        }
    }
    return false;
}

//Main function, initializes the window, brings it to fullscreen, and runs the intakeStudentNumbers().
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    attendance w;
    w.showFullScreen();
    intakeStudentNumbers();

    return a.exec();
}

void attendance::on_submit_clicked()
{
    if(ui->studNum->text().isEmpty())
    {
        QMessageBox::warning(this, "No number supplied", "Please put your student number in the box labeled student number!");
    }
    else if(ui->studNum->text() == "-")
    {
        QMessageBox::warning(this, "Invalid Character", "Invalid character. Please try again.");
        ui->studNum->clear();
    }
    else if(ui->studNum->text() == "+")
    {
        QMessageBox::warning(this, "Invalid Character", "Invalid character. Please try again.");
        ui->studNum->clear();
    }
    else
    {
        ofstream log;
        log.open(logLocation, ios::app);
        QString qsInputNum = ui->studNum->text();
        string sInputNum = qsInputNum.toUtf8().constData();
        int inputNum = stoi(sInputNum);
        //Match the input number with one present in the studentList
        if(parseStudentNumbers(studentList,inputNum))
        {
            //Prevent the user from selecting both sign-in and sign-out group boxes at once.
            if(ui->signInBox->isChecked() && ui->signOutBox->isChecked())
            {
                QMessageBox::warning(this, "Both options selected", "Please select the sign in OR sign out box. Not both.");
            }
            //Sign-in logic path
            else if(ui->signInBox->isChecked())
            {
                if(ui->lateButton->isChecked())
                {
                    QString output = QString::fromStdString(studentList[listNumber].name) + " Is signing in on " + QString::fromStdString(printTimeDate()) + " because they are late.";
                    string logOutput = output.toUtf8().constData();
                    QMessageBox::information(this, "Sign-in successful", output);
                    log << logOutput;
                    ui->studNum->clear();
                    log.close();
                }
                else if(ui->lunchButtonIn->isChecked())
                {
                    QString output = QString::fromStdString(studentList[listNumber].name) + " Is signing in on " + QString::fromStdString(printTimeDate()) + " because they were out for lunch.";
                    string logOutput = output.toUtf8().constData();
                    QMessageBox::information(this, "Sign-in successful", output);
                    log << logOutput;
                    ui->studNum->clear();
                    log.close();
                }
                else if(ui->medicalButtonIn->isChecked())
                {
                    QString output = QString::fromStdString(studentList[listNumber].name) + " Is signing in on " + QString::fromStdString(printTimeDate()) + " because they had a medical/dental appointment";
                    string logOutput = output.toUtf8().constData();
                    QMessageBox::information(this, "Sign-in successful", output);
                    log << logOutput;
                    ui->studNum->clear();
                    log.close();
                }
                else if (ui->sportButtonIn->isChecked())
                {
                    QString output = QString::fromStdString(studentList[listNumber].name) + " Is signing in on " + QString::fromStdString(printTimeDate()) + " because they were out for sports.";
                    string logOutput = output.toUtf8().constData();
                    QMessageBox::information(this, "Sign-in successful", output);
                    log << logOutput;
                    ui->studNum->clear();
                    log.close();
                }
                else if (ui->familyButtonIn->isChecked())
                {
                    QString output = QString::fromStdString(studentList[listNumber].name) + " Is signing in on " + QString::fromStdString(printTimeDate()) + " because they had a family emergency.";
                    string logOutput = output.toUtf8().constData();
                    QMessageBox::information(this, "Sign-in successful", output);
                    log << logOutput;
                    ui->studNum->clear();
                    log.close();
                }
                //Check to see if they put a reason into the "other" category - if they did, all clear, log the reason. If they did not, prevent them from logging out.
                else if (ui->otherButtonIn->isChecked())
                {
                    QString output = QString::fromStdString(studentList[listNumber].name) + " Is signing in on " + QString::fromStdString(printTimeDate()) + " for an unspecified reason.";
                    string logOutput = output.toUtf8().constData();
                    QMessageBox::information(this, "Sign-in successful", output);
                    log << logOutput;
                    ui->studNum->clear();
                    log.close();

                    /*Logic for if other text box exists
                    if(ui->otherReasonIn->text().isEmpty())
                    {
                        QMessageBox::warning(this, "Provide valid reason!", "Please provide a valid reason to use the 'other' sign in/out option.");
                    }
                    else
                    {
                        QString output = QString::fromStdString(studentList[listNumber].name) + " Is signing in on " + QString::fromStdString(printTimeDate()) + " because " + ui->otherReasonIn->text();
                        string logOutput = output.toUtf8().constData();
                        QMessageBox::information(this, "Sign-in successful", output);
                        log << logOutput;
                        ui->studNum->clear();
                        log.close();
                    }
                    */
                }
            }
            //Sign-out logic path
            else if(ui->signOutBox->isChecked())
            {
                //QString output = "Thank you for signing out " + QString::fromStdString(studentList[listNumber].name);
                //QMessageBox::information(this, "Signing out", output);
                if(ui->lunchButtonOut->isChecked())
                {
                    int year = returnYear();
                    int month = compareMonth();
                    int abbrevStudNum = stoi(to_string(studentList[listNumber].returnNumber()).substr(0, 2));
                    if (abbrevStudNum == year && month == 1)
                    {
                        QString output = QString::fromStdString(studentList[listNumber].name) + " Is signing out on " + QString::fromStdString(printTimeDate()) + " because they are going out for lunch.";
                        string logOutput = output.toUtf8().constData();
                        QMessageBox::information(this, "Sign-out successful", output);
                        log << logOutput;
                        ui->studNum->clear();
                        log.close();
                    }
                    else if (abbrevStudNum == (year+1) && month == 2)
                    {
                        QString output = QString::fromStdString(studentList[listNumber].name) + " Is signing out on " + QString::fromStdString(printTimeDate()) + " because they are going out for lunch.";
                        string logOutput = output.toUtf8().constData();
                        QMessageBox::information(this, "Sign-out successful", output);
                        log << logOutput;
                        ui->studNum->clear();
                        log.close();
                    }
                    else
                    {
                        QString output = QString::fromStdString(studentList[listNumber].name) + ": ATTMEPTING TO SIGN OUT FOR UNAUTHORIZED OFF-CAMPUS LUNCH HAS BEEN LOGGED.";
                        string logOutput = output.toUtf8().constData();
                        QMessageBox::critical(this, "UNAUTHORIZED LUNCH SIGNOUT", output);
                        log << logOutput;
                        ui->studNum->clear();
                        log.close();
                    }

                }
                else if(ui->medicalButtonOut->isChecked())
                {
                    QString output = QString::fromStdString(studentList[listNumber].name) + " Is signing out on " + QString::fromStdString(printTimeDate()) + " because they have a medical/dental appointment.";
                    string logOutput = output.toUtf8().constData();
                    QMessageBox::information(this, "Sign-out successful", output);
                    log << logOutput;
                    ui->studNum->clear();
                    log.close();
                }
                else if (ui->sportButtonOut->isChecked())
                {
                    QString output = QString::fromStdString(studentList[listNumber].name) + " Is signing out on " + QString::fromStdString(printTimeDate()) + " because they have a sporting event.";
                    string logOutput = output.toUtf8().constData();
                    QMessageBox::information(this, "Sign-out successful", output);
                    log << logOutput;
                    ui->studNum->clear();
                    log.close();
                }
                else if (ui->familyButtonOut->isChecked())
                {
                    QString output = QString::fromStdString(studentList[listNumber].name) + " Is signing out on " + QString::fromStdString(printTimeDate()) + " because they have a family emergency.";
                    string logOutput = output.toUtf8().constData();
                    QMessageBox::information(this, "Sign-out successful", output);
                    log << logOutput;
                    ui->studNum->clear();
                    log.close();
                }
                //Check to see if the user gave a reason, if they didn't, inform them they have to.
                else if (ui->otherButtonOut->isChecked())
                {
                    QString output = QString::fromStdString(studentList[listNumber].name) + " Is signing out on " + QString::fromStdString(printTimeDate()) + " for an unspecified reason.";
                    string logOutput = output.toUtf8().constData();
                    QMessageBox::information(this, "Sign-out successful", output);
                    log << logOutput;
                    ui->studNum->clear();
                    log.close();


                    //Logic for if other text box exists
                    /*if(ui->otherReasonOut->text().isEmpty())
                    {
                        QMessageBox::warning(this, "Provide valid reason!", "Please provide a valid reason to use the 'other' sign in/out option.");
                    }
                    else
                    {
                        QString output = QString::fromStdString(studentList[listNumber].name) + " Is signing out on " + QString::fromStdString(printTimeDate()) + " because " + ui->otherReasonOut->text();
                        string logOutput = output.toUtf8().constData();
                        QMessageBox::information(this, "Sign-out successful", output);
                        log << logOutput;
                        ui->studNum->clear();
                        log.close();
                    }
                    */
                }
            }
            //Inform the user they have to select a logic path
            else
            {
                QMessageBox::warning(this, "No option selected!", "Please select the sign in or sign out box.");
            }

        }
        else
        {
           QMessageBox::warning(this, "Student number not found ", "Please enter your Cooper student number (lunch code).");
        }
    }

}

//Allows easy switching between signin and signout
void attendance::on_signOutBox_clicked(bool checked)
{
    if(checked)
    {
        ui->signInBox->setChecked(false);
    }

}

void attendance::on_signInBox_clicked(bool checked)
{
    if(checked)
    {
        ui->signOutBox->setChecked(false);
    }
}
