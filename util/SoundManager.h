#pragma once

#include <unordered_map>
#include <string>

/// <summary>
/// SEの管理を行うシングルトンクラス
/// </summary>
class SoundManager
{
private:
	//変更したサウンド情報をファイルに書き込む
	struct SoundConfigInfo {
		char signature[4];//"SND_"
		float version;//1.0
		unsigned short volumeSE;//0～255
		unsigned short volumeBGM;//0～255
	};

	int m_volumeSE = 128;
	int m_volumeBGM = 128;

	std::unordered_map<std::string, int>nameAndHandleTable_;
	int loadSoundFile(const char* fileName);

	//シングルトンパターンなのでコンストラクタはprivateに置く
	SoundManager();

	//コピーも代入も禁止
	SoundManager(const SoundManager&) = delete;//コピーコンストラクタ
	void operator = (const SoundManager&) = delete;//代入も禁止

	void loadSoundConfig();
	

public:
	~SoundManager();

	/// <summary>
	/// SoundManager使用者はGetInstance()を通した参照からしか利用できない
	/// </summary>
	/// <returns></returns>
	static SoundManager& getInstance() {
		static SoundManager instance;//ここが唯一の実態
		return instance;//それの参照を返す
	}

	/// <summary>
	/// 指定のサウンドを鳴らす
	/// </summary>
	/// <param name="name"></param>
	void play(const char* name);
	void playMusic(const char* path);

	void setSEVolume(unsigned int volume);
	int  getSEVolume()const ;

	void setBGMVolume(unsigned int volume);
	/// <summary>
	/// BGMの現在の鳴らす音量の割合
	/// </summary>
	/// <param name="rate">音量の割合(一時的なもの0.0～1.0)</param>
	void setBGMRate(float rate);
	int  getBGMVolume()const;

	//サウンドを全部一時停止する
	void stopSE(const char* name = nullptr);
	void stopBGM();

	//サウンド情報のセーブ
	void saveSoundConfig();
};

