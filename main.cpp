#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <easyx.h>

#define SIGHT_W 800
#define SIGHT_H 800

void DrawQuadrant()
{
	int x, y;
	x = SIGHT_W / 2;
	y = SIGHT_H / 2;

	line(-x, 0, x, 0);
	line(0, y, 0, -y);

	for (int i = -x; i <= x; i += 10)
		line(i, 0, i, 4);
	for (int j = -y; j <= y; j += 10)
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

//y=kx+b
void LineEqual(const char* equal,float length,COLORREF color)
{
	float y, k, b;
	float x_intercept, y_intercept;
	POINT p1, p2;
	sscanf(equal, "y=%fx%f",&k, &b);

	//printf("%s\n", equal);
	//printf("%f %f",k, b);
	//exit(1);

	x_intercept = -(b / k);
	y_intercept = b;

	p1.x = x_intercept * 10;
	p1.y = 0;

	p2.x = 0;
	p2.y = y_intercept * 10;

	setlinecolor(color);

	for (float x = -(length / 2); x <= (length/2); x+=0.1f) {
		y = k * x + b;
		putpixel(x*10, y*10,color);
	}
}



int main(int argc, char* argv[]) {


	initgraph(800, 800, 0);
	InitMyDraw();
	DrawQuadrant();

	DrawPhyLine();

	LineEqual((char*)"y=2x-4",200,CYAN);
	



	getchar();
	return 0;
}