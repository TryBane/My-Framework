#pragma once
#include <vector>
#include "CoreGraphics.h"
#include <DirectXColors.h>


static std::vector<VERTEX> OurVertices =
{
	{{-0.25f,  0.25f,-0.25f},DirectX::Colors::White},	   //0
	{{ 0.25f,  0.25f,-0.25f},DirectX::Colors::Blue},	   //1
	{{-0.25f, -0.25f,-0.25f},DirectX::Colors::Black},	   //2
	{{ 0.25f, -0.25f,-0.25f},DirectX::Colors::Red},		   //3
	{{-0.25f,  0.25f, 0.25f},DirectX::Colors::Green},	   //4
	{{ 0.25f,  0.25f, 0.25f},DirectX::Colors::Teal},	   //5
	{{-0.25f, -0.25f, 0.25f},DirectX::Colors::Magenta},	   //6
	{{ 0.25f, -0.25f, 0.25f},DirectX::Colors::Gold}		   //7

};