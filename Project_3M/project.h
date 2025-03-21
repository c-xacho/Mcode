#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <conio.h>
#include <graphics.h>
#include <iostream>;
#include <vector>;
#include "tools.hpp";
#include <windows.h>
using namespace std;
const int width = 382, height = 700;
class Enemy {
public:
	Enemy(IMAGE& img, int x) :img(img) {
		rect.left = x;
		rect.right = rect.left + img.getwidth();
		rect.top = -img.getheight();
		rect.bottom = 0;
	}
	bool Show() {
		if (rect.top >= height) {
			return false;
		}
		rect.top += 4;
		rect.bottom += 4;
		drawImg(rect.left, rect.top, &img);
		return true;
	}
	RECT& getRECT() {
		return rect;
	}
private:
	RECT rect;
	IMAGE& img;
	int x;
};
class Bullet {
public:
	Bullet(IMAGE& img, RECT r1) : img(img){
		rect.left = r1.left + (r1.right - r1.left) / 2 - img.getwidth() / 2;
		rect.right = rect.left + img.getwidth();
		rect.top = r1.top - img.getheight();
		rect.bottom = r1.top;
	}
	bool Show() {
		if (rect.bottom <= 0) {
			return false;
		}
		rect.top -= 6;
		rect.bottom -= 6;
		drawImg(rect.left, rect.top, &img);
		return true;
	}
	RECT& getRECT() {
		return rect;
	}
private:
	IMAGE& img;
	RECT rect;
};
class Player {
public:
	Player(IMAGE& img) :img(img) {
		rect.left = (width - img.getwidth()) / 2;
		rect.right = rect.left + img.getwidth();
		rect.top = height - img.getheight();
		rect.bottom = height;
	}
	void Show() {
		drawImg(rect.left, rect.top, &img);
	}
	void Control() {
		ExMessage meg;
		if (peekmessage(&meg, EX_MOUSE)) {
			rect.left = meg.x - img.getwidth() / 2;
			rect.right = rect.left + img.getwidth();
			rect.top = meg.y - img.getheight() / 2;
			rect.bottom = rect.top + img.getheight() / 2;
		}
	}
	RECT& getRECT() {
		return rect;
	}
private:
	IMAGE& img;
	RECT rect;
};
class Background {
public:
	Background(IMAGE& img) : img(img), y(-height) {
	}
	void Show() {
		if (y == 0) {
			y = -height;
		}
		y += 2;
		putimage(0, y, &img);
	}
private:
	IMAGE& img;
	int y;
};
