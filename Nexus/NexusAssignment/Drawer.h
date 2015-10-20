#pragma once
#ifndef _DRAWER
#define _DRAWER
#include "Board.h"
//header file for the drawer class

using namespace System::Drawing;

ref class Drawer
{
	//array of different coloured balls
	static array <Bitmap^>^ ball = {
		(Bitmap^)Image::FromFile("img\\free.png"),
		(Bitmap^)Image::FromFile("img\\blue.png"),
		(Bitmap^)Image::FromFile("img\\red.png"),
		(Bitmap^)Image::FromFile("img\\green.png"),
		(Bitmap^)Image::FromFile("img\\yellow.png"),
		(Bitmap^)Image::FromFile("img\\purple.png"),
		(Bitmap^)Image::FromFile("img\\bleau.png"),
		(Bitmap^)Image::FromFile("img\\selected.png"),
	};
	static Graphics^ formGraphics;
public:
	static void init(Graphics^);
	static void draw(Board&);

};

#endif