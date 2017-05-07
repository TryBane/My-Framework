#pragma once
#include <vector>
#include "CoreGraphics.h"
#include <DirectXColors.h>


static std::vector<VERTEX> OurVertices =
{
	{ {-1.0f, -1.0f, 1.0f }, { 0.0f, 0.0f, 1.0f}, DirectX::Colors::White },    // side 1
	{ {1.0f, -1.0f, 1.0f  }, { 0.0f, 0.0f, 1.0f}, DirectX::Colors::White }, 
	{ {-1.0f, 1.0f, 1.0f  }, { 0.0f, 0.0f, 1.0f}, DirectX::Colors::White },
	{ {1.0f, 1.0f, 1.0f   }, { 0.0f, 0.0f, 1.0f}, DirectX::Colors::White },
	 						
	{ {-1.0f, -1.0f, -1.0f}, { 0.0f, 0.0f, -1.0f}, DirectX::Colors::White },    // side 2
	{ {-1.0f, 1.0f, -1.0f }, { 0.0f, 0.0f, -1.0f}, DirectX::Colors::White },
	{ {1.0f, -1.0f, -1.0f }, { 0.0f, 0.0f, -1.0f}, DirectX::Colors::White },
	{ {1.0f, 1.0f, -1.0f  }, { 0.0f, 0.0f, -1.0f}, DirectX::Colors::White },
	 						 
	{ {-1.0f, 1.0f, -1.0f }, { 0.0f, 1.0f, 0.0f}, DirectX::Colors::White },    // side 3
	{ {-1.0f, 1.0f, 1.0f  }, { 0.0f, 1.0f, 0.0f}, DirectX::Colors::White },
	{ {1.0f, 1.0f, -1.0f  }, { 0.0f, 1.0f, 0.0f}, DirectX::Colors::White },
	{ {1.0f, 1.0f, 1.0f   }, { 0.0f, 1.0f, 0.0f}, DirectX::Colors::White },
	 						
	{ {-1.0f, -1.0f, -1.0f}, { 0.0f, -1.0f, 0.0f}, DirectX::Colors::White },    // side 4
	{ {1.0f, -1.0f, -1.0f }, { 0.0f, -1.0f, 0.0f}, DirectX::Colors::White },
	{ {-1.0f, -1.0f, 1.0f }, { 0.0f, -1.0f, 0.0f}, DirectX::Colors::White },
	{ {1.0f, -1.0f, 1.0f  }, { 0.0f, -1.0f, 0.0f}, DirectX::Colors::White },
	 						
	{ {1.0f, -1.0f, -1.0f }, { 1.0f, 0.0f, 0.0f}, DirectX::Colors::White },    // side 5
	{ {1.0f, 1.0f, -1.0f  }, { 1.0f, 0.0f, 0.0f}, DirectX::Colors::White },
	{ {1.0f, -1.0f, 1.0f  }, { 1.0f, 0.0f, 0.0f}, DirectX::Colors::White },
	{ {1.0f, 1.0f, 1.0f   }, { 1.0f, 0.0f, 0.0f}, DirectX::Colors::White },
	 						
	{ {-1.0f, -1.0f, -1.0f}, { -1.0f, 0.0f, 0.0f}, DirectX::Colors::White },    // side 6
	{ {-1.0f, -1.0f, 1.0f }, { -1.0f, 0.0f, 0.0f}, DirectX::Colors::White },
	{ {-1.0f, 1.0f, -1.0f }, { -1.0f, 0.0f, 0.0f}, DirectX::Colors::White },
	{ {-1.0f, 1.0f, 1.0f  }, { -1.0f, 0.0f, 0.0f}, DirectX::Colors::White },

};