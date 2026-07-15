#include "VertexBuffer.h"
#include "KamataEngine.h"

#include<d3d12.h>
#include<cassert>

using namespace KamataEngine;

//生成

void VertexBuffer::Create(const UINT size, const UINT stride) { 
	
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	
	// vertexResourceの作成
	// 頂点リソース用のヒープの設定
	D3D12_HEAP_PROPERTIES uploadHeapProperties{};
	uploadHeapProperties.Type = D3D12_HEAP_TYPE_UPLOAD; // CPUから書き込むヒープ
	// 頂点リソースの設定
	D3D12_RESOURCE_DESC vertexResourceDesc{};
	vertexResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER; // バッファ
	vertexResourceDesc.Width = size;                 // リソースのサイズ		今回はVector4を3頂点分
	// バッファの場合はこれらは1
	vertexResourceDesc.Height = 1;
	vertexResourceDesc.DepthOrArraySize = 1;
	vertexResourceDesc.MipLevels = 1;
	vertexResourceDesc.SampleDesc.Count = 1;
	// バッファの場合はこれにする決まり
	vertexResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	// 実際にリソースを生成する
	ID3D12Resource* vertexResource = nullptr;
	[[maybe_unused]] HRESULT hr =
	    dxCommon->GetDevice()->CreateCommittedResource(&uploadHeapProperties, D3D12_HEAP_FLAG_NONE, &vertexResourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&vertexResource));
	assert(SUCCEEDED(hr));

	//vertexbufferをとっておく
	vertexBuffer_ = vertexResource;

		// VertexBufferViewの作成
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};
	// リソースの先頭アドレスから使う
	vertexBufferView.BufferLocation = vertexResource->GetGPUVirtualAddress();
	// 使用するリソースのサイズは頂点size分のサイズ
	vertexBufferView.SizeInBytes =size;
	// 1つの頂点のサイズ
	vertexBufferView.StrideInBytes = stride;

	// vertexBufferViewをとっておく
	vertexBufferView_ = vertexBufferView;

}

// ゲッター
ID3D12Resource* VertexBuffer::Get() { 
	return vertexBuffer_;
}

D3D12_VERTEX_BUFFER_VIEW* VertexBuffer::GetView() { 
	return &vertexBufferView_;
}

// コンストラクタ
VertexBuffer::VertexBuffer() {}

// デストラクタ
VertexBuffer::~VertexBuffer() {
	if (vertexBuffer_) {
		vertexBuffer_->Release();
		vertexBuffer_ = nullptr;
	}
}