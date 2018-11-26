// A constant buffer that stores the three basic column-major matrices for composing geometry.
cbuffer ModelViewProjectionConstantBuffer : register(b0)
{
	matrix model;
	matrix view;
	matrix projection;
};

// Per-vertex data used as input to the vertex shader.
struct VertexShaderInput
{
	float3 pos : POSITION;
	float3 color : COLOR0;
};

// Per-pixel color data passed through the pixel shader.
struct PixelShaderInput
{
	float4 pos : SV_POSITION;
	float3 color : COLOR0;
};

// Simple shader to do vertex processing on the GPU.
PixelShaderInput main(VertexShaderInput input)
{
	PixelShaderInput output;

	float4 pos = float4(input.pos.x, input.pos.y, input.pos.z, 1.0f);

	// Transform the vertex position into projected space.
	//pos = mul(pos, model);
	pos = mul(pos, view);
	pos = mul(pos, projection);

	output.pos = float4(pos.x, pos.y, pos.z, pos.w);

	// Pass the color through without modification.
	//output.color = float3(input.color.r * sin(f_B * f_C) * 5, input.color.g * sin(f_C * f_A) * 5, input.color.b * sin(f_B * f_B) * 5);
	output.color = float3(input.color.r, input.color.g, input.color.b);

	return output;
}
