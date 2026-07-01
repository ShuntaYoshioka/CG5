#pragma once

#include <d3d12.h>

class RootSignature {
public:
	void Create();

	ID3D12RootSignature* Get();

	RootSignature();

	~RootSignature();

private:
	ID3D12RootSignature* rootSignature_ = nullptr;
};
