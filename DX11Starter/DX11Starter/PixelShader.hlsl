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
	DirectionalLight light;
	DirectionalLight light2;
};

// A6
Texture2D DiffuseTexture : register(t0);
SamplerState Sampler : register(s0);

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
	float3 normal		: NORMAL;			// normal
	float2 uv			: TEXCOORD;			// uv
	// float4 color		: COLOR;
};

// Helper method to calculate the final light color for each source
float4 CalculateFinalLightColor(float3 normal, DirectionalLight light)
{
	float3 normalDirToLight = normalize(light.Direction);

	float NdotL = dot(normal, -normalDirToLight);
	NdotL = saturate(NdotL);

	return float4((light.AmbientColor + (light.DiffuseColor * NdotL)));
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
	// A5 - Normalize
	input.normal = normalize(input.normal);

	// Lighting Calculations

	// Directional Light 1
	float4 dLight1 = CalculateFinalLightColor(input.normal, light);
	// Directional Light 2
	float4 dLight2 = CalculateFinalLightColor(input.normal, light2);
	
	// A6 - Textures
	float4 surfaceColor = DiffuseTexture.Sample(Sampler, input.uv);

	// Just return the input color
	// - This color (like most values passing through the rasterizer) is 
	//   interpolated for each pixel between the corresponding vertices 
	//   of the triangle we're rendering
	return dLight1 * surfaceColor + dLight2 * surfaceColor;
}