#include "Common.hlsl"

[maxvertexcount(1)]
void main( point Photon _in[1], inout PointStream<PhotonOut> _out )
{
	PhotonOut vertexOut;
	vertexOut.Position.z = 0.5; vertexOut.Position.w = 1;
	vertexOut.Energy = _in[0].Energy;
	vertexOut.Direction = _in[0].Direction;
	vertexOut.Position.xy = _in[0].Position + _in[0].Direction;
	_out.Append(vertexOut);
}