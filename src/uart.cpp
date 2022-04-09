#include<iostream>
#include<cstring>
#include<thread>

#include<unistd.h>
#include<serial/serial.h>
#include"ctrl_car.h"
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
    for(int i=0;i<=1;++i)if(!before[i])before[i]=0x3F3F3F3F;
    while(1)
    {
        if(!running)return 0;
        fd.carForward(10);
        usleep(1000);
        tie<int,int,int,int>(data[0],data[2],data[1],data[3])=fd.getDistance();
        if(data[0]+data[2]+data[1]+data[3]!=0)
        {
            // if(data[lean]<20)fd.carTurn(0.1,lean^1);//0.175
            // else if(data[lean]>40)fd.carTurn(0.1,lean);
            for(int i=0;i<=1;++i)if(!before[i])data[i]=0x3F3F3F3F;
            if(data[lean]-before[lean]>data[lean^1]-before[lean^1])
                fd.carTurn(0.1,lean);
            else if(data[lean]-before[lean]<data[lean^1]-before[lean^1])
                fd.carTurn(0.1,lean^1);
            before=data;
        }
        usleep(1000);
    }
}
int main(int argc,char **argv)
{
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
    while(1)
    {
        getline(cin,inData);
        if(inData[0]=='s')
            car1.carForward(-10);
        else if(inData[0]=='w')
            car1.carForward(10);
        else if(inData[0]=='y'&&!running)
        {
            thread th2(controlCar,ref(car1),CAR_LEFT);
            th2.detach();
            running=true;
        }
        else if(inData[0]=='n')running=false;
        else if(inData[0]=='p')
        {
            int a,b,c,d;
            tie<int,int,int,int>(a,b,c,d)=car1.getDistance();
            printf("%d %d %d %d\n",a,b,c,d);
        }
        else if(inData[0]=='u')break;
        else
        {
            inData+="\r\n";
            fd.write(inData);
        }
        // inData+="\r\n";
        // fd.write(inData);
        // fd.flushOutput();
        inData.clear();
    }
    fd.close();
    return 0;
}