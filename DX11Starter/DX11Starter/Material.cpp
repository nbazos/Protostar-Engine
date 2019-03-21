#include "Material.h"

Material::Material(SimpleVertexShader * vShader, SimplePixelShader * pShader, ID3D11ShaderResourceView* srv, ID3D11SamplerState* samplerState)
{
	vertexShader = vShader;
	pixelShader = pShader;
	srvPtr = srv;
	samplerStatePtr = samplerState;
}

Material::Material()
{
	vertexShader = nullptr;
	pixelShader = nullptr;
	srvPtr = nullptr;
	samplerStatePtr = nullptr;
}

Material::~Material()
{
}

SimpleVertexShader * Material::GetVertexShader()
{
	return vertexShader;
}

SimplePixelShader * Material::GetPixelShader()
{
	return pixelShader;
}
