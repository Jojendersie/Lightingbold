#include "Common.hlsl"

[maxvertexcount(32)]
void main( point Vertex _in[1], inout PointStream<PhotonOut> _out )
{
	PhotonOut vertexOut;
	vertexOut.Position.z = 0.5; vertexOut.Position.w = 1;
	vertexOut.Energy = 1.0;
	vertexOut.Direction = float2(0,0.01);
	vertexOut.Position.xy = _in[0].Position;
	_out.Append(vertexOut);
}