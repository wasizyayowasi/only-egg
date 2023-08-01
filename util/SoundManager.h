#pragma once

#include <unordered_map>
#include <string>

/// <summary>
/// SE�̊Ǘ����s���V���O���g���N���X
/// </summary>
class SoundManager
{
private:
	//�ύX�����T�E���h�����t�@�C���ɏ�������
	struct SoundConfigInfo {
		char signature[4];//"SND_"
		float version;//1.0
		unsigned short volumeSE;//0�`255
		unsigned short volumeBGM;//0�`255
	};

	int m_volumeSE = 128;
	int m_volumeBGM = 128;

	std::unordered_map<std::string, int>nameAndHandleTable_;
	int loadSoundFile(const char* fileName);

	//�V���O���g���p�^�[���Ȃ̂ŃR���X�g���N�^��private�ɒu��
	SoundManager();

	//�R�s�[��������֎~
	SoundManager(const SoundManager&) = delete;//�R�s�[�R���X�g���N�^
	void operator = (const SoundManager&) = delete;//������֎~

	void loadSoundConfig();
	

public:
	~SoundManager();

	/// <summary>
	/// SoundManager�g�p�҂�GetInstance()��ʂ����Q�Ƃ��炵�����p�ł��Ȃ�
	/// </summary>
	/// <returns></returns>
	static SoundManager& getInstance() {
		static SoundManager instance;//�������B��̎���
		return instance;//����̎Q�Ƃ�Ԃ�
	}

	/// <summary>
	/// �w��̃T�E���h��炷
	/// </summary>
	/// <param name="name"></param>
	void play(const char* name);
	void playMusic(const char* path);

	void setSEVolume(unsigned int volume);
	int  getSEVolume()const ;

	void setBGMVolume(unsigned int volume);
	/// <summary>
	/// BGM�̌��݂̖炷���ʂ̊���
	/// </summary>
	/// <param name="rate">���ʂ̊���(�ꎞ�I�Ȃ���0.0�`1.0)</param>
	void setBGMRate(float rate);
	int  getBGMVolume()const;

	//�T�E���h��S���ꎞ��~����
	void stopSE(const char* name = nullptr);
	void stopBGM();

	//�T�E���h���̃Z�[�u
	void saveSoundConfig();
};

