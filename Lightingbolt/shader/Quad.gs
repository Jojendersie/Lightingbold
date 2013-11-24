#include "Common.hlsl"

[maxvertexcount(4)]
void main( point Vertex _in[1], inout TriangleStream<PSInput> _out )
{
	PSInput vertexOut;
	vertexOut.Position.z = 0.5; vertexOut.Position.w = 1;
	vertexOut.Param = _in[0].Param;
	vertexOut.Shape = _in[0].Shape;
	
	vertexOut.TexCoord = float2(-1,-1);
	vertexOut.Position.xy = _in[0].Size * vertexOut.TexCoord + _in[0].Position;
	_out.Append(vertexOut);

	vertexOut.TexCoord = float2(1,-1);
	vertexOut.Position.xy = _in[0].Size * vertexOut.TexCoord + _in[0].Position;
	_out.Append(vertexOut);

	vertexOut.TexCoord = float2(-1,1);
	vertexOut.Position.xy = _in[0].Size * vertexOut.TexCoord + _in[0].Position;
	_out.Append(vertexOut);

	vertexOut.TexCoord = float2(1,1);
	vertexOut.Position.xy = _in[0].Size * vertexOut.TexCoord + _in[0].Position;
	_out.Append(vertexOut);
}