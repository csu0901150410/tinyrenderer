#include <vector>
#include <cmath>
#include "tgaimage.h"
#include "model.h"
#include "geometry.h"

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);
const TGAColor green = TGAColor(0, 255, 0, 255);
const TGAColor blue = TGAColor(0, 0, 255, 255);
Model *model = NULL;
const int width = 200;
const int height = 200;

void line(int x0, int y0, int x1, int y1, TGAImage &image, const TGAColor &color)
{
	// 转置，转置之后x为长轴，斜率不会超过1
	bool steep = false;
	if (std::abs(x0 - x1) < std::abs(y0 - y1))
	{
		std::swap(x0, y0);
		std::swap(x1, y1);
		steep = true;
	}

	if (x0 > x1)
	{
		std::swap(x0, x1);
		std::swap(y0, y1);
	}

	int dx = x1 - x0;
	int dy = y1 - y0;
	int derror = std::abs(dy) << 2;
	int error = 0;
	int y = y0;

	// 沿着长轴x步进，y方向累加斜率，超过0.5时，y沿着变化方向变化1
	for (int x = x0; x <= x1; x++)
	{
		if (steep)
		{
			image.set(y, x, color);
		}
		else
		{
			image.set(x, y, color);
		}

		error += derror; // 累加，代替乘法计算y
		if (error > dx)
		{
			y += (y1 > y0 ? 1 : -1);
			error -= dx << 2;
		}
	}
}

// 扫描线的方式填充三角形
void triangle(Vec2i p0, Vec2i p1, Vec2i p2, TGAImage &image, const TGAColor &color)
{
	if (p0.y > p1.y)
		std::swap(p0, p1);
	if (p0.y > p2.y)
		std::swap(p0, p2);
	if (p1.y > p2.y)
		std::swap(p1, p2);

	line(p0.x, p0.y, p2.x, p2.y, image, red);// A
	line(p0.x, p0.y, p1.x, p1.y, image, green);// B
	line(p1.x, p1.y, p2.x, p2.y, image, white); // C

	float red_height = p2.y - p0.y;
	float green_height = p1.y - p0.y;
	float white_height = p2.y - p1.y;

	// 下部分，位于红绿边界之间
	for (int y = p0.y; y <= p1.y; y++)
	{
		float t_red = (float)(y - p0.y) / red_height;
		float t_green = (float)(y - p0.y) / green_height;
		Vec2i p_red = p0 + (p2 - p0) * t_red;
		Vec2i p_green = p0 + (p1 - p0) * t_green;
		
		line(p_red.x, y, p_green.x, y, image, blue);
	}

	// 上部分，位于红白边界之间
	for (int y = p1.y; y <= p2.y; y++)
	{
		float t_red = (float)(y - p0.y) / red_height;
		float t_white = (float)(y - p1.y) / white_height;
		Vec2i p_red = p0 + (p2 - p0) * t_red;
		Vec2i p_white = p1 + (p2 - p1) * t_white;

		line(p_red.x, y, p_white.x, y, image, blue);
	}
}

int main(int argc, char **argv)
{
	// if (2 == argc)
	// {
	// 	model = new Model(argv[1]);
	// }
	// else
	// {
	// 	model = new Model("obj/african_head.obj");
	// }

	// TGAImage image(width, height, TGAImage::RGB);
	// for (int i = 0; i < model->nfaces(); i++)
	// {
	// 	std::vector<int> face = model->face(i);
	// 	for (int j = 0; j < 3; j++)
	// 	{
	// 		Vec3f v0 = model->vert(face[j]);
	// 		Vec3f v1 = model->vert(face[(j + 1) % 3]);

	// 		int x0 = (v0.x + 1.) * width / 2.;
	// 		int y0 = (v0.y + 1.) * height / 2.;
	// 		int x1 = (v1.x + 1.) * width / 2.;
	// 		int y1 = (v1.y + 1.) * height / 2.;
	// 		line(x0, y0, x1, y1, image, white);
	// 	}
	// }

	TGAImage image(width, height, TGAImage::RGB);

	Vec2i t0[3] = {Vec2i(10, 70), Vec2i(50, 160), Vec2i(70, 80)};
	Vec2i t1[3] = {Vec2i(180, 50), Vec2i(150, 1), Vec2i(70, 180)};
	Vec2i t2[3] = {Vec2i(180, 150), Vec2i(120, 160), Vec2i(130, 180)};

	triangle(t0[0], t0[1], t0[2], image, red);
	triangle(t1[0], t1[1], t1[2], image, white);
	triangle(t2[0], t2[1], t2[2], image, green);

	image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
	image.write_tga_file("output.tga");
	delete model;
	return 0;
}