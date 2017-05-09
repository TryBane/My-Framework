cbuffer ConstantBuffer
{
	float4x4 transMatrix;
	float4x4 rotation;
	float4 lightvec;
	float4 lightcol;
	float4 ambientcol;
};

struct VOut
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
	float2 texcoord : TEXCOORD;
};

VOut main(float4 position : POSITION ) //, float4 normal : NORMAL, float4 color : COLOR, float2 texcoord : TEXCOORD )
{
    VOut output;

	output.position = mul(transMatrix, position);    // transform the vertex from 3D to 2D
	//output.color = ambientcol;

	//float4 norm = normalize( mul( rotation,normal ) );
	//float diffusebrightness = saturate( dot( norm, lightvec ) );
	//output.color += lightcol * diffusebrightness;
    //output.position = position;
	//output.position.x += offset.x;
	//output.position.y += offset.y;
	//output.position.xy *= offset.z;
	//output.position.w += offset.w;
    //output.color *= color;
	//output.texcoord = texcoord;

    return output;
}