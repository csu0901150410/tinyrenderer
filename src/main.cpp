#include "tgaimage.h"

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);

// v1 : 按照x轴从x0到x1逐像素计算对应的y像素值
// 问题1 : 当x轴变化范围小时，y轴出现跳点
// 问题2 : 当 x0>x1 时，无法正确处理
// void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color)
// {
// 	for (int x = x0; x <= x1; x++)
// 	{
// 		float t = (x - x0) / (float)(x1 - x0);
// 		int y = y0 + (y1 - y0) * t;
// 		image.set(x, y, color);
// 	}
// }

// v2 : 同时确保按坐标变化范围大的进行逐像素遍历，同时保证大小关系
// void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color)
// {
// 	bool exchange = false;
// 	if (std::abs(x0 - x1) < std::abs(y0 - y1))
// 	{
// 		// y方向坐标变化大，转置，变成x方向坐标变化大
// 		std::swap(x0, y0);
// 		std::swap(x1, y1);
// 		exchange = true;
// 	}

// 	if (x0 > x1)
// 	{
// 		std::swap(x0, x1);
// 		std::swap(y0, y1);
// 	}

// 	// 以坐标变化大的轴进行细分
// 	for (int x = x0; x <= x1; x++)
// 	{
// 		float t = (x - x0) / (float)(x1 - x0);
// 		int y = y0 + (y1 - y0) * t;
// 		if (exchange)
// 			image.set(y, x, color);
// 		else
// 			image.set(x, y, color);
// 	}
// }

// v3 : 计算y值时使用累加代替乘法
// void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color)
// {
// 	// 转置，转置之后x为长轴，斜率不会超过1
// 	bool steep = false;
// 	if (std::abs(x0 - x1) < std::abs(y0 - y1))
// 	{
// 		std::swap(x0, y0);
// 		std::swap(x1, y1);
// 		steep = true;
// 	}

// 	if (x0 > x1)
// 	{
// 		std::swap(x0, x1);
// 		std::swap(y0, y1);
// 	}

// 	int dx = x1 - x0;
// 	int dy = y1 - y0;
// 	float derror = std::abs(dy / float(dx));// 斜率，x方向变化一个像素，y方向的变化值
// 	float error = 0;
// 	int y = y0;

// 	// 沿着长轴x步进，y方向累加斜率，超过0.5时，y沿着变化方向变化1
// 	for (int x = x0; x <= x1; x++)
// 	{
// 		if (steep)
// 		{
// 			image.set(y, x, color);
// 		}
// 		else
// 		{
// 			image.set(x, y, color);
// 		}

// 		error += derror;// 累加，代替乘法计算y
// 		if (error > 0.5f)
// 		{
// 			y += (y1 > y0 ? 1 : -1);
// 			error -= 1.0f;
// 		}
// 	}
// }

// v4 : 去除浮点运算
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

int main(int argc, char **argv)
{
	TGAImage image(100, 100, TGAImage::RGB);
	for (int i = 0; i < 1000000; i++)
	{
		line(13, 20, 80, 40, image, white);
		line(20, 13, 40, 80, image, red);
		line(80, 40, 13, 20, image, red);
	}
	image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
	image.write_tga_file("output.tga");
	return 0;
}