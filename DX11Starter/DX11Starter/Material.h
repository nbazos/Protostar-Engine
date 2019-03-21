#pragma once

#include "SimpleShader.h"

class Material
{
public:
	Material(SimpleVertexShader* vShader, SimplePixelShader* pShader, ID3D11ShaderResourceView* srvPtr, ID3D11SamplerState* samplerStatePtr);
	Material();
	~Material();

	// member variables
	SimpleVertexShader* vertexShader;
	SimplePixelShader* pixelShader;

	ID3D11ShaderResourceView* srvPtr;
	ID3D11SamplerState* samplerStatePtr;

	// getters
	SimpleVertexShader* GetVertexShader();
	SimplePixelShader* GetPixelShader();

};

