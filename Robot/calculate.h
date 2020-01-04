#ifndef  _CALCULATE_H_
#define  _CALCULATE_H_
#include "steppercontrol.h"

//逆向解算
void CalculateReverse(Point point);
Angle calculateReverse(Point point);

//正向解算
//angle:角度值，函数内部会自动转为弧度
Point CalculatePositive(Angle angle);

//获取实际角度
//输入:算法角度
//输出:实际角度
//angle.beta 对应轴1
//angle.alpha 对应轴2
//angle.gamma 对应轴3
Angle GetRealAngle(Angle angle);
//获取算法角度
//输入:实际角度
//输出:算法角度
Angle GetCalculateAngle(Angle angle);


#endif


