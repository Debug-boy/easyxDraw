#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <easyx.h>

#define SIGHT_W		800
#define SIGHT_H			800
#define SIGHT_MAX	800
#define SIGHT_HALF	400
#define SIGHT_SCALE 10

#define PI 3.1415926f

#define PHY_MOD_YBS	0
#define PHY_MOD_DDS 1

typedef struct {
	float x;
	float y;
}POINTF;

void DrawPhyLine(const char* expression, bool drawAxis, int mod, COLORREF color);
void LineEqual(float _value, char mod, int length, COLORREF color);

void DrawQuadrant()
{
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
		ret = sscanf(expression, "%f*x*x%fx%f", &a, &b, &c);//解析一般式
		axis = -(b / 2 * a);
		break;

	case PHY_MOD_DDS:
		ret = sscanf(expression, "%f(x-(%f))^2+%f", &a, &h, &k);//解析顶点式(x-(6)+4
		axis = h;
		break;

	default:
		break;
	}

	//half = -(b/2*a);
	//是否绘制对称轴
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


//1.x=0,  2.y=0;	特殊类型直线
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

//绘制sin函数图像
void DrawSin(COLORREF color)
{
	static float y, x;
	for (x = -SIGHT_HALF; x <= SIGHT_HALF; x += 0.001f) {
		y = sinf(x);
		putpixel(x * SIGHT_SCALE, y * SIGHT_SCALE, color);
	}

}

//绘制cos函数图像
void DrawCos(COLORREF color)
{
	static float y, x;
	for (x = -SIGHT_HALF; x <= SIGHT_HALF; x += 0.001f) {
		y = cosf(x+1.5f);
		putpixel(x * SIGHT_SCALE, y * SIGHT_SCALE, color);
	}
}

//绘制tan函数图像
void DrawTan(COLORREF color)
{
	static float y, x;
	for (x = -SIGHT_HALF; x <= SIGHT_HALF; x += 0.001f) {
		y = tanf(x);
		putpixel(x * SIGHT_SCALE, y * SIGHT_SCALE, color);
	}
}

void DrawAbsLine(char* express)
{

}


int main(int argc, char* argv[]) {

	initgraph(800, 800, 0);
	InitMyDraw();
	DrawQuadrant();


	DrawPhyLine("1(x-(0))^2+5", true, 1, CYAN);
	DrawPhyLine("1*x*x+8x-18",false,0,CYAN);
	LineEqual((char*)"y=2x-6",SIGHT_MAX,GREEN);
	//CicleEqual((char*)"(x-2)+(y-4)=16", RED);

	//LineEqual(3, 'y', SIGHT_MAX, RED);


	//DrawSin(YELLOW);
	//DrawCos(CYAN);
	//DrawTan(RED);

	int ret = getchar();

	return EXIT_SUCCESS;
}