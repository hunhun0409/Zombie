#pragma once

class ModelClip
{
private:
    friend class ModelAnimator;
    friend class ModelAnimatorInstancing;

    ModelClip() = default;
    ~ModelClip();

    void Init(float playRate);
    void Execute();

    KeyFrame* GetKeyFrame(string boneName);

public:
    void SetEvent(Event event, float ratio);


private:
    string name;
    string lockBone;

    UINT frameCount;
    float tickPerSecond;
    float duration;

    float playTime = 0.0f;
    bool isLoop = false;
    
    vector<Vector3> rootMotionDeltas;
    vector<KeyTransform> rootMotionTransforms;
    unordered_map<string, KeyFrame*> keyFrames;

    map<float, Event> events;
    map<float, Event>::iterator eventIter;
};