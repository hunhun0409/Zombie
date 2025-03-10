#pragma once

class Observer : public Singleton<Observer>
{
private:
    friend class Singleton;

    Observer() = default;
    ~Observer() = default;

public:
    void AddEvent(string key, Event event);
    void AddParamEvent(string key, ParamEvent paramEvent);
    void AddIntParamEvent(string key, IntParamEvent paramEvent);
    void AddFloatParamEvent(string key, FloatParamEvent paramEvent);

    void ExcuteEvent(string key);
    void ExcuteParamEvent(string key, void* object);
    void ExcuteIntParamEvent(string key, int value);
    void ExcuteFloatParamEvent(string key, float value);

private:
    unordered_map<string, vector<Event>> totalEvent;
    unordered_map<string, vector<ParamEvent>> totalParamEvent;
    unordered_map<string, vector<IntParamEvent>> totalIntParamEvent;
    unordered_map<string, vector<FloatParamEvent>> totalFloatParamEvent;
};