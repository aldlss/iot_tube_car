#ifndef _COMOPT
#define _COMOPT

#include<vector>
#include<string>
using namespace std;

vector<int>readNumFromStr(string str);
vector<int>readNumFromStr(char* str,size_t size);

vector<string>divideStrByMark(string str,char mark=' ');
vector<string>divideStrByMark(char* str,int len,char mark=' ');

#endif //_COMOPT