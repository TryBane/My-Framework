#pragma once
#include <vector>
#include <map>
#include <array>
#include <iterator>
#include <DirectXMath.h>
#include <math.h>

namespace icosahedron
{
	static float X=.525731112119133606f;
	static float Z=.850650808352039932f;
	static float N=0.f;

	using TriangleList=std::vector<short>;

	static const std::vector<DirectX::XMVECTOR> vertices =
	{
		{-X,N,Z}, {X,N,Z}, {-X,N,-Z}, {X,N,-Z},
		{N,Z,X}, {N,Z,-X}, {N,-Z,X}, {N,-Z,-X},
		{Z,X,N}, {-Z,X, N}, {Z,-X,N}, {-Z,-X, N}
	};

	static const TriangleList triangles =
	{
		0,4,1,0,9,4,9,5,4,4,5,8,4,8,1,
		8,10,1,8,3,10,5,3,8,5,2,3,2,7,3,
		7,10,3,7,6,10,7,11,6,11,0,6,0,1,6,
		6,1,10,9,0,11,9,11,2,9,2,5,7,2,11
	};

	inline void setIcosahedronLocation( float x, float y, float z )
	{
		X = x;
		Z = z;
		N = y;
	}

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
			vertices.push_back( DirectX::XMVector3Normalize( DirectX::XMVectorAdd( edge1,edge2 ) ) );
		}

		return inserted.first->second;
	}


	inline TriangleList subdivide( std::vector<DirectX::XMVECTOR>& vertices,
		TriangleList triangles)
	{
		std::map<std::pair<short,short>,short> lookup;
		TriangleList result = triangles;

		for( auto begin = 0; begin < triangles.size() - 2; begin++, begin++, begin++)
		{

			std::array<short,3> mid;
			for( int edge = 0; edge < 3; ++edge )
			{
				mid[edge] = vertex_for_edge( lookup,vertices,
					triangles[begin + edge],triangles[begin + ( (edge + 1) % 3 )] );
			}

			result.push_back( triangles[ begin ] );
			result.push_back( mid[0] );
			result.push_back( mid[2] );

			result.push_back( triangles[ begin + 1 ] );
			result.push_back( mid[1] );
			result.push_back( mid[0] );

			result.push_back( triangles[ begin + 2 ] );
			result.push_back( mid[2] );
			result.push_back( mid[1] );

			result.push_back( mid[0] );
			result.push_back( mid[1] );
			result.push_back( mid[2] );

		}

		return result;
	}

	inline std::pair<std::vector<DirectX::XMVECTOR>,TriangleList> make_icosphere( int subdivisions )
	{
		std::vector<DirectX::XMVECTOR> vertices = icosahedron::vertices;
		TriangleList triangles = icosahedron::triangles;

		for( int i = 0; i < subdivisions; ++i )
		{
			triangles = subdivide( vertices,triangles );

		}

		std::vector<short>::iterator first = triangles.begin( );
		triangles.erase( first,first + ( 60 * ( subdivisions * pow( 4,subdivisions - 1) ) ) );

		return{ vertices, triangles };
	}
}