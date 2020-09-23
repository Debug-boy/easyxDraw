#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <easyx.h>

#define SIGHT_W		800
#define SIGHT_H		800
#define SIGHT_MAX	800
#define SIGHT_HALF	400
#define SIGHT_SCALE 10

typedef struct {
	float x;
	float y;
}POINTF;

void LineEqual(const char* equal, float length, COLORREF color);
void LineEqual(float _value, float mod, int length, COLORREF color);

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


//y=a*x*x+bx+c
void DrawPhyLine(char* expression) {

	static float a, b, c;
	static float x, y;
	int ret = sscanf(expression, "%f*x*x%fx%f", &a, &b, &c);

	//half = -(b/2*a);

	float half_X = -(b / 2 * a);
	LineEqual(half_X,1,SIGHT_MAX,RED);

	for (x = -SIGHT_W; x < SIGHT_W; x += 0.01f) {
		y = a *(x*x) + (b*x) + c;
		putpixel(x * SIGHT_SCALE, y * SIGHT_SCALE, CYAN);
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


//1.x=0,  2.y=0;
void LineEqual(float _value, float mod, int length, COLORREF color) {
	setlinecolor(color);
	_value *= 10;
	if (mod)
		line(_value, length / 2, _value, -(length / 2));
	else
		line(length / 2, _value, -(length / 2), _value);
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

//»æÖÆsinº¯ÊýÍ¼Ïñ
void DrawSin()
{

}


//»æÖÆcosº¯ÊýÍ¼Ïñ
void DrawCos()
{

}

//»æÖÆtanº¯ÊýÍ¼Ïñ
void DrawTan()
{

}

void DrawAbsLine(char* express)
{

}


int main(int argc, char* argv[]) {

	initgraph(800, 800, 0);
	InitMyDraw();
	DrawQuadrant();


	DrawPhyLine((char*)"1*x*x+7x-18");
	LineEqual((char*)"y=2x-6",SIGHT_MAX,GREEN);
	CicleEqual((char*)"(x-2)+(y-4)=16", RED);

	int ret = getchar();

	return EXIT_SUCCESS;
}