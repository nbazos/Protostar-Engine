#include "Material.h"

Material::Material(SimpleVertexShader * vShader, SimplePixelShader * pShader, ID3D11ShaderResourceView* srv, ID3D11SamplerState* sampler)
{
	vertexShader = vShader;
	pixelShader = pShader;
	textureSRV = srv;
	samplerState = sampler;
}

Material::~Material() { }


