#include "Common.hlsl"

[maxvertexcount(100)]
void main( point Vertex _in[1], inout PointStream<PhotonOut> _out )
{
	PhotonOut vertexOut;
	vertexOut.Position.z = 0.5; vertexOut.Position.w = 1;
	vertexOut.Energy = 1.0;
	vertexOut.Position.xy = _in[0].Position;
	for( int i=0; i<100; ++i )
	{
		float rndAngle = RandomSample(_in[0].Position, i * 346) * 6.283185307;
		vertexOut.Direction = float2(sin(rndAngle),cos(rndAngle)) * 0.01;
		_out.Append(vertexOut);
	}
}