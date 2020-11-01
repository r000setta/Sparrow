cbuffer cbPerObject : register(b0)
{
	float4x4 gWorld; 
	float4 gPulseColor;
	float gTime;
};

cbuffer cbPass : register(b1)
{
	float4x4 gViewProj;
};

struct VertexIn
{
	float3 PosL  : POSITION;
    float4 Color : COLOR;
};

struct VertexOut
{
	float4 PosH  : SV_POSITION;
    float4 Color : COLOR;
};

VertexOut VS(VertexIn vin)
{
	VertexOut vout;
	
	// Transform to homogeneous clip space.
	float3 PosW=mul(float4(vin.PosL,1.0f),gWorld).xyz;
	vout.PosH = mul(float4(PosW, 1.0f), gViewProj);
	//vout.PosH.xy+=0.5*sin(vin.PosL.x)*sin(3.0f*gTime);
	//vout.PosH.z*=0.6f+0.4f*sin(2.0f*gTime);
	
	// Just pass vertex color into the pixel shader.
    vout.Color = vin.Color;
    
    return vout;
}

float4 PS(VertexOut pin) : SV_Target
{
	const float pi=3.1415926;
	float s=0.5f*sin(2*gTime-0.25*pi)+0.5f;
	float4 c=lerp(pin.Color,gPulseColor,s);
    return c;
}