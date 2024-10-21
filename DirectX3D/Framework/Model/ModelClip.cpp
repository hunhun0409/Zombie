#include "Framework.h"

ModelClip::~ModelClip()
{
    for (auto keyFrame : keyFrames)
    {
        delete keyFrame.second;
    }
}

void ModelClip::Init(float playRate)
{
    eventIter = events.begin();
    playTime = 0.0f;
    duration = frameCount / (tickPerSecond * playRate);
}

void ModelClip::Execute()
{
    if (events.empty()) return;
    if (eventIter == events.end()) return;

    float ratio = playTime / duration;

    if (eventIter->first > ratio) return;

    eventIter->second();
    eventIter++;
}

KeyFrame* ModelClip::GetKeyFrame(string boneName)
{
    if (keyFrames.count(boneName) == 0)
        return nullptr;

    return keyFrames[boneName];
}

void ModelClip::SetEvent(Event event, float ratio)
{
    if (events.count(ratio) > 0) return;
    events[ratio] = event;
}

