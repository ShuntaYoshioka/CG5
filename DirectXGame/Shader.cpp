#include "Shader.h"
#include "MiscUtility.h"
#include<d3dcompiler.h>
#include<cassert>

#include <dxcapi.h>
#pragma comment(lib, "dxcompiler.lib")

void Shader::Load(const std::wstring& filePath, const std::wstring& shaderModel) { 
	ID3DBlob* shaderBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;

	std::string mbShaderModel = ConvertString(shaderModel);

	HRESULT hr =D3DCompileFromFile(
			filePath.c_str(), 
			nullptr, 
			D3D_COMPILE_STANDARD_FILE_INCLUDE, 
			"main", mbShaderModel.c_str(),
			D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
			0, &shaderBlob, &errorBlob);
	// エラーが発生したら止める
	if (FAILED(hr)) {
		if (errorBlob) {
			OutputDebugStringA(reinterpret_cast<char*>(errorBlob->GetBufferPointer()));
			errorBlob->Release();
		}
		assert(false);
	}
	// 生成したshaderBlobを返す
	blob_ = shaderBlob;

}

// シェーダーファイルを読み込みコンパイルする
//外部コンパイラ版　シェーダーモデル6.0以上
void Shader::LoadDxc(const std::wstring& filePath, const std::wstring& shaderModel) {
//初期化
	static IDxcUtils* dxcUtils = nullptr;
static IDxcCompiler3* dxcCompiler = nullptr;
	static IDxcIncludeHandler* includeHandler = nullptr;

	HRESULT hr;

	if (dxcUtils == nullptr) {
		hr = DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&dxcUtils));
		assert(SUCCEEDED(hr));
	}
	if (dxcCompiler == nullptr) {
		hr = DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&dxcCompiler));
		assert(SUCCEEDED(hr));
	}
	if (includeHandler == nullptr) {
		hr = dxcUtils->CreateDefaultIncludeHandler(&includeHandler);
		assert(SUCCEEDED(hr));
	}

	// 1.hlslファイルを読み込む
	IDxcBlobEncoding* shaderSource = nullptr;
	hr = dxcUtils->LoadFile(filePath.c_str(), nullptr, &shaderSource);
	assert(SUCCEEDED(hr));

	//読み込んファイルの内容をDxBufferに設定する
	DxcBuffer shaderSourceBuffer;
	shaderSourceBuffer.Ptr = shaderSource->GetBufferPointer();
	shaderSourceBuffer.Size = shaderSource->GetBufferSize();
	shaderSourceBuffer.Encoding = DXC_CP_UTF8;

	//Compileする
	LPCWSTR arguments[] = {
	    filePath.c_str(),
	    L"-E", 
		L"main",             // エントリーポイント名の指定
	    L"-T", 
		shaderModel.c_str(), // シェーダープロファイル指定
	    L"-Zi", 
		L"-Qembed_debug",    // デバッグ用情報を埋め込む
	    L"-Od",                      // 最適化を外す
	    L"-Zpr"           // メモリレイアウトを行優先
	};

	// 実際にShaderをCompileする
	IDxcResult* shaderResult = nullptr;
	hr = dxcCompiler->Compile(
		&shaderSourceBuffer, 
		arguments,
		_countof(arguments), 
		includeHandler, 
		IID_PPV_ARGS(&shaderResult)
	);
	assert(SUCCEEDED(hr));

	//エラーが出てないかの確認
	IDxcBlobUtf8* shaderError = nullptr;
	IDxcBlobWide* nameBlob = nullptr;
	shaderResult->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(&shaderError), &nameBlob);
	if (shaderError != nullptr && shaderError->GetStringLength() != 0) {
		OutputDebugStringA(shaderError->GetStringPointer());
		assert(false);
	}

	IDxcBlob* shaderBlob = nullptr;
	hr = shaderResult->GetOutput(DXC_OUT_OBJECT, IID_PPV_ARGS(&shaderBlob), &nameBlob);
	assert(SUCCEEDED(hr));

	//解放
	shaderSource->Release();
	shaderResult->Release();

	//実行用バイナリ
	dxcBlob_ = shaderBlob;
}

ID3DBlob* Shader::GetBlob() { 
	return blob_; 
}

IDxcBlob* Shader::GetDxcBlob() { 
	return dxcBlob_;
}

// コンストラクタ
Shader::Shader() {}

// デストラクタ
Shader::~Shader() {
	if (blob_ != nullptr) {
		blob_->Release();
		blob_ = nullptr;
	}

	if (dxcBlob_ != nullptr) {
		dxcBlob_->Release();
		dxcBlob_ = nullptr;
	}

}