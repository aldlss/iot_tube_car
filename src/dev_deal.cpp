#include<unistd.h>
#include<termios.h>
#include<cstring>
#include<sys/types.h>
#include<fcntl.h>
#include<iostream>

#include"dev_deal.h"

int OpenDev(char *dev)
{
    int fd=open(dev,O_RDWR|O_NOCTTY|O_NDELAY);
    if(-1==fd)
    {
        perror("Can't Open Serial Port");
        return -1;
    }
    if(fcntl(fd,F_SETFL,0)<0)
    {
        perror("fcntl failed.");
        return -1;
    }
    return fd;
}

int InitDev(char *dev)
{
    int fd;
    fd=OpenDev(dev);
    if(fd<0)return -1;
    termios opt;
    tcgetattr(fd,&opt);
    cfsetspeed(&opt,B115200);
    tcsetattr(fd,TCSANOW,&opt);
    opt.c_cflag&=~CSIZE;
    opt.c_cflag|=CS8;
    opt.c_cflag&=~PARENB;
    opt.c_iflag&=~INPCK;
    opt.c_cflag&=~CSTOPB;
    opt.c_cflag|=CLOCAL;
    opt.c_cflag|=CREAD;
    opt.c_iflag|=IXON|IXOFF|IXANY;//iflag
    opt.c_oflag &= ~OPOST;
    opt.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    tcflush(fd,TCIFLUSH);
    return fd;
}

int DevWrite(int fd,char *sendBuf,int len)
{
    if(fd<0)return -1;
    tcflush(fd,TCOFLUSH);
    int writeLen;
    writeLen=write(fd,sendBuf,len);
    if(writeLen==len)
        return len;
    else
    {
        tcflush(fd,TCOFLUSH);
        return writeLen;
    }
}

int DevRead(int fd,char *recvBuf,int len)
{
    if(fd<0)return -1;
    int readLen=0;
    int fsSel;
    // char buff[10];
    char ch;
    fd_set fsRead;
    FD_ZERO(&fsRead);
    FD_SET(fd,&fsRead);
    timeval time;
    time.tv_sec=10;
    time.tv_usec=0;
    fsSel=select(fd+1,&fsRead,NULL,NULL,&time);
    if(fsSel)
    {
        // readLen+=read(fd,recvBuf,len);
        do
        {      
            // memset(buff,0,sizeof(buff));
            readLen+=read(fd,&ch,1);
            if(readLen>=len)
            {
                // readLen-=strlen(buff);
                --readLen;
                tcflush(fd,TCIFLUSH);
                return readLen;
            }
            // strcat(recvBuf,buff);
            strcat(recvBuf,&ch);
        }
        while(ch!='\n');
        // while(buff[strlen(buff)-1]!='\n'&&buff[strlen(buff)-1]!='\r');
        tcflush(fd,TCIFLUSH);
        return readLen;
   }
    return 0;
}

