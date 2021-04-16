#include "main.h"
int64_t h=0;
int64_t h2=0;
void Base_Caculation(double left,double right,double c)
{
    h=(2.0*Triangle_Square(left,right,c))/c;
  
    left_side=Other_Angle_Side(h,left);
    right_side=Other_Angle_Side(h,right);
}

int64_t Max_64t(int64_t x, int64_t y)
{
    if(ABS(x)>=ABS(y))
        return x;
    else 
        return y;
}
int64_t Min_64t(int64_t x, int64_t y)
{
    if(ABS(y)<ABS(x))
        return y;
    else 
        return x;
}

double Triangle_Square(double a,double b,double c)
{
    double p;
    double S;
    double S1;
    double S2;
    a=a/10;
    b=b/10;
    c=c/10;
    p=(a+b+c)/2;
    //h=p;
    S1=sqrt(p*(p-a));
    S2=sqrt((p-b)*(p-c));
    S=S1*S2;
    S=S*100;
    return S;
}
double RT_Angle_Side(double a,double b)//third side for RT triangle
{
    double c;
    c=sqrt(a*a+b*b);
    return c;
}
double Other_Angle_Side(double a,double c)
{
    double b;
    b=sqrt(c*c-a*a);
    return b;
}
double Eculid_Distance(double x1,double y1,double x2,double y2)
{
    double result;
    result=sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
    return result;
}
