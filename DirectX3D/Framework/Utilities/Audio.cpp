#include "Framework.h"

Audio::Audio()
{
    System_Create(&soundSystem);
    soundSystem->init(MAX_CHANNEL, FMOD_INIT_NORMAL, nullptr);

    soundSystem->set3DSettings(1.0f, 100.0f, 0.01f);
}

Audio::~Audio()
{
    for (pair<string, SoundInfo*> sound : sounds)
        delete sound.second;

    soundSystem->release();
}

void Audio::Update()
{
    listenerPos = { CAM->Pos().x, CAM->Pos().y, CAM->Pos().z };
    soundSystem->set3DListenerAttributes(0, &listenerPos, nullptr, nullptr, nullptr);

    soundSystem->update();
}

void Audio::Add(string key, string file, bool bgm, bool loop, bool is3D)
{
    if (sounds.count(key) > 0) return;
    SoundInfo* info = new SoundInfo();

    // 기존 사운드 생성 코드는 동일
    if (bgm)
    {
        soundSystem->createStream(file.c_str(),
            FMOD_LOOP_NORMAL, nullptr, &info->sound);
    }
    else
    {
        if (is3D)
        {
            if (loop)
            {
                soundSystem->createSound(file.c_str(),
                    FMOD_3D | FMOD_LOOP_NORMAL, nullptr, &info->sound);
            }
            else
            {
                soundSystem->createSound(file.c_str(),
                    FMOD_3D | FMOD_DEFAULT, nullptr, &info->sound);
            }
        }
        else
        {
            if (loop)
            {
                soundSystem->createSound(file.c_str(),
                    FMOD_LOOP_NORMAL, nullptr, &info->sound);
            }
            else
            {
                soundSystem->createSound(file.c_str(),
                    FMOD_DEFAULT, nullptr, &info->sound);
            }
        }
    }
    info->channels.resize(CHANNELS_PER_SOUND, nullptr);
    sounds[key] = info;
}

void Audio::Play(string key, float volume)
{
    if (sounds.count(key) == 0) return;

    SoundInfo* info = sounds[key];

    // 다음 사용 가능한 채널 찾기
    bool isPlaying = false;
    int channelIndex = info->currentChannel;

    // 현재 채널이 재생 중인지 확인
    if (info->channels[channelIndex] != nullptr)
    {
        info->channels[channelIndex]->isPlaying(&isPlaying);
    }

    // 재생 중이 아닌 채널 찾기
    if (isPlaying)
    {
        for (int i = 0; i < CHANNELS_PER_SOUND; i++)
        {
            if (info->channels[i] == nullptr)
            {
                channelIndex = i;
                isPlaying = false;
                break;
            }
            else
            {
                info->channels[i]->isPlaying(&isPlaying);
                if (!isPlaying)
                {
                    channelIndex = i;
                    break;
                }
            }
        }
    }

    // 모든 채널이 사용 중이면 다음 채널을 강제로 사용
    if (isPlaying)
    {
        channelIndex = (info->currentChannel + 1) % CHANNELS_PER_SOUND;
    }

    // 새 채널에서 사운드 재생
    soundSystem->playSound(info->sound, nullptr, false, &info->channels[channelIndex]);
    info->channels[channelIndex]->setVolume(volume);

    // 다음에 사용할 채널 인덱스 업데이트
    info->currentChannel = (channelIndex + 1) % CHANNELS_PER_SOUND;

}

void Audio::Play(string key, Vector3 position, float volume)
{
    if (sounds.count(key) == 0) return;

    SoundInfo* info = sounds[key];

    // 다음 사용 가능한 채널 찾기 (위와 동일한 로직)
    bool isPlaying = false;
    int channelIndex = info->currentChannel;

    if (info->channels[channelIndex] != nullptr)
    {
        info->channels[channelIndex]->isPlaying(&isPlaying);
    }

    if (isPlaying)
    {
        for (int i = 0; i < CHANNELS_PER_SOUND; i++)
        {
            if (info->channels[i] == nullptr)
            {
                channelIndex = i;
                isPlaying = false;
                break;
            }
            else
            {
                info->channels[i]->isPlaying(&isPlaying);
                if (!isPlaying)
                {
                    channelIndex = i;
                    break;
                }
            }
        }
    }

    if (isPlaying)
    {
        channelIndex = (info->currentChannel + 1) % CHANNELS_PER_SOUND;
    }

    // 새 채널에서 사운드 재생
    soundSystem->playSound(info->sound, nullptr, false, &info->channels[channelIndex]);
    info->channels[channelIndex]->setVolume(volume);

    FMOD_VECTOR pos = { position.x, position.y, position.z };
    FMOD_VECTOR vel = {};
    info->channels[channelIndex]->set3DAttributes(&pos, &vel);
    info->channels[channelIndex]->set3DMinMaxDistance(1.0f, 10000.0f);

    info->currentChannel = (channelIndex + 1) % CHANNELS_PER_SOUND;
}

void Audio::PlayRandom(string key, int min, int max, float volume)
{
    string s;
    UINT idx = Random(min, max);;

    s = to_string(idx);
    key = key + s;
    Play(key, volume);
}

void Audio::Stop(string key)
{
    if (sounds.count(key) == 0) return;

    // 모든 채널 정지
    for (Channel* channel : sounds[key]->channels)
    {
        if (channel != nullptr)
            channel->stop();
    }
}

void Audio::Pause(string key)
{
    if (sounds.count(key) == 0) return;

    // 모든 채널 일시정지
    for (Channel* channel : sounds[key]->channels)
    {
        if (channel != nullptr)
            channel->setPaused(true);
    }
}

void Audio::Resume(string key)
{
    if (sounds.count(key) == 0) return;

    // 모든 채널 재개
    for (Channel* channel : sounds[key]->channels)
    {
        if (channel != nullptr)
            channel->setPaused(false);
    }
}

void Audio::SetVolume(string key, float volume)
{
    if (sounds.count(key) == 0) return;

    // 모든 채널의 볼륨 설정
    for (Channel* channel : sounds[key]->channels)
    {
        if (channel != nullptr)
            channel->setVolume(volume);
    }
}

bool Audio::IsPlaySound(string key)
{
    if (sounds.count(key) == 0) return false;

    // 하나라도 재생 중이면 true 반환
    bool isPlay = false;
    for (Channel* channel : sounds[key]->channels)
    {
        if (channel != nullptr)
        {
            channel->isPlaying(&isPlay);
            if (isPlay) return true;
        }
    }
    return false;
}
