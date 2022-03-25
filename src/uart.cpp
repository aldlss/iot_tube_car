#include<iostream>
#include<cstring>
#include<thread>

#include"dev_deal.h"

using namespace std;

void readData(int fd)
{
    char receBuf[200];
    while(1)
    {
        if(DevRead(fd,receBuf,sizeof(receBuf))>0)
        {
            // printf("%d\n",strlen(receBuf));
            printf("Recieve data : %s",receBuf);
            memset(receBuf,0,sizeof(receBuf));
            // usleep(500);
        }
    }
}

int main(int argc,char **argv)
{
    int fd,nread;
    char inData[200];
    char dev[30];
    if(argc==1)strcpy(dev,"/dev/ttyACM0");
    else if(argc==2)
    {
        strcpy(dev,"/dev/");
        strcat(dev,argv[1]);
    }
    else
    {
        puts("input too much!");
        return -1;
    }
    // printf("%s",dev);
    fd=InitDev(dev);
    if(fd<0)return -1;
    // thread th1(readData,fd);
    // th1.detach();
    while(1)
    {
        cin.getline(inData,200);
        // getchar();
        strcat(inData,"\r\n");
        // printf("%s",inData);
        DevWrite(fd,inData,strlen(inData));
        memset(inData,0,sizeof(inData));
    }
    return 0;
}