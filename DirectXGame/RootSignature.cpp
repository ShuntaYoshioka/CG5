#include "RootSignature.h"
#include "KamataEngine.h"

using namespace KamataEngine;

void RootSignature::Create() {

	if (rootSignature_) {
		rootSignature_->Release();
		rootSignature_ = nullptr;
	}

	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};
	descriptionRootSignature.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	ID3DBlob* signatureBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;
	HRESULT hr = D3D12SerializeRootSignature(&descriptionRootSignature, 
		D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob, &errorBlob);
	if (FAILED(hr)) {
		DebugText::GetInstance()->ConsolePrintf(reinterpret_cast<char*>(errorBlob->GetBufferPointer()));
		assert(false);
	}

	ID3D12RootSignature* rootSignature = nullptr;
	hr = dxCommon->GetDevice()->CreateRootSignature(0, signatureBlob->GetBufferPointer(),
		signatureBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignature));
	assert(SUCCEEDED(hr));

	signatureBlob->Release();

	rootSignature_ = rootSignature;
}

ID3D12RootSignature* RootSignature::Get() {
	return rootSignature_; 
}

RootSignature::RootSignature() {
}

RootSignature::~RootSignature() {
	if (rootSignature_) {
		rootSignature_->Release();
		rootSignature_ = nullptr;
	}
}
