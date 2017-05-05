cbuffer ConstantBuffer
{
	float4 Offset;
};

struct VOut
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

VOut main(float4 position : POSITION, float4 color : COLOR)
{
    VOut output;

    output.position = position;
	output.position.x += Offset.x;
	output.position.y += Offset.y;
	output.position.xy *= Offset.z;
	output.position.w += Offset.w;
    output.color = color;

    return output;
}