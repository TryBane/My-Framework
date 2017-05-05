cbuffer ConstantBuffer
{
	float4x4 transMatrix;
};

struct VOut
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

VOut main(float4 position : POSITION, float4 color : COLOR)
{
    VOut output;

	output.position = mul(transMatrix, position);    // transform the vertex from 3D to 2D
    //output.position = position;
	//output.position.x += offset.x;
	//output.position.y += offset.y;
	//output.position.xy *= offset.z;
	//output.position.w += offset.w;
    output.color = color;

    return output;
}