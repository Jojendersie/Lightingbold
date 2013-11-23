#include "Common.hlsl"

[maxvertexcount(1)]
void main( point Photon _in[1], inout PointStream<PhotonOut> _out )
{
	PhotonOut vertexOut;
	vertexOut.Position.z = 0.5; vertexOut.Position.w = 1;
	vertexOut.Position.xy = _in[0].Position;
	vertexOut.Energy = _in[0].Energy;
	vertexOut.Direction = _in[0].Direction;
	float speed = RandomSample(vertexOut.Position.xy, 0) * 0.6 + 0.7;
	vertexOut.Position.xy = vertexOut.Position.xy + _in[0].Direction * speed;
	_out.Append(vertexOut);
}