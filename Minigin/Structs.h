#pragma once
struct Rect
{
	int x;
	int y;
	int width;
	int height;
	Rect()
		: x{ 0 }
		, y{ 0 }
		, width{ 0 }
		, height{ 0 }
	{}
	Rect(int x, int y, int w, int h)
		: x{ x }
		, y{ y }
		, width{ w }
		, height{ h }
	{}
};
