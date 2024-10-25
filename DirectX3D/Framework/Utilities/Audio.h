#pragma once

class Vector3;

using namespace FMOD;

class Audio : public Singleton<Audio>
{
private:
    /*friend class Singleton;

    const int MAX_CHANNEL = 50;

    struct SoundInfo
    {
        Sound* sound = nullptr;
        Channel* channel = nullptr;

        ~SoundInfo()
        {
            sound->release();
        }
    };*/

    friend class Singleton;
    const int MAX_CHANNEL = 50;
    const int CHANNELS_PER_SOUND = 50; // 사운드당 채널 수

    struct SoundInfo
    {
        Sound* sound = nullptr;
        vector<Channel*> channels;  // 채널을 벡터로 관리
        int currentChannel = 0;     // 현재 사용할 채널 인덱스

        ~SoundInfo()
        {
            sound->release();
        }
    };

    Audio();
    ~Audio();

public:
    void Update();

    void Add(string key, string file,
        bool bgm = false, bool loop = false, bool is3D = false);

    void Play(string key, float volume = 1.0f);
    void Play(string key, Vector3 position, float volume = 1.0f);
    void PlayRandom(string key, int min, int man, float volume = 1.0f);
    void Stop(string key);
    void Pause(string key);
    void Resume(string key);

    void SetVolume(string key, float volume);

    bool IsPlaySound(string key);

private:
    System* soundSystem;

    unordered_map<string, SoundInfo*> sounds;

    FMOD_VECTOR listenerPos;
};