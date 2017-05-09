#pragma once
#include <vector>
#include "CoreGraphics.h"
#include <DirectXColors.h>


//static std::vector<VERTEX> OurVertices =
//{
//	{ {-1.0f, -1.0f, 1.0f }, { 0.0f, 0.0f, 1.0f}, DirectX::Colors::White,  {0.0f, 0.0f} },    // side 1
//	{ {1.0f, -1.0f, 1.0f  }, { 0.0f, 0.0f, 1.0f}, DirectX::Colors::White,  {1.0f, 0.0f} }, 
//	{ {-1.0f, 1.0f, 1.0f  }, { 0.0f, 0.0f, 1.0f}, DirectX::Colors::White,  {0.0f, 1.0f} },
//	{ {1.0f, 1.0f, 1.0f   }, { 0.0f, 0.0f, 1.0f}, DirectX::Colors::White,  {1.0f, 1.0f} },
//	 						
//	{ {-1.0f, -1.0f, -1.0f}, { 0.0f, 0.0f, -1.0f}, DirectX::Colors::White,  {0.0f, 0.0f} },    // side 2
//	{ {-1.0f, 1.0f, -1.0f }, { 0.0f, 0.0f, -1.0f}, DirectX::Colors::White,  {1.0f, 0.0f} },
//	{ {1.0f, -1.0f, -1.0f }, { 0.0f, 0.0f, -1.0f}, DirectX::Colors::White,  {0.0f, 1.0f} },
//	{ {1.0f, 1.0f, -1.0f  }, { 0.0f, 0.0f, -1.0f}, DirectX::Colors::White,  {1.0f, 1.0f} },
//	 						 
//	{ {-1.0f, 1.0f, -1.0f }, { 0.0f, 1.0f, 0.0f}, DirectX::Colors::White,  {0.0f, 0.0f} },    // side 3
//	{ {-1.0f, 1.0f, 1.0f  }, { 0.0f, 1.0f, 0.0f}, DirectX::Colors::White,  {1.0f, 0.0f} },
//	{ {1.0f, 1.0f, -1.0f  }, { 0.0f, 1.0f, 0.0f}, DirectX::Colors::White,  {0.0f, 1.0f} },
//	{ {1.0f, 1.0f, 1.0f   }, { 0.0f, 1.0f, 0.0f}, DirectX::Colors::White,  {1.0f, 1.0f} },
//	 						
//	{ {-1.0f, -1.0f, -1.0f}, { 0.0f, -1.0f, 0.0f}, DirectX::Colors::White,  {0.0f, 0.0f} },    // side 4
//	{ {1.0f, -1.0f, -1.0f }, { 0.0f, -1.0f, 0.0f}, DirectX::Colors::White,  {1.0f, 0.0f} },
//	{ {-1.0f, -1.0f, 1.0f }, { 0.0f, -1.0f, 0.0f}, DirectX::Colors::White,  {0.0f, 1.0f} },
//	{ {1.0f, -1.0f, 1.0f  }, { 0.0f, -1.0f, 0.0f}, DirectX::Colors::White,  {1.0f, 1.0f} },
//	 						
//	{ {1.0f, -1.0f, -1.0f }, { 1.0f, 0.0f, 0.0f}, DirectX::Colors::White,  {0.0f, 0.0f} },    // side 5
//	{ {1.0f, 1.0f, -1.0f  }, { 1.0f, 0.0f, 0.0f}, DirectX::Colors::White,  {1.0f, 0.0f} },
//	{ {1.0f, -1.0f, 1.0f  }, { 1.0f, 0.0f, 0.0f}, DirectX::Colors::White,  {0.0f, 1.0f} },
//	{ {1.0f, 1.0f, 1.0f   }, { 1.0f, 0.0f, 0.0f}, DirectX::Colors::White,  {1.0f, 1.0f} },
//	 						
//	{ {-1.0f, -1.0f, -1.0f}, { -1.0f, 0.0f, 0.0f}, DirectX::Colors::White,  {0.0f, 0.0f} },    // side 6
//	{ {-1.0f, -1.0f, 1.0f }, { -1.0f, 0.0f, 0.0f}, DirectX::Colors::White,  {1.0f, 0.0f} },
//	{ {-1.0f, 1.0f, -1.0f }, { -1.0f, 0.0f, 0.0f}, DirectX::Colors::White,  {0.0f, 1.0f} },
//	{ {-1.0f, 1.0f, 1.0f  }, { -1.0f, 0.0f, 0.0f}, DirectX::Colors::White,  {1.0f, 1.0f} },
//
//};
//
//static std::vector<VERTEX> TextureVertices =
//{
//	{{-3.0f, -3.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f},  {0.0f, 0.0f}},
//	{{3.0f, -3.0f, 1.0f},  {0.0f, 0.0f, 1.0f}, {0.0f},  {1.0f, 0.0f}},
//	{{-3.0f, 3.0f, 1.0f},  {0.0f, 0.0f, 1.0f}, {0.0f},  {0.0f, 1.0f}},
//	{{3.0f, 3.0f, 1.0f},   {0.0f, 0.0f, 1.0f}, {0.0f},  {1.0f, 1.0f}},
//};