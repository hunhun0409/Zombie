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

    sounds[key] = info;
}

void Audio::Play(string key, float volume)
{
    if (sounds.count(key) == 0) return;

    soundSystem->playSound(sounds[key]->sound,
        nullptr, false, &sounds[key]->channel);
    sounds[key]->channel->setVolume(volume);

}

void Audio::Play(string key, Vector3 position, float volume)
{
    if (sounds.count(key) == 0) return;

    soundSystem->playSound(sounds[key]->sound,
        nullptr, false, &sounds[key]->channel);

    sounds[key]->channel->setVolume(volume);
    FMOD_VECTOR pos = { position.x, position.y, position.z };
    FMOD_VECTOR vel = {};
    sounds[key]->channel->set3DAttributes(&pos, &vel);
    sounds[key]->channel->set3DMinMaxDistance(1.0f, 10000.0f);
    
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

    sounds[key]->channel->stop();
}

void Audio::Pause(string key)
{
    if (sounds.count(key) == 0) return;

    sounds[key]->channel->setPaused(true);
}

void Audio::Resume(string key)
{
    if (sounds.count(key) == 0) return;

    sounds[key]->channel->setPaused(false);
}

void Audio::SetVolume(string key, float volume)
{
    if (sounds.count(key) == 0) return;

    sounds[key]->channel->setVolume(volume);
}

bool Audio::IsPlaySound(string key)
{
    if (sounds.count(key) == 0) return false;

    bool isPlay = false;
    sounds[key]->channel->isPlaying(&isPlay);

    return isPlay;
}
