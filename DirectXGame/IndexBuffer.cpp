#include "IndexBuffer.h"
#include "KamataEngine.h"

#include <d3d12.h>
#include<cassert>

using namespace KamataEngine;

void IndexBuffer::Create(const UINT size, const UINT stride) {
	//strideの値によって1つのインデックスのフォーマットを決める
	assert(stride == 2 || stride == 4);
	DXGI_FORMAT format = (stride == 2) ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT;

	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

		// vertexResourceの作成
	// 頂点リソース用のヒープの設定
	D3D12_HEAP_PROPERTIES uploadHeapProperties{};
	uploadHeapProperties.Type = D3D12_HEAP_TYPE_UPLOAD; // CPUから書き込むヒープ
	// インデックスリソースの設定
	D3D12_RESOURCE_DESC indexResourceDesc{};
	indexResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER; // バッファ
	indexResourceDesc.Width = size;                                // リソースのサイズ		今回はVector4を3頂点分
	// バッファの場合はこれらは1
	indexResourceDesc.Height = 1;
	indexResourceDesc.DepthOrArraySize = 1;
	indexResourceDesc.MipLevels = 1;
	indexResourceDesc.SampleDesc.Count = 1;
	// バッファの場合はこれにする決まり
	indexResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	ID3D12Resource* indexResource = nullptr;

	HRESULT hr =dxCommon->GetDevice()->CreateCommittedResource(
		&uploadHeapProperties, D3D12_HEAP_FLAG_NONE, &indexResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&indexResource));
	assert(SUCCEEDED(hr));

	// indexbufferをとっておく
	indexBuffer_ = indexResource;

	// IndexBufferViewの作成
	D3D12_INDEX_BUFFER_VIEW indexBufferView{};
	// リソースの先頭アドレスから使う
	indexBufferView.BufferLocation = indexResource->GetGPUVirtualAddress();
	// 使用するインデックスデータのサイズ
	indexBufferView.SizeInBytes = size;
	// インデックスのフォーマット
	indexBufferView.Format = format;

	// indexBufferViewをとっておく
	indexBufferView_ = indexBufferView;

}

// ゲッター
ID3D12Resource* IndexBuffer::Get() { 
	return indexBuffer_; 
}

// インデックスバッファビューのゲッター
D3D12_INDEX_BUFFER_VIEW* IndexBuffer::GetView() {
	return &indexBufferView_; 
}

// コンストラクタ
IndexBuffer::IndexBuffer() {}

// デストラクタ
IndexBuffer::~IndexBuffer() {
	if (indexBuffer_) {
		indexBuffer_->Release();
		indexBuffer_ = nullptr;
	}
}
