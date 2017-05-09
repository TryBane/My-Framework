#pragma once
#include <vector>
#include <map>
#include <array>
#include <DirectXMath.h>


struct Triangle
{
	short vertex[3];
};

namespace icosahedron
{
	const float X=.525731112119133606f;
	const float Z=.850650808352039932f;
	const float N=0.f;

	static const std::vector<DirectX::XMVECTOR> vertices =
	{
		{-X,N,Z}, {X,N,Z}, {-X,N,-Z}, {X,N,-Z},
		{N,Z,X}, {N,Z,-X}, {N,-Z,X}, {N,-Z,-X},
		{Z,X,N}, {-Z,X, N}, {Z,-X,N}, {-Z,-X, N}
	};

	static const std::vector<Triangle> triangles =
	{
		{0,4,1},{0,9,4},{9,5,4},{4,5,8},{4,8,1},
		{8,10,1},{8,3,10},{5,3,8},{5,2,3},{2,7,3},
		{7,10,3},{7,6,10},{7,11,6},{11,0,6},{0,1,6},
		{6,1,10},{9,0,11},{9,11,2},{9,2,5},{7,2,11}
	};

	inline short vertex_for_edge( std::map<std::pair<short,short>,short>& lookup,
		std::vector<DirectX::XMVECTOR>& vertices,short first,short second )
	{
		std::map<std::pair<short,short>,short>::key_type key( first,second );
		if( key.first > key.second )
			std::swap( key.first,key.second );

		auto inserted = lookup.insert( { key, vertices.size( ) } );
		if( inserted.second )
		{
			auto edge1 = vertices[first];
			auto edge2 = vertices[ second ];
			auto point = DirectX::XMVector3Normalize( DirectX::XMVectorAdd( edge1,edge2 ) );
			vertices.push_back( point );
		}

		return inserted.first->second;
	}

	inline std::vector<Triangle> subdivide( std::vector<DirectX::XMVECTOR>& vertices,
		std::vector<Triangle> triangles )
	{
		std::map<std::pair<short,short>,short> lookup;
		std::vector<Triangle> result;

		for( auto&& each : triangles )
		{
			std::array<short,3> mid;
			for( int edge = 0; edge < 3; ++edge )
			{
				mid[edge] = vertex_for_edge( lookup,vertices,
					each.vertex[edge],each.vertex[(edge + 1) % 3] );
			}

			result.push_back( { each.vertex[0], mid[0], mid[2] } );
			result.push_back( { each.vertex[1], mid[1], mid[0] } );
			result.push_back( { each.vertex[2], mid[2], mid[1] } );
			result.push_back( { mid[0], mid[1], mid[2] } );
		}

		return result;
	}

	inline std::pair<std::vector<DirectX::XMVECTOR>,std::vector<Triangle>> make_icosphere( int subdivisions )
	{
		std::vector<DirectX::XMVECTOR> vertices = icosahedron::vertices;
		std::vector<Triangle> triangles = icosahedron::triangles;

		for( int i = 0; i < subdivisions; ++i )
		{
			triangles = subdivide( vertices,triangles );
		}

		return{ vertices, triangles };
	}
}