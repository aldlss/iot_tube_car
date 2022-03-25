#ifndef dev_deal
#define dev_deal

int OpenDev(char *dev);

/*Return fd if init succeed.*/
int InitDev(char *dev);

int DevWrite(int fd,char *sendBuf,int len);

int DevRead(int fd,char *recvBuf,int len);

#endif //dev_deal