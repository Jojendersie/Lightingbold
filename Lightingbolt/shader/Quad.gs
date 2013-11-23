#include "Common.hlsl"

[maxvertexcount(4)]
void main( point Vertex _in[1], inout TriangleStream<PSInput> _out )
{
	PSInput vertexOut;
	vertexOut.Position.z = 0; vertexOut.Position.w = 1;
	vertexOut.Params = _in[0].Params;
	
	vertexOut.TexCoord = float2(-1,-1);
	vertexOut.Position.xy = float2( -_in[0].Size.x*_in[0].Rotation.y-_in[0].Size.y*_in[0].Rotation.x,
									+_in[0].Size.x*_in[0].Rotation.x-_in[0].Size.y*_in[0].Rotation.y );
	_out.Append(vertexOut);

	vertexOut.TexCoord = float2(1,-1);
	vertexOut.Position.xy = float2( +_in[0].Size.x*_in[0].Rotation.y-_in[0].Size.y*_in[0].Rotation.x,
									-_in[0].Size.x*_in[0].Rotation.x-_in[0].Size.y*_in[0].Rotation.y );
	_out.Append(vertexOut);

	vertexOut.TexCoord = float2(-1,1);
	vertexOut.Position.xy = float2( -_in[0].Size.x*_in[0].Rotation.y+_in[0].Size.y*_in[0].Rotation.x,
									+_in[0].Size.x*_in[0].Rotation.x+_in[0].Size.y*_in[0].Rotation.y );
	_out.Append(vertexOut);

	vertexOut.TexCoord = float2(1,1);
	vertexOut.Position.xy = float2( +_in[0].Size.x*_in[0].Rotation.y+_in[0].Size.y*_in[0].Rotation.x,
									-_in[0].Size.x*_in[0].Rotation.x+_in[0].Size.y*_in[0].Rotation.y );
	_out.Append(vertexOut);
}