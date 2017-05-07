#pragma once
#include <vector>
#include "CoreGraphics.h"
#include <DirectXColors.h>


static std::vector<VERTEX> OurVertices =
{
	{ {-1.0f, -1.0f, 1.0f }, { 0.0f, 0.0f, 1.0f}, DirectX::Colors::Blue },    // side 1
	{ {1.0f, -1.0f, 1.0f  }, { 0.0f, 0.0f, 1.0f}, DirectX::Colors::Blue }, 
	{ {-1.0f, 1.0f, 1.0f  }, { 0.0f, 0.0f, 1.0f}, DirectX::Colors::Blue },
	{ {1.0f, 1.0f, 1.0f   }, { 0.0f, 0.0f, 1.0f}, DirectX::Colors::Blue },
	 						
	{ {-1.0f, -1.0f, -1.0f}, { 0.0f, 0.0f, -1.0f}, DirectX::Colors::Green },    // side 2
	{ {-1.0f, 1.0f, -1.0f }, { 0.0f, 0.0f, -1.0f}, DirectX::Colors::Green },
	{ {1.0f, -1.0f, -1.0f }, { 0.0f, 0.0f, -1.0f}, DirectX::Colors::Green },
	{ {1.0f, 1.0f, -1.0f  }, { 0.0f, 0.0f, -1.0f}, DirectX::Colors::Green },
	 						 
	{ {-1.0f, 1.0f, -1.0f }, { 0.0f, 1.0f, 0.0f}, DirectX::Colors::Yellow },    // side 3
	{ {-1.0f, 1.0f, 1.0f  }, { 0.0f, 1.0f, 0.0f}, DirectX::Colors::Yellow },
	{ {1.0f, 1.0f, -1.0f  }, { 0.0f, 1.0f, 0.0f}, DirectX::Colors::Yellow },
	{ {1.0f, 1.0f, 1.0f   }, { 0.0f, 1.0f, 0.0f}, DirectX::Colors::Yellow },
	 						
	{ {-1.0f, -1.0f, -1.0f}, { 0.0f, -1.0f, 0.0f}, DirectX::Colors::Red },    // side 4
	{ {1.0f, -1.0f, -1.0f }, { 0.0f, -1.0f, 0.0f}, DirectX::Colors::Red },
	{ {-1.0f, -1.0f, 1.0f }, { 0.0f, -1.0f, 0.0f}, DirectX::Colors::Red },
	{ {1.0f, -1.0f, 1.0f  }, { 0.0f, -1.0f, 0.0f}, DirectX::Colors::Red },
	 						
	{ {1.0f, -1.0f, -1.0f }, { 1.0f, 0.0f, 0.0f}, DirectX::Colors::Orange },    // side 5
	{ {1.0f, 1.0f, -1.0f  }, { 1.0f, 0.0f, 0.0f}, DirectX::Colors::Orange },
	{ {1.0f, -1.0f, 1.0f  }, { 1.0f, 0.0f, 0.0f}, DirectX::Colors::Orange },
	{ {1.0f, 1.0f, 1.0f   }, { 1.0f, 0.0f, 0.0f}, DirectX::Colors::Orange },
	 						
	{ {-1.0f, -1.0f, -1.0f}, { -1.0f, 0.0f, 0.0f}, DirectX::Colors::White },    // side 6
	{ {-1.0f, -1.0f, 1.0f }, { -1.0f, 0.0f, 0.0f}, DirectX::Colors::White },
	{ {-1.0f, 1.0f, -1.0f }, { -1.0f, 0.0f, 0.0f}, DirectX::Colors::White },
	{ {-1.0f, 1.0f, 1.0f  }, { -1.0f, 0.0f, 0.0f}, DirectX::Colors::White },

};