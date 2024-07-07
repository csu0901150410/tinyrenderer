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
void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color)
{
	bool exchange = false;
	if (std::abs(x0 - x1) < std::abs(y0 - y1))
	{
		// y方向坐标变化大，转置，变成x方向坐标变化大
		std::swap(x0, y0);
		std::swap(x1, y1);
		exchange = true;
	}

	if (x0 > x1)
	{
		std::swap(x0, x1);
		std::swap(y0, y1);
	}

	// 以坐标变化大的轴进行细分
	for (int x = x0; x <= x1; x++)
	{
		float t = (x - x0) / (float)(x1 - x0);
		int y = y0 + (y1 - y0) * t;
		if (exchange)
			image.set(y, x, color);
		else
			image.set(x, y, color);
	}
}

int main(int argc, char **argv)
{
	TGAImage image(100, 100, TGAImage::RGB);
	line(13, 20, 80, 40, image, white);
	line(20, 13, 40, 80, image, red);
	line(80, 40, 13, 20, image, red);
	image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
	image.write_tga_file("output.tga");
	return 0;
}