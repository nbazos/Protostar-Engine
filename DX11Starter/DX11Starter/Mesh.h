#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include <vector>
#include <fstream>
#include "Vertex.h"

// For the DirectX Math library
using namespace DirectX;

class Mesh {
public:
	Mesh(Vertex* vertices, int numVerts, unsigned int* indices, int numInds, ID3D11Device* device);
	Mesh(char* filename, ID3D11Device* device);
	~Mesh();

	ID3D11Buffer* GetVertexBuffer() { return vertexBuffer; };
	ID3D11Buffer* GetIndexBuffer() { return indexBuffer; };
	int GetIndexCount() { return numIndices; };

private:
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;
	int numIndices;

	void CreateBuffers(Vertex* vertices, int numVerts, unsigned int* indices, int numInds, ID3D11Device* device);
};
