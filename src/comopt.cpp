#include"comopt.h"

vector<int>readNumFromStr(string str)
{
    vector<int>val{};
    int sum=0,flag=1;;
    bool read=false;
    for(char q:str)
    {
        if(q>='0'&&q<='9')
        {
            sum*=10;
            sum+=q-'0';
            read=true;
        }
        else if(q=='-'&&!read)
        {
            flag=-1;
        }
        else if(read)
        {
            val.push_back(sum*flag);
            sum=0;
            read=false;
            flag=1;
        }
    }
    if(read&&sum)val.push_back(sum*flag);
    return val;
}

vector<string>divideStrByMark(string str,char mark)
{
    vector<string>val{};
    string tem{};
    for(char q:str)
    {
        if(q!=mark)
        {
            tem+=q;
        }
        else if(!tem.empty())
        {
            val.push_back(tem);
            tem.clear();
        }
    }
    if(!tem.empty())
        val.push_back(tem);
    return val;
}