#include "PerlinNoise.h"

// パーリンノイズを参考にしたページ
//https://zenn.dev/baroqueengine/books/a19140f2d9fc1a/viewer/95c334

const int PERLIN_NOISE_SIZE = 256;
const int PERLIN_NOISE[PERLIN_NOISE_SIZE] = {
	151, 160, 137,  91,  90,  15, 131,  13, 201,  95,  96,  53, 194, 233,   7, 225, 140,  36, 103,  30,  69, 142,   8,
	 99,  37, 240,  21,  10,  23, 190,   6, 148, 247, 120, 234,  75,   0,  26, 197,  62,  94, 252, 219, 203, 117,  35,
	 11,  32,  57, 177,  33,  88, 237, 149,  56,  87, 174,  20, 125, 136, 171, 168,  68, 175,  74, 165,  71, 134, 139,
	 48,  27, 166,  77, 146, 158, 231,  83, 111, 229, 122,  60, 211, 133, 230, 220, 105,  92,  41,  55,  46, 245,  40,
	244, 102, 143,  54,  65,  25,  63, 161,   1, 216,  80,  73, 209,  76, 132, 187, 208,  89,  18, 169, 200, 196, 135,
	130, 116, 188, 159,  86, 164, 100, 109, 198, 173, 186,   3,  64,  52, 217, 226, 250, 124, 123,   5, 202,  38, 147,
	118, 126, 255,  82,  85, 212, 207, 206,  59, 227,  47,  16,  58,  17, 182, 189,  28,  42, 223, 183, 170, 213, 119,
	248, 152,   2,  44, 154, 163,  70, 221, 153, 101, 155, 167,  43, 172,   9, 129,  22,  39, 253,  19,  98, 108, 110,
	 79, 113, 224, 232, 178, 185, 112, 104, 218, 246,  97, 228, 251,  34, 242, 193, 238, 210, 144,  12, 191, 179, 162,
	241,  81,  51, 145, 235, 249,  14, 239, 107,  49, 192, 214,  31, 181, 199, 106, 157, 184,  84, 204, 176, 115, 121,
	 50,  45, 127,   4, 150, 254, 138, 236, 205,  93, 222, 114,  67,  29,  24,  72, 243, 141, 128, 195,  78,  66, 215,
	 61, 156, 180,
};

namespace PerlinNoise
{
	float Grad(int index, float x, float y, float z);	// 勾配を求める
	float Smootherstep(float number);					// 値を滑らかにする
	int Inc(int number);								// ループ対応を入れる
	float Lerp(float start, float end, float t);		// 線形補完を行う
}

float PerlinNoise::Noise(VECTOR3 p)
{
	// 格子点を求める 3次元だから8つ
	// 255と論理積を取り、255より大きくならない・小数点以下を切り捨てる・負の値の場合、マイナスを外す
	int xi = (int)floor(p.x) & 255;
	int yi = (int)floor(p.y) & 255;
	int zi = (int)floor(p.z) & 255;

	float xf = p.x - floor(p.x);
	float yf = p.y - floor(p.y);
	float zf = p.z - floor(p.z);

	// 補完係数
	float u = PerlinNoise::Smootherstep(xf);
	float v = PerlinNoise::Smootherstep(yf);
	float w = PerlinNoise::Smootherstep(zf);

	auto perm = [](int i) { return PERLIN_NOISE[i & 255]; };

	// 8つの頂点
	int p0 = perm(perm(perm(xi) + yi) + zi);
	int p1 = perm(perm(perm(Inc(xi)) + yi) + zi);
	int p2 = perm(perm(perm(xi) + Inc(yi)) + zi);
	int p3 = perm(perm(perm(xi) + yi) + Inc(zi));
	int p4 = perm(perm(perm(Inc(xi)) + Inc(yi)) + zi);
	int p5 = perm(perm(perm(xi) + Inc(yi)) + Inc(zi));
	int p6 = perm(perm(perm(Inc(xi)) + yi) + Inc(zi));
	int p7 = perm(perm(perm(Inc(xi)) + Inc(yi)) + Inc(zi));

	// 勾配計算
	float g0 = PerlinNoise::Grad(p0, xf, yf, zf);
	float g1 = PerlinNoise::Grad(p1, xf - 1.0f, yf, zf);
	float g2 = PerlinNoise::Grad(p2, xf, yf - 1.0f, zf);
	float g3 = PerlinNoise::Grad(p3, xf, yf, zf - 1.0f);
	float g4 = PerlinNoise::Grad(p4, xf - 1.0f, yf - 1.0f, zf);
	float g5 = PerlinNoise::Grad(p5, xf, yf - 1.0f, zf - 1.0f);
	float g6 = PerlinNoise::Grad(p6, xf - 1.0f, yf, zf - 1.0f);
	float g7 = PerlinNoise::Grad(p7, xf - 1.0f, yf - 1.0f, zf - 1.0f);

	// 各方向の補完
	float x0 = PerlinNoise::Lerp(g0, g1, u);
	float x1 = PerlinNoise::Lerp(g2, g3, u);
	float x2 = PerlinNoise::Lerp(g4, g5, u);
	float x3 = PerlinNoise::Lerp(g6, g7, u);
	float y0 = PerlinNoise::Lerp(x0, x1, v);
	float y1 = PerlinNoise::Lerp(x2, x3, v);
	float z0 = PerlinNoise::Lerp(y0, y1, w);

	return float(z0 + 1.0f) * 0.5f;
}

float PerlinNoise::Grad(int index, float x, float y, float z)
{
	int h = index & 15;
	float u = h < 8 ? x : y;
	int v;

	if (h < 4)
	{
		v = y;
	}
	else if (h == 12 || h == 14)
	{
		v = x;
	}
	else
	{
		v = z;
	}
	return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}

float PerlinNoise::Smootherstep(float number) {
	return number * number * number * (number * (number * 6.0f - 15.0f) + 10.0f);
}

int PerlinNoise::Inc(int number)
{
	// 1を足して、256になった場合、0に戻す処理を入れ、ループ対応をする
	return (number + 1) % PERLIN_NOISE_SIZE;
}

float PerlinNoise::Lerp(float start, float end, float t)
{
	// start : 始点、end : 終点、t : 補完係数
	return (end - start) * t + start;
}