#pragma once

#include <d3d11.h>
#include "Vertex.h"
#include <string>

class Mesh
{
public:
	Mesh(
		Vertex* verticePtr, 
		int verticeCount, 
		unsigned int * indicePtr, 
		int indiceCount, 
		ID3D11Device* devicePtr);
	Mesh(char* fileName, ID3D11Device * devicePtr); // model loading constructor
	~Mesh();

	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;

	int indexBufferCount;

	void CreateBuffers(
		Vertex* verticePtr,
		int verticeCount,
		unsigned int * indicePtr,
		int indiceCount,
		ID3D11Device* devicePtr);

	ID3D11Buffer* GetVertexBuffer();

	ID3D11Buffer* GetIndexBuffer();

	int GetIndexCount();

};

