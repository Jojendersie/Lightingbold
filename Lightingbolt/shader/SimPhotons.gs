#include "Common.hlsl"

Texture2D g_mapTexture : register(t0);

[maxvertexcount(1)]
void main( point Photon _in[1], inout PointStream<PhotonOut> _out )
{
	PhotonOut vertexOut;
	vertexOut.Position.z = 0.5; vertexOut.Position.w = 1;
	vertexOut.Position.xy = _in[0].Position;
	int3 sampleLocation = int3((_in[0].Position.xy*0.5+0.5)*c_mapSize, 0);
	vertexOut.EnergyOut =((g_mapTexture.Load(sampleLocation).x) + 0.1) * 0.2 * _in[0].Energy;
	vertexOut.Energy = _in[0].Energy - vertexOut.EnergyOut;
	vertexOut.Direction = _in[0].Direction;
	float speed = RandomSample(vertexOut.Position.xy, 0) * 0.6 + 0.7;
	vertexOut.Position.xy = vertexOut.Position.xy + _in[0].Direction * speed;
	_out.Append(vertexOut);
}