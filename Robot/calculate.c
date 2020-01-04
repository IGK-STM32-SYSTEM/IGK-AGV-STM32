#include "calculate.h"
#include "math.h"
#define pi 3.1415926535898
// ��֪����,��λmm
float OA = 100;
float AB = 135;
float BC = 160;
float Ax = 0;
float Ay = 0;
float Az = 100;
//�Ƕ�ת����
float deg2rad(float deg)
{
	return pi/180.0*deg;
}
//����ת�Ƕ�
float rad2deg(float rad)
{
	return 180/pi*rad;
}
//��λ����
#define theta  deg2rad(90)

//������㺯������֪������Ƕ�
void CalculateReverse(Point point)
{
	Angle angle = {0,0,0};
//	//��zС��OAʱ�����ݹ�ʽ����Ľ������ֺͶ�Ӧλ�õĽǶ���ͬ�����������������
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
	//���������ٶȱ���
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
	  //1.����
    angle.alpha = atan(point.y / point.x );
    float AC = pow((point.x - Ax),2) + pow((point.y - Ay),2) + pow((point.z - Az),2);
    //2.����
    angle.gamma = acos((pow(BC,2) + pow(AB,2) - AC) / (2 * BC * AB));
    //3.����
    float A = AB - BC * cos(angle.gamma);
    float B = BC * sin(angle.gamma);
    float C = sqrt(pow(point.x,2) + pow(point.y,2)) - OA * cos(theta);
		//��ע:����d1ֱ������Ľ����һ�����ȷǳ��ߵĵ����֣����ܻᳬ��asin����ķ�Χ����˲���
		//�ֲ����㣬ǿ��תΪfloat�ͣ�ȥ�������β������������ڼ���������ɣ�
    //beta = asin(C / sqrt(pow(A,2) + pow(B,2))) - atan(B / A);
	  float d1 = C / sqrt(pow(A,2) + pow(B,2));
	  //�������ڼ������d1���ܴ���1
		if(d1>1)d1=1;
	
		float d2 = atan(B / A);
		float d3 = asin(d1);
	  angle.beta = d3 - d2;
		
		angle.beta = rad2deg(angle.beta);
		angle.alpha = rad2deg(angle.alpha);
		angle.gamma = rad2deg(angle.gamma);
		return angle;
}


//������㺯������֪�Ƕ�������
//angle:�Ƕ�ֵ�������ڲ����Զ�תΪ����
Point CalculatePositive(Angle angle)
{
	  Point point;
		//���Ƕ�תΪ����
		angle.alpha = deg2rad(angle.alpha);
		angle.beta = deg2rad(angle.beta);
		angle.gamma = deg2rad(angle.gamma);
	
    //1.���x,�������������������Ҫ�ֱ����
    float temp1 = sqrt(1 + pow(tan(angle.alpha),2));
    point.x = (BC * sin(angle.gamma - angle.beta) + AB * sin(angle.beta) + OA * cos(theta)) / temp1;
    //2.���y
    point.y = point.x * tan(angle.alpha);
    // 3.���Cz
    // ����ACʵ��ΪAC��ƽ�������ñ�ʾ������ֱ��д����AC
    float AC = pow(BC,2) + pow(AB,2) - 2 * AB * BC * cos(angle.gamma);
    point.z = sqrt(AC - pow((point.x - Ax),2) - pow((point.y - Ay),2)) + Az;
		return point;
}

//��ȡʵ�ʽǶ�
//����:�㷨�Ƕ�
//���:ʵ�ʽǶ�
//angle.beta ��Ӧ��1
//angle.alpha ��Ӧ��2
//angle.gamma ��Ӧ��3
Angle GetRealAngle(Angle angle)
{  
	Angle angles;
	//��1
	angles.beta = angle.beta;
	//��2
	angles.alpha = angle.alpha+90;
	//��3
	angles.gamma = 90-(angle.gamma-angle.beta);
	return angles;
}
//��ȡ�㷨�Ƕ�
//����:ʵ�ʽǶ�
//���:�㷨�Ƕ�
Angle GetCalculateAngle(Angle angle)
{
	Angle angles;
	//��1
	angles.beta = angle.beta;
	//��2
	angles.alpha = angle.alpha-90;
	//��3
	angles.gamma = 90-(angle.gamma-angle.beta);
	return angles;
}




