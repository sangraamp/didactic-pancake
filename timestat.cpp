#include<iostream>
#include<string>
#include<fstream>
#include<cmath>

using namespace std;

double toDouble(string s, int start, int stop) {   //converting part of a string to double (to identify minutes and seconds)
    unsigned long int m = 1;
    double time = 0;
    for (int i = stop; i >= start; i--) {
        time += (s[i] - '0') * m;
        m *= 10;
    }
    return time;
}



int main(){


int count=0;

ifstream infile;
infile.open("E:\\timestat.txt");            //location of txt file on my PC. can be modified for your PC.

if(infile.fail()) cout<<"Your file didn't work.";

else{ //main body starts

double realminsum=0,realsecsum=0,sysminsum=0,syssecsum=0,userminsum=0,usersecsum=0;

string str;

while(getline(infile, str))
{

if(str[0]=='r')
{
double minutes=toDouble(str,8,8);
realminsum += minutes;
double seconds= (toDouble(str,12,14)*0.001)+toDouble(str,10,10);
realsecsum += seconds;
}

if(str[0]=='u')
{
double minutes=toDouble(str,8,8);
userminsum += minutes;
double seconds= (toDouble(str,12,14)*0.001)+toDouble(str,10,10);
usersecsum += seconds;
}

if(str[0]=='s')
{
double minutes=toDouble(str,4,4);
sysminsum += minutes;
double seconds=(toDouble(str,8,10)*0.001)+toDouble(str,6,6);
syssecsum += seconds;

count++;
}

}

//now calculating the average of the sum for real, user and sys respectively (creating another function for this seemed messier
//so I performed the calculations here itself)

double realavg = ( (realminsum*60) + realsecsum ) / count;  //converting to seconds first, then to minutes and seconds
int temp1 = realavg;
int realminavg = temp1/60;
double realsecavg = realavg - (realminavg*60);

double useravg = ( (userminsum*60) + usersecsum ) / count;
int temp2 = useravg;
int userminavg = temp2/60;
double usersecavg = useravg - (userminavg*60);

double sysavg = ( (sysminsum*60) + syssecsum ) / count;
int temp3 = sysavg;
int sysminavg = temp3/60;
double syssecavg = sysavg - (sysminavg*60);

infile.close();  //just to be safe

//now to calculate standard deviation (for seconds first,then converting to minutes and seconds)
//I could have taken an arbitrarily long array to store the individual values of minutes and seconds
//in the above loop itself but I'm aiming for efficiency
//even if it means another 20-30 lines of code

infile.open("E:\\timestat.txt");             //again, location will differ on your PC, please use accordingly
if(infile.fail()) cout<<"Your file didn't work.";

string s;
double realsdsum=0,syssdsum=0,usersdsum=0; //to store sum of square terms

while(getline(infile,s)){

if(s[0]=='r')
{
double minutes=toDouble(s,8,8);
double seconds= (toDouble(s,12,14)*0.001)+toDouble(s,10,10);

realsdsum += ( ( (minutes*60) + seconds ) - realavg ) * ( ( (minutes*60) + seconds ) - realavg ); //calculating the sum of square terms
}

if(s[0]=='u')
{
double minutes=toDouble(s,8,8);
double seconds= (toDouble(s,12,14)*0.001)+toDouble(s,10,10);

usersdsum += ( ( (minutes*60) + seconds ) - useravg ) * ( ( (minutes*60) + seconds ) - useravg );//sum of square terms
}


if(s[0]=='s')
{
double minutes=toDouble(s,4,4);
double seconds=(toDouble(s,8,10)*0.001)+toDouble(s,6,6);

syssdsum += ( ( (minutes*60) + seconds ) - sysavg ) * ( ( (minutes*60) + seconds ) - sysavg );//sum of square terms
}

}

double realsdavg = realsdsum / count;    //standard deviation for real, user, sys (in seconds first, then
double realsd = pow(realsdavg,0.5);      //converting to minutes and seconds)
int temp4 = realsd;
int realsdmin = temp4/60;
double realsdsec = realsd - (realsdmin*60);

double usersdavg = usersdsum / count;
double usersd = pow(usersdavg,0.5);
int temp5 = usersd;
int usersdmin = temp5/60;
double usersdsec = usersd - (usersdmin*60);

double syssdavg = syssdsum / count;
double syssd = pow(syssdavg,0.5);
int temp6 = syssd;
int syssdmin = temp6/60;
double syssdsec = syssd - (syssdmin*60);

infile.close();

//now all that's left is calculating number of runs
//within range avg-sd to avg+sd

infile.open("E:\\timestat.txt");  //last time, pakka (please modify the location according to your PC) (crude, I know :P)
if(infile.fail()) cout<<"Your file didn't work.";


string st;
int realcount=0,usercount=0,syscount=0;

while(getline(infile, st))
{

if(st[0]=='r')
{
double minutes=toDouble(st,8,8);
double seconds= (toDouble(st,12,14)*0.001)+toDouble(st,10,10);

if( ((minutes*60)+seconds) > realavg-realsd && ((minutes*60)+seconds) < realavg+realsd ) realcount++;
}

if(st[0]=='u')
{
double minutes=toDouble(st,8,8);
double seconds= (toDouble(st,12,14)*0.001)+toDouble(st,10,10);

if( ((minutes*60)+seconds) > useravg-usersd && ((minutes*60)+seconds) < useravg+usersd ) usercount++;
}

if(st[0]=='s')
{
double minutes=toDouble(st,4,4);
double seconds=(toDouble(st,8,10)*0.001)+toDouble(st,6,6);

if( ((minutes*60)+seconds) > sysavg-syssd && ((minutes*60)+seconds) < sysavg+syssd ) syscount++;
}

}



//displaying on screen

cout<<"Number of runs : "<<count<<endl<<endl;

cout<<"Average time statistics : "<<endl;
cout<<"Real : "<<realminavg<<" minutes "<<realsecavg<<" seconds"<<"   ";
cout<<"User : "<<userminavg<<" minutes "<<usersecavg<<" seconds"<<"   ";
cout<<"System : "<<sysminavg<<" minutes "<<syssecavg<<" seconds"<<"   "<<endl<<endl;

cout<<"Standard deviation of time statistics : "<<endl;
cout<<"Real : "<<realsdmin<<" minutes "<<realsdsec<<" seconds"<<"   ";
cout<<"User : "<<usersdmin<<" minutes "<<usersdsec<<" seconds"<<"   ";
cout<<"System : "<<syssdmin<<" minutes "<<syssdsec<<" seconds"<<"   "<<endl<<endl;

cout<<"Number of runs within average - standard deviation to average + standard deviation : "<<endl;
cout<<"Real : "<<realcount<<"   ";
cout<<"User : "<<usercount<<"   ";
cout<<"System : "<<syscount<<"   "<<endl;

}

return 0;
}
