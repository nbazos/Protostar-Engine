#pragma once

#include "SimpleShader.h"

class Material {
public:
	Material(SimpleVertexShader* vShader, SimplePixelShader* pShader, ID3D11ShaderResourceView* srv, ID3D11SamplerState* sampler);
	~Material();

	SimpleVertexShader* GetVertexShader() { return vertexShader; };
	SimplePixelShader* GetPixelShader() { return pixelShader; };
	ID3D11ShaderResourceView* GetSRV() { return textureSRV; };
	ID3D11SamplerState* GetSamplerState() { return samplerState; };

private:
	// Wrappers for DirectX shaders to provide simplified functionality
	SimpleVertexShader* vertexShader;
	SimplePixelShader* pixelShader;
	ID3D11ShaderResourceView* textureSRV;
	ID3D11SamplerState* samplerState;
};