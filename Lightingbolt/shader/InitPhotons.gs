#include "Common.hlsl"

[maxvertexcount(84)]
void main( point Vertex _in[1], uint _primitiveID: SV_PrimitiveID, inout PointStream<PhotonOut> _out )
{
	PhotonOut vertexOut;
	vertexOut.Position.z = 0.5; vertexOut.Position.w = 1;
	vertexOut.Energy = 10*_in[0].Param.x * c_materials[_in[0].Shape.z].Color;
	vertexOut.EnergyOut = float3(0,0,0);
	vertexOut.Position.xy = _in[0].Position;
	for( int i=0; i<84; ++i )
	{
		float rndAngle = RandomSample(_in[0].Position, i * 346) * 6.283185307;
		vertexOut.Direction = float2(sin(rndAngle),cos(rndAngle)) * 0.01;
		_out.Append(vertexOut);
	}
}