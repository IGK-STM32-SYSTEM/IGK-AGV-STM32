#include "calculate.h"
#include "math.h"
#define pi 3.1415926535898
// 已知常量,单位mm
float OA = 100;
float AB = 135;
float BC = 160;
float Ax = 0;
float Ay = 0;
float Az = 100;
//角度转弧度
float deg2rad(float deg)
{
	return pi/180.0*deg;
}
//弧度转角度
float rad2deg(float rad)
{
	return 180/pi*rad;
}
//单位弧度
#define theta  deg2rad(90)

//逆向解算函数，已知坐标求角度
void CalculateReverse(Point point)
{
	Angle angle = {0,0,0};
//	//当z小于OA时，根据公式计算的结果会出现和对应位置的角度相同，因此这里做镜像处理
//	if(point.z < OA){
//		Angle angle1 = calculateReverse(point);
//		point.z = OA;
//		angle = calculateReverse(point);
//		angle.beta = (angle.beta-angle1.beta)*2+angle1.beta;
//		angle.alpha = angle1.alpha;
//		angle.gamma = angle1.gamma;
//	}
//	else
//		angle = calculateReverse(point);
	//计算三轴速度比例
//	Axis1Speed = 1;
//	Axis2Speed = fabs((angle.beta-AxisNowAngle1)/(angle.alpha+90 - AxisNowAngle2));
//	Axis3Speed = fabs((angle.beta-AxisNowAngle1)/(90-(angle.gamma-angle.beta) - AxisNowAngle3));
	
	angle = calculateReverse(point);
	
	RobotAllToAngle(angle.beta,angle.alpha+90,90-(angle.gamma-angle.beta));
//	RobotToAngle(1,angle.beta);
//	RobotToAngle(2,angle.alpha+90);
//	RobotToAngle(3,90-(angle.gamma-angle.beta));
}

Angle calculateReverse(Point point)
{
	  Angle angle;
	  //1.求解α
    angle.alpha = atan(point.y / point.x );
    float AC = pow((point.x - Ax),2) + pow((point.y - Ay),2) + pow((point.z - Az),2);
    //2.求解γ
    angle.gamma = acos((pow(BC,2) + pow(AB,2) - AC) / (2 * BC * AB));
    //3.求解β
    float A = AB - BC * cos(angle.gamma);
    float B = BC * sin(angle.gamma);
    float C = sqrt(pow(point.x,2) + pow(point.y,2)) - OA * cos(theta);
		//备注:由于d1直接算出的结果是一个精度非常高的的数字，可能会超出asin允许的范围，因此采用
		//分步计算，强制转为float型，去除后面的尾数（这个是由于计算机误差造成）
    //beta = asin(C / sqrt(pow(A,2) + pow(B,2))) - atan(B / A);
	  float d1 = C / sqrt(pow(A,2) + pow(B,2));
	  //这里由于计算机误差，d1可能大于1
		if(d1>1)d1=1;
	
		float d2 = atan(B / A);
		float d3 = asin(d1);
	  angle.beta = d3 - d2;
		
		angle.beta = rad2deg(angle.beta);
		angle.alpha = rad2deg(angle.alpha);
		angle.gamma = rad2deg(angle.gamma);
		return angle;
}


//正向解算函数，已知角度求坐标
//angle:角度值，函数内部会自动转为弧度
Point CalculatePositive(Angle angle)
{
	  Point point;
		//将角度转为弧度
		angle.alpha = deg2rad(angle.alpha);
		angle.beta = deg2rad(angle.beta);
		angle.gamma = deg2rad(angle.gamma);
	
    //1.求解x,开根号有两个结果，需要分别求解
    float temp1 = sqrt(1 + pow(tan(angle.alpha),2));
    point.x = (BC * sin(angle.gamma - angle.beta) + AB * sin(angle.beta) + OA * cos(theta)) / temp1;
    //2.求解y
    point.y = point.x * tan(angle.alpha);
    // 3.求解Cz
    // 这里AC实际为AC的平方，不好表示，所以直接写成了AC
    float AC = pow(BC,2) + pow(AB,2) - 2 * AB * BC * cos(angle.gamma);
    point.z = sqrt(AC - pow((point.x - Ax),2) - pow((point.y - Ay),2)) + Az;
		return point;
}

//获取实际角度
//输入:算法角度
//输出:实际角度
//angle.beta 对应轴1
//angle.alpha 对应轴2
//angle.gamma 对应轴3
Angle GetRealAngle(Angle angle)
{  
	Angle angles;
	//轴1
	angles.beta = angle.beta;
	//轴2
	angles.alpha = angle.alpha+90;
	//轴3
	angles.gamma = 90-(angle.gamma-angle.beta);
	return angles;
}
//获取算法角度
//输入:实际角度
//输出:算法角度
Angle GetCalculateAngle(Angle angle)
{
	Angle angles;
	//轴1
	angles.beta = angle.beta;
	//轴2
	angles.alpha = angle.alpha-90;
	//轴3
	angles.gamma = 90-(angle.gamma-angle.beta);
	return angles;
}




