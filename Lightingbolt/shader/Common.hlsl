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
	float c_time;
	float2 c_mapSize;
	float c_lightScale;
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
	float2 Energy : TEXCOORD1;		// x remaining energy, y part to be saved to the map
};



int HashInt( int _a )
{
	_a -= _a << 6;
	_a ^= _a >> 17;
	_a -= _a << 9;
	_a ^= _a << 4;
	_a -= _a << 3;
	_a ^= _a << 10;
	_a ^= _a >> 15;
	return _a;
}

// Standard random variable [0,1]
float RandomSample( float2 _val, int _seed )
{
	return HashInt(int(_val.x*2011.364 + _val.y*4501001.21 + _seed + c_time * 1234.456)) * 4.656612873e-10;
}