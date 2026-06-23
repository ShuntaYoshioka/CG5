#pragma once

#include <string>
#include <d3d12.h>

#include <d3dcompiler.h>
#include <dxcapi.h>

class Shader {
public:
	// シェーダーファイルを読み込み、コンパイルする関数
	void Load(const std::wstring& filePath, const std::wstring& shaderModel);
	void LoadDxc(const std::wstring& filePath, const std::wstring& shaderModel);

	// 生成したコンパイル済みデータを取得する
	ID3DBlob* GetBlob();
	IDxcBlob* GetDxcBlob();

	//コンストラクタ
	Shader();
	// デストラクタ
	~Shader();

	private:
	ID3DBlob* blob_ = nullptr; // コンパイル済みデータ

	IDxcBlob* dxcBlob_ = nullptr; // コンパイル済みデータ 外部
};
