#pragma once

#include <string>
#include <d3d12.h>

class Shader {
public:
	// シェーダーファイルを読み込み、コンパイルする関数
	void Load(const std::wstring& filePath, const std::string& shaderModel);

	// 生成したコンパイル済みデータを取得する
	ID3DBlob* GetBlob();

	//コンストラクタ
	Shader();
	// デストラクタ
	~Shader();

	private:
	ID3DBlob* blob_ = nullptr;

};
