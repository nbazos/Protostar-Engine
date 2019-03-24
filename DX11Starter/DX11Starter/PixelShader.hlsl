
// Struct representing the data we expect to receive from earlier pipeline stages
// - Should match the output of our corresponding vertex shader
// - The name of the struct itself is unimportant
// - The variable names don't have to match other shaders (just the semantics)
// - Each variable must have a semantic, which defines its usage
struct VertexToPixel
{
	// Data type
	//  |
	//  |   Name          Semantic
	//  |    |                |
	//  v    v                v
	float4 position		: SV_POSITION;
	float3 normal       : NORMAL;
	float2 uv           : TEXCOORD;
};

struct DirectionalLight
{
	float4 AmbientColor;
	float4 DiffuseColor;
	float3 Direction;
};

// Constant Buffer
// - Allows us to define a buffer of individual variables 
//    which will (eventually) hold data from our C++ code
// - All non-pipeline variables that get their values from 
//    our C++ code must be defined inside a Constant Buffer
// - The name of the cbuffer itself is unimportant
cbuffer externalData : register(b0)
{
	DirectionalLight light1;
	DirectionalLight light2;
};

// Global variables
Texture2D diffuseTexture  : register(t0);
SamplerState basicSampler : register(s0);

// Helper Functions
float4 CalculateLightColor(float3 normal, DirectionalLight light)
{
	normal = normalize(normal);
	float3 normalizedDir = normalize(-light.Direction);
	float lightAmount = dot(normal, normalizedDir);
	lightAmount = saturate(lightAmount);

	return (light.DiffuseColor * lightAmount) + light.AmbientColor;
}

// --------------------------------------------------------
// The entry point (main method) for our pixel shader
// 
// - Input is the data coming down the pipeline (defined by the struct)
// - Output is a single color (float4)
// - Has a special semantic (SV_TARGET), which means 
//    "put the output of this into the current render target"
// - Named "main" because that's the default the shader compiler looks for
// --------------------------------------------------------
float4 main(VertexToPixel input) : SV_TARGET
{
	return diffuseTexture.Sample(basicSampler, input.uv) * CalculateLightColor(input.normal, light1) + CalculateLightColor(input.normal, light2);
}

