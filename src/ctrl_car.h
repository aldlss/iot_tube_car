#ifndef _CTRL_CAR
#define _CTRL_CAR
#include<serial/serial.h>
#include<tuple>
using namespace std;
const double PI=3.14;
enum {
    CAR_LEFT,CAR_RIGHT
};

class Car
{
    
private:
    serial::Serial* fd;
public:
    Car(serial::Serial* fdin);
    Car();
    int setCarFd(serial::Serial* fdin);
    int carTurn(double rad,int direction);
    int carForward(double speed);
    tuple<int,int,int,int>getDistance();
};
#endif //_CTRL_CAR