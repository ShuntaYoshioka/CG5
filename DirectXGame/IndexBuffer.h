#pragma once

#include <d3d12.h>

class IndexBuffer {
	
public:
	IndexBuffer();
	~IndexBuffer();

	void Create(const UINT size, const UINT stride);
	ID3D12Resource* Get();
	D3D12_INDEX_BUFFER_VIEW* GetView();

private:
	ID3D12Resource* indexBuffer_ = nullptr;
	D3D12_INDEX_BUFFER_VIEW indexBufferView_ = {};

};
