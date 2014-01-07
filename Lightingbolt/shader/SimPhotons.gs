#include "Common.hlsl"

Texture2D g_mapTexture : register(t0);
Texture2D g_refractionTexture : register(t2);

[maxvertexcount(1)]
void main( point Photon _in[1], inout PointStream<PhotonOut> _out )
{
	int3 sampleLocation = int3((_in[0].Position.xy*float2(0.5,-0.5)+0.5)*float2(2048,2048), 0);
	float3 refract0 = g_refractionTexture.Load(sampleLocation).xyz;

	PhotonOut vertexOut;
	vertexOut.Position.z = 0.5; vertexOut.Position.w = 1;
	vertexOut.Position.xy = _in[0].Position;
	sampleLocation = int3((_in[0].Position.xy*0.5+0.5)*c_mapSize, 0);
	float map00 = g_mapTexture.Load(sampleLocation).x;
	float map01 = g_mapTexture.Load(sampleLocation+int3(0,1,0)).x;
	float map10 = g_mapTexture.Load(sampleLocation+int3(1,0,0)).x;

	float speed = RandomSample(vertexOut.Position.xy, 0) * 0.006 + 0.007;
	vertexOut.Position.xy = vertexOut.Position.xy + _in[0].Direction * speed;

	sampleLocation = int3((vertexOut.Position.xy*float2(0.5,-0.5)+0.5)*float2(2048,2048), 0);
	float3 refract1 = g_refractionTexture.Load(sampleLocation).xyz;
	sampleLocation = int3((vertexOut.Position.xy*0.5+0.5)*c_mapSize, 0);
//	float map1 = g_mapTexture.Load(sampleLocation).x;
	float strength = (refract0.x - refract1.x);
	float2 n = normalize((refract0.yz * 2 - 1) + (refract1.yz * 2 - 1));
	float2 refractDirection = _in[0].Direction - dot(_in[0].Direction, n) * n;
	vertexOut.Direction = normalize(_in[0].Direction + refractDirection * strength);

	float ndotl = saturate(sin(dot(_in[0].Direction, normalize(float2(map10-map00, map01-map00)))*10)) + 0.5;
	vertexOut.EnergyOut = (map00 + 1) * 0.02 * _in[0].Energy;
	vertexOut.Energy = _in[0].Energy - vertexOut.EnergyOut;
	vertexOut.EnergyOut = ndotl * vertexOut.EnergyOut;

	_out.Append(vertexOut);
}