#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <easyx.h>
#include <vector>

using namespace std;

#define SIGHT_W		800
#define SIGHT_H			800
#define SIGHT_MAX	800
#define SIGHT_HALF	400
//#define SIGHT_SCALE 100
float SIGHT_SCALE = 10;

#define PI 3.1415926f

#define PHY_MOD_YBS	0
#define PHY_MOD_DDS 1

typedef struct {
	float x;
	float y;
}POINTF;

void DrawPhyLine(const char* expression, bool drawAxis, int mod, COLORREF color);
void LineEqual(float _value, char mod, int length, COLORREF color);


void setScale(double scale) {
	SIGHT_SCALE = scale;
}

void DrawQuadrant(float scale)
{
	setScale(scale);
	int x, y;
	x = SIGHT_W / 2;
	y = SIGHT_H / 2;

	line(-x, 0, x, 0);
	line(0, y, 0, -y);

	for (int i = -x; i <= x; i += SIGHT_SCALE)
		line(i, 0, i, 4);
	for (int j = -y; j <= y; j += SIGHT_SCALE)
		line(0, j, 4, j);

}

void InitMyDraw()
{
	setorigin(SIGHT_W/2, SIGHT_H/2);
	setaspectratio(1, -1);
}



//y=ax^2
void DrawPhyLine()
{
	float y, x;//y=x^2;
	for (x = -SIGHT_W / 40; x < SIGHT_W / 40; x+=0.1f) {
		y = x * x;
		putpixel(x, y, CYAN);
		putpixel(x, -y, RED);
		putpixel(y, x, 0xFF00FF);
		putpixel(-y, -x, 0x00FFFF);
	}
}

/*
y=a*x*x+bx+c
y=(x-(h))^2+k;
*/
void DrawPhyLine(const char* expression,bool drawAxis,int mod,COLORREF color) {

	static float a = 1, b, c, h, k;
	static float x, y, axis;
	static int ret;

	switch (mod)
	{
	case PHY_MOD_YBS:
		ret = sscanf(expression, "%f*x*x%fx%f", &a, &b, &c);//����һ��ʽ
		axis = -(b / 2 * a);
		break;

	case PHY_MOD_DDS:
		ret = sscanf(expression, "%f(x-(%f))^2+%f", &a, &h, &k);//��������ʽ(x-(6)+4
		axis = h;
		break;

	default:
		break;
	}

	//half = -(b/2*a);
	//�Ƿ���ƶԳ���
	if (drawAxis)
		LineEqual(axis, 'x', SIGHT_MAX, RED);
	
	for (x = -SIGHT_W; x < SIGHT_W; x += 0.01f) {

		switch (mod)
		{
		case PHY_MOD_YBS:
			y = a * (x * x) + (b * x) + c;
			break;

		case PHY_MOD_DDS:
			y = (x - h) * (x - h) + k;
			break;

		default:
			break;
		}
		putpixel(x * SIGHT_SCALE, y * SIGHT_SCALE, color);
	}
}


//y=kx+b
void LineEqual(const char* expression,float length,COLORREF color)
{
	float y, k, b;
	float x_intercept, y_intercept;
	POINT p1, p2;
	int ret  = sscanf(expression, "y=%fx%f",&k, &b);

	x_intercept = -(b / k);
	y_intercept = b;

	p1.x = x_intercept * SIGHT_SCALE;
	p1.y = 0;

	p2.x = 0;
	p2.y = y_intercept * SIGHT_SCALE;

	setlinecolor(color);

	for (float x = -(length / 2); x <= (length/2); x+=0.1f) {
		y = k * x + b;
		putpixel(x * SIGHT_SCALE, y * SIGHT_SCALE, color);
	}
}


//1.x=0,  2.y=0;	��������ֱ��
void LineEqual(float _value, char mod, int length, COLORREF color) {
	setlinecolor(color);
	_value *= 10;
	switch (mod)
	{
	case 'x':
		line(_value, length / 2, _value, -(length / 2));
		break;

	case 'y':
		line(length / 2, _value, -(length / 2), _value);
		break;

	default:
		break;
	}
}


//(x-a)^2+(y-b)^2=r*r;
void CicleEqual(char* express, COLORREF color) {

	static float a, b, r;
	static POINTF c;
	int ret = sscanf(express, "(x%f)+(y%f)=%f", &a, &b, &r);
	c = { -a * SIGHT_SCALE,-b * SIGHT_SCALE };
	r = sqrtf(r) * SIGHT_SCALE;
	circle(c.x, c.y, r);
}

//����sin����ͼ��
void DrawSin(COLORREF color)
{
	static float y, x;
	for (x = -SIGHT_HALF; x <= SIGHT_HALF; x += 0.001f) {
		y = sinf(x);
		putpixel(x * SIGHT_SCALE, y * SIGHT_SCALE, color);
	}

}

//����cos����ͼ��
void DrawCos(COLORREF color)
{
	static float y, x;
	for (x = -SIGHT_HALF; x <= SIGHT_HALF; x += 0.001f) {
		y = cosf(x+1.5f);
		putpixel(x * SIGHT_SCALE, y * SIGHT_SCALE, color);
	}
}

//����tan����ͼ��
void DrawTan(COLORREF color)
{
	static float y, x;
	for (x = -SIGHT_HALF; x <= SIGHT_HALF; x += 0.001f) {
		y = tanf(x);
		putpixel(x * SIGHT_SCALE, y * SIGHT_SCALE, color);
	}
}



#define INTERVAL_BETWEEN	0
#define INTERVAL_DOUBLE	1
#define INTERVAL_MINMIN	0xFFFFFFFF
#define INTERVAL_MAXMAX	0x7FFFFFFF

/*
0������	[1,2]		(2,3)
1������	[1,2]U[3,4]
2������	(-��,0)
3������	(0,+��)
*/
void DrawInterVal(const char* expression,int mod,float drawHeight, COLORREF LineColor, COLORREF pointColor) {

	static float left,left1, right,right1;
	static char s1, s2, s3, s4;
	static int ret;

	setlinecolor(pointColor);
	setfillcolor(pointColor);

	switch (mod)
	{
	case INTERVAL_BETWEEN:
		ret = sscanf(expression, "%c%f,%f%c", &s1, &left, &right, &s2);

		//���ƶ˵�Բ��
		if (s1 == '(')
			circle(left * SIGHT_SCALE, 0, 5);
		else
			solidcircle(left * SIGHT_SCALE, 0, 5);

		if (s2 == ')')
			circle(right * SIGHT_SCALE, 0, 5);
		else
			solidcircle(right * SIGHT_SCALE, 0, 5);

		
		line(
			left * SIGHT_SCALE, 0,
			left * SIGHT_SCALE, drawHeight * SIGHT_SCALE);//��������_left

		line(
			right * SIGHT_SCALE, 0,
			right * SIGHT_SCALE, drawHeight * SIGHT_SCALE);//��������_right

		line(
			left * SIGHT_SCALE, drawHeight * SIGHT_SCALE,
			right * SIGHT_SCALE, drawHeight * SIGHT_SCALE);//���ƺ���

		break;



		//[1,2]U[3,4]
	case INTERVAL_DOUBLE:
		ret = sscanf(expression, "%c%f,%f%cU%c%f,%f%c", 
			&s1, &left, &left1, &s2, 
			&s3, &right, &right1, &s4);
	
		if (left == INTERVAL_MINMIN)
			line(-9999, drawHeight * SIGHT_SCALE,
				left1*SIGHT_SCALE, drawHeight * SIGHT_SCALE);
		else
			line(left*SIGHT_SCALE, drawHeight * SIGHT_SCALE,
				left1*SIGHT_SCALE, drawHeight * SIGHT_SCALE);

		line(left*SIGHT_SCALE, 0, left * SIGHT_SCALE, drawHeight * SIGHT_SCALE);
		line(left1 * SIGHT_SCALE, 0, left1 * SIGHT_SCALE, drawHeight * SIGHT_SCALE);

		if (right1 == INTERVAL_MAXMAX)
			line(9999, drawHeight * SIGHT_SCALE, right * SIGHT_SCALE, drawHeight * SIGHT_SCALE);
		else
			line(right1 * SIGHT_SCALE, drawHeight * SIGHT_SCALE,
				right * SIGHT_SCALE, drawHeight * SIGHT_SCALE);

		line(right * SIGHT_SCALE, 0, right * SIGHT_SCALE, drawHeight * SIGHT_SCALE);
		line(right1 * SIGHT_SCALE, 0, right1 * SIGHT_SCALE, drawHeight * SIGHT_SCALE);

		switch (s1)
		{
		case '(':
			circle(left * SIGHT_SCALE, 0, 5); break;

		case '[':
			solidcircle(left * SIGHT_SCALE, 0, 5); break;
		
		default:
			break;
		}

		switch (s2)
		{
		case ')':
			circle(left1 * SIGHT_SCALE, 0, 5); break;

		case ']':
			solidcircle(left1 * SIGHT_SCALE, 0, 5); break;

		default:
			break;
		}

		switch (s3)
		{
			
		case '(':
			circle(right * SIGHT_SCALE, 0, 5); break;

		case '[':
			solidcircle(right * SIGHT_SCALE, 0, 5); break;

		default:
			break;
		}

		switch (s4)
		{

		case ')':
			circle(right1 * SIGHT_SCALE, 0, 5); break;

		case ']':
			solidcircle(right1 * SIGHT_SCALE, 0, 5); break;

		default:
			break;
		}

	default:
		break;
	}
}

template <typename T>
struct Vector2 {
	T x;
	T y;

	void set_xy(T _x, T _y) {
		x = _x;
		y = _y;
	}

};

template <typename T>
void Multipoint(vector<Vector2<T>>p,COLORREF color) {
	setlinecolor(color);
	for (int i = 0; i < p.size()-1; i++) {
		p[i].x,p[i].y;
		line(
			p[i].x, p[i].y,
			p[i + 1].x, p[i + 1].y
		);
	}
	
}


//	|x|=3
void DrawAbsLine(const char* express,float height,COLORREF color)
{
	static int ret;
	static float x;
	static float x1, x2;
	static char buffer[128];

	ret = sscanf(express, "|x|=%f",&x);
	x1 = x;
	x2 = -x;

	sprintf(buffer, "[%f,%f]", x1, x2);

	DrawInterVal(buffer, INTERVAL_BETWEEN, height, color, color);

}

template <typename T>
T MySqrt(T _x) {
	return (_x > (T)0) ? (T)sqrt((T)_x) : -(sqrt((T)-_x));
}


template <typename T>
T MyPow(T _x,T _y) {
	return (_x > (T)0) ? (T)pow(T(_x), T(_y)) :
		-(pow((T)-_x, (T)_y));
}


template <typename T>
void Draw(T(*fun)(T) ,DWORD dwMilliseconds, COLORREF color) {

	for (double x = -SIGHT_HALF; x <= SIGHT_HALF; x += 0.001) {
		T y = fun(x);
		putpixel(x * SIGHT_SCALE, y * SIGHT_SCALE, color);
		SleepEx(dwMilliseconds, true);//�������������
	}
}

double f(double x)
{
	//return MyPow<double>(MyPow<double>(x, 4) - 2 * MyPow<double>(x, 3), 1.0/3.0);

	//return x * MyPow<double>(x - 2.0, 1 / 3.0);
	//return MyPow<double>(3.0, 2 - x);//sqrt(-x);

	//return MyPow<double>(x, 3.0) - 6 * MyPow<double>(x, 2.0) + 11 * x - 6.0;
	//return 9*MyPow<double>(x, 3.0) - 12* MyPow<double>(x, 2.0) + 3* x;
	
	return 3 * MyPow<double>(x, 2.0) - 4 * MyPow<double>(x, 1) + 1;
}




int main(int argc, char* argv[]) {

	initgraph(SIGHT_W, SIGHT_H, 0);
	InitMyDraw();
	DrawQuadrant(100);
	

	Draw(f, 0, 0xFFFF00);



	//vector<Vector2<double>>p;
	//for (double x = -SIGHT_HALF; x <= SIGHT_HALF; x += 0.001) {
	//	Vector2<double>point;
	//	double y = f(x);
	//	point.set_xy(x * SIGHT_SCALE,y * SIGHT_SCALE);
	//	p.push_back(point);
	//}
	//Multipoint(p, CYAN);
	// 


	//DrawPhyLine("1(x-(0))^2+5", true, 1, CYAN);
	//DrawPhyLine("1*x*x-6x+5",false,0,CYAN);
	//LineEqual((char*)"y=2x-6",SIGHT_MAX,GREEN);
	//CicleEqual((char*)"(x-2)+(y-4)=16", RED);

	//LineEqual(3, 'y', SIGHT_MAX, RED);

	//DrawInterVal("[-2,2)", INTERVAL_BETWEEN, 5, CYAN, RED);
	//DrawInterVal("[-5,10]", INTERVAL_BETWEEN, 5, CYAN, RED);
	//DrawInterVal("[-5,-2]U[1,3]", INTERVAL_DOUBLE, 5, CYAN, RED);
	//DrawAbsLine("|x|=3", 10, CYAN);

	//DrawSin(YELLOW);
	//DrawCos(CYAN);
	//DrawTan(RED);

	//cout << '��' << endl;
	//printf("%c\n", '��');

	int ret = getchar();

	return EXIT_SUCCESS;
}