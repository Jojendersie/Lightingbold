#include "Common.hlsl"

// TODO: Why is this no vertex shader??

Texture2D g_mapTexture : register(t0);
Texture2D g_refractionTexture : register(t2);

[maxvertexcount(1)]
void main( point Photon _in[1], inout PointStream<PhotonOut> _out )
{
	int3 sampleLocation = int3((_in[0].Position.xy*float2(0.5,-0.5)+0.5)*float2(512,512), 0);
	float3 refract0 = g_refractionTexture.Load(sampleLocation).xyz;

	PhotonOut vertexOut;
	vertexOut.Position.z = 0.5; vertexOut.Position.w = 1;
	vertexOut.Position.xy = _in[0].Position;
	sampleLocation = int3((_in[0].Position.xy*0.5+0.5)*c_mapSize, 0);
	vertexOut.EnergyOut = ((g_mapTexture.Load(sampleLocation).x) + 0.1) * 0.2 * _in[0].Energy;
	vertexOut.Energy = _in[0].Energy - vertexOut.EnergyOut;
	float speed = RandomSample(vertexOut.Position.xy, 0) * 0.006 + 0.007;
	vertexOut.Position.xy = vertexOut.Position.xy + _in[0].Direction * speed;

	sampleLocation = int3((vertexOut.Position.xy*float2(0.5,-0.5)+0.5)*float2(512,512), 0);
	float3 refract1 = g_refractionTexture.Load(sampleLocation).xyz;
	float strength = (refract0.x - refract1.x) * 10;
	float2 n = normalize((refract0.yz * 2 - 1) + (refract1.yz * 2 - 1));
	float2 refractDirection = _in[0].Direction - dot(_in[0].Direction, n) * n;
	vertexOut.Direction = normalize(_in[0].Direction + refractDirection * strength);

	_out.Append(vertexOut);
}