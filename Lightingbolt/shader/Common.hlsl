struct Vertex
{ 
    float2 Position : POSITION;
	float2 Size : TEXCOORD0;
	float2 Rotation : TEXCOORD1;
	float2 Params : TEXCOORD2;
};

struct PSInput
{
	float4 Position : SV_POSITION;
	float2 TexCoord : TEXCOORD0;
	float2 Params : TEXCOORD1;
};

struct Material {
	float Refraction;
};

cbuffer Constants : register(b0)
{
	Material c_materials[8];
};

struct Photon {
	float2 Position : POSITION;
	float2 Direction : TEXCOORD0;
	float Energy : TEXCOORD1;
};

struct PhotonOut {
	float4 Position : SV_POSITION;
	float2 Direction : TEXCOORD0;
	float Energy : TEXCOORD1;
};