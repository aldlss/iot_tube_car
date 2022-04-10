#include<iostream>
#include<cstring>
#include<thread>

#include<unistd.h>
#include<serial/serial.h>
#include"ctrl_car.h"
#include"comopt.h"
using namespace std;
using namespace serial;
bool running;
void readData(Serial *fd)
{
    string receBuf;
    while(1)
    {
        if(fd->waitReadable())
        {
            receBuf=fd->readline();
            printf("Recieve data : %s",receBuf.c_str());
            fd->flushInput();
            receBuf.clear();
        }
    }
}

int controlCar(Car& fd,int lean)
{
    array<int,4>data{},before{};//0=aheadLeft,1=aheadRight,2=ahead,4=behind
    tie<int,int,int,int>(before[0],before[2],before[1],before[3])=fd.getDistance();
    printf("l:%d r:%d\n",before[0],before[1]);
    for(int i=0;i<=1;++i)if(!before[i])before[i]=200;
    while(1)
    {
        if(!running)return 0;
        fd.carForward(10);
        usleep(500000);
        tie<int,int,int,int>(data[0],data[2],data[1],data[3])=fd.getDistance();
        printf("l:%d r:%d\n",data[0],data[1]);
        if(data[0]+data[2]+data[1]+data[3]!=0)
        {
            // if(data[lean]<20)fd.carTurn(0.1,lean^1);//0.175
            // else if(data[lean]>40)fd.carTurn(0.1,lean);
            for(int i=0;i<=1;++i)if(!data[i])data[i]=200;
            if(data[lean]-before[lean]>data[lean^1]-before[lean^1])
            {
                fd.carTurn(0.175,lean);
                printf("Turn %s\n",lean?"right":"left");
            }
            else if(data[lean]-before[lean]<data[lean^1]-before[lean^1])
            {
                fd.carTurn(0.175,lean^1);
                printf("Turn %s\n",lean?"left":"right");
            }
            before=data;
        }
        usleep(500000);
    }
}
int main(int argc,char **argv)
{
    // string w{"za500 161 888"};
    // auto a=divideStrByMark(w,'a');
    // for(auto str:a)printf("%s\n",str.c_str());
    // auto b=readNumFromStr(a[1]);
    // for(int num:b)printf("%d\n",num);
    string dev,inData;
    if(argc==1)dev="/dev/ttyACM0";
    else if(argc==2)
    {
        dev+="/dev/";
        dev.append(argv[1]);
    }
    else
    {
        puts("input too much!");
        return -1;
    }
    Serial fd(dev,115200U,Timeout(0,10000U,0,0,0));
    if(!fd.isOpen())
    {
        puts("Open failed.");
        return -1;
    }
    fd.flush();
    // thread th1(readData,&fd);
    // th1.detach();
    Car car1(&fd);
    // thread th2(controlCar,ref(car1),CAR_LEFT);
    bool always=true;
    while(always)
    {
        getline(cin,inData);
        switch(inData[0])
        {
            case 's':
            {
                car1.carForward(-10);
                break;
            }
            case 'w':
            {
                car1.carForward(10);
                break;
            }
            case 'y':
            {
                if(running)break;
                if(inData[2]=='l')
                {
                    thread th2(controlCar,ref(car1),CAR_LEFT);
                    th2.detach();
                }
                else if(inData[2]=='r')
                {
                    thread th2(controlCar,ref(car1),CAR_RIGHT);
                    th2.detach();
                }
                else
                {
                    puts("need direction");
                    break;
                }
                running=true;
                break;
            }
            case 'n':
            {
                running=false;
                break;
            }
            case 'p':
            {
                int a,b,c,d;
                tie<int,int,int,int>(a,b,c,d)=car1.getDistance();
                printf("%d %d %d %d\n",a,b,c,d);
                break;
            }
            case 'u':
            {
                always=false;
                break;
            }
            case 'l':
            {
                auto a=divideStrByMark(inData);
                car1.carTurn(atof(a[1].c_str()),CAR_LEFT);
                break;
            }
            case 'r':
            {
                auto a=divideStrByMark(inData);
                car1.carTurn(atof(a[1].c_str()),CAR_RIGHT);
                break;
            }
            default:
            {
                inData+="\r\n";
                fd.write(inData);
                break;
            }
        }
        // inData+="\r\n";
        // fd.write(inData);
        // fd.flushOutput();
        inData.clear();
    }
    fd.close();
    return 0;
}