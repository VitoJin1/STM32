#ifndef _BASE_CALCULATION_H_
#define _BASE_CALCULATION_H_
#include "stdint.h"
double Triangle_Square(double a,double b,double c);
double RT_Angle_Side(double a,double b);
double Other_Angle_Side(double a,double c);
void Base_Caculation(double left,double right,double c);
double Eculid_Distance(double x1,double y1,double x2,double y2);
extern int64_t h;
extern int64_t h2;
int64_t Max_64t(int64_t x, int64_t y);
int64_t Min_64t(int64_t x, int64_t y);
#endif
