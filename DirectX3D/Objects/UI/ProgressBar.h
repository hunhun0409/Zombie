#pragma once

class ProgressBar : public Quad
{
public:
    ProgressBar(wstring frontImageFile, wstring backImageFile);
    ~ProgressBar();

    virtual void Render() override;

    void SetAmount(float value);
private:
    FloatValueBuffer* valueBuffer;
    Texture* backImage;

    float fillAmount = 1.0f;

    
};