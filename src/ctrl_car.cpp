#include"ctrl_car.h"
#include<iostream>
#include<cstring>
using namespace std;

Car::Car(serial::Serial* fdin):fd(fdin){};

Car::Car(){};

int Car::setCarFd(serial::Serial* fdin)
{
    if(fdin->isOpen())fd=fdin;
    else return -1;
    return 0;
}

int Car::carTurn(double rad,int direction)
{
    if(!rad)return 0;
    rad*=20;
    if(abs(rad)>32)return -1;
    char command[20];
    if(direction==CAR_LEFT)sprintf(command,"z 0 %.2lf\r\n",rad);
    if(direction==CAR_RIGHT)sprintf(command,"z %.2lf 0\r\n",rad);
    fd->write((uint8_t*)command,strlen(command));
    return 0;
}

int Car::carForward(double speed)
{
    if(!speed)return 0;
    if(abs(speed)>25)return -1;
    char command[20];
    sprintf(command,"z %.2lf %.2lf\r\n",speed,speed);
    fd->write((uint8_t*)command,strlen(command));
    return 0;
}

tuple<int,int,int,int> Car::getDistance()
{
    fd->write("p\r\n");
    serial::Timeout tem=fd->getTimeout();
    string massage{};
    int val[]{0,0,0,0};
    int temVal=0,num=0;
    fd->setTimeout(0,800,0,0,0);
    if(fd->waitReadable())
    {
        fd->readline(massage);
        // printf("s:%s",massage.c_str());
        for(char ch:massage)
        {
            if(ch>='0'&&ch<='9')
            {
                temVal*=10;
                temVal+=ch-'0';
            }
            else
            {
                val[num]=temVal;
                temVal=0;
                ++num;
                if(num==4)break;//if no this will cross memory
            }
        }
    }
    fd->flush();
    fd->setTimeout(tem);//lvalue?
    return make_tuple(val[0],val[1],val[2],val[3]);
}