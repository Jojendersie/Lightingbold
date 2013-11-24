#include "Common.hlsl"

[maxvertexcount(84)]
void main( point Vertex _in[1], uint _primitiveID: SV_PrimitiveID, inout PointStream<PhotonOut> _out )
{
	PhotonOut vertexOut;
	vertexOut.Position.z = 0.5; vertexOut.Position.w = 1;
	// Color from energy difference
	float dif = (c_playerEnergy - _in[0].Param.x) * 5;
	float3 color = (dif < 0) ? lerp(float3(1,1,1),float3(0.3,0.6,1), -dif) : lerp(float3(1,1,1),float3(0.6,1.0,0.3), dif);
	vertexOut.Energy = 30.0 * _in[0].Param.x * color;
	vertexOut.EnergyOut = vertexOut.Energy * 0.02;
	for( int i=0; i<84; ++i )
	{
		float rndAngle = RandomSample(_in[0].Position, i * 346) * 6.283185307;
		vertexOut.Direction = float2(sin(rndAngle),cos(rndAngle));
		float rndStep = RandomSample(_in[0].Position, i) * 0.01;
		vertexOut.Position.xy = _in[0].Position + vertexOut.Direction * rndStep;
		_out.Append(vertexOut);
	}
}