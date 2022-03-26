#include<iostream>
#include<cstring>
#include<thread>

#include<serial/serial.h>

using namespace std;
using namespace serial;
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
    thread th1(readData,&fd);
    th1.detach();
    while(1)
    {
        getline(cin,inData);
        inData+="\r\n";
        fd.write(inData);
        // fd.flushOutput();
        inData.clear();
    }
    return 0;
}