/*******************************************************************************************
 * Created by Ian Steneker using free QtCreator and Designer, as well as Visual Studio 2017
 * Compiled in MinGW32
 * For use at the John Cooper School
 * Summer 2017
********************************************************************************************/

#ifndef ATTENDANCE_H
#define ATTENDANCE_H

#include <QMainWindow>

namespace Ui {
class attendance;
}

class attendance : public QMainWindow
{
    Q_OBJECT

public:
    explicit attendance(QWidget *parent = 0);
    ~attendance();

private slots:
    void on_submit_clicked();
    void on_signOutBox_clicked(bool checked);
    void on_signInBox_clicked(bool checked);


private:
    Ui::attendance *ui;
};

#endif // ATTENDANCE_H
