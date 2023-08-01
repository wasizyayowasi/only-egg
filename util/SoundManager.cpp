#include "SoundManager.h"
#include <DxLib.h>
#include<cassert>

namespace {
	constexpr char sound_file_signature[] = "SND_";
	constexpr char sound_confg_file_path[] = "./sound.conf";
	constexpr float sound_config_version = 1.0f;
}

int SoundManager::loadSoundFile(const char* fileName)
{
	std::string path = "data/sound/SE/";
	path += fileName;
	path += ".mp3";
	int handle = LoadSoundMem(path.c_str());
	assert(handle >= 0);
	nameAndHandleTable_[fileName] = handle;
	return handle;
}

SoundManager::SoundManager() {
	loadSoundConfig();
	loadSoundFile("bake");
	loadSoundFile("cure");
	loadSoundFile("interim");
	loadSoundFile("bane");
	loadSoundFile("select");
	loadSoundFile("decision");
	setSEVolume(m_volumeSE);
	setBGMVolume(m_volumeBGM);
}

void SoundManager::loadSoundConfig()
{
	SoundConfigInfo conf = {};
	FILE* fp = nullptr;
	fopen_s(&fp, sound_confg_file_path, "rb");
	if (fp) {
		fread(&conf, sizeof(conf), 1, fp);
		fclose(fp);
		m_volumeBGM = conf.volumeBGM;
		m_volumeSE = conf.volumeSE;
	}
	
}

void SoundManager::saveSoundConfig()
{
	SoundConfigInfo conf = {};
	std::copy_n(std::begin(sound_file_signature),sizeof(char)*4, std::begin(conf.signature));
	conf.version = sound_config_version;
	conf.volumeSE = m_volumeSE;
	conf.volumeBGM = m_volumeBGM;

	FILE* fp = nullptr;
	fopen_s(&fp, sound_confg_file_path, "wb");
	if (fp) {
		fwrite(&conf, sizeof(conf), 1, fp);
		fclose(fp);
	}
}

SoundManager::~SoundManager()
{
}

void SoundManager::play(const char* name)
{
	PlaySoundMem(nameAndHandleTable_[name], DX_PLAYTYPE_BACK);
}

void SoundManager::playMusic(const char* path)
{
	DxLib::PlayMusic(path,DX_PLAYTYPE_LOOP);
	SetVolumeMusic(m_volumeBGM);
}

void SoundManager::setSEVolume(unsigned int volume)
{
	for (auto& record : nameAndHandleTable_) {
		ChangeVolumeSoundMem(volume, record.second);
	}
	m_volumeSE = volume;
}

int SoundManager::getSEVolume() const
{
	return m_volumeSE;
}

void SoundManager::setBGMVolume(unsigned int volume)
{
	SetVolumeMusic(volume);
	m_volumeBGM = volume;
}

void SoundManager::setBGMRate(float rate)
{
	assert(0.0f <= rate && rate <= 1.0f);
	SetVolumeMusic(static_cast<int>(static_cast<float>(m_volumeBGM * rate)));
}

int SoundManager::getBGMVolume() const
{
	return m_volumeBGM;
}

void SoundManager::stopSE(const char* name)
{
	if (name != nullptr) {
		StopSoundMem(nameAndHandleTable_[name]);
	}
}

void SoundManager::stopBGM()
{
	StopMusic();
}
