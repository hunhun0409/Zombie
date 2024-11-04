#include "Framework.h"

ModelAnimatorInstancing::ModelAnimatorInstancing(string name)
    :ModelAnimator(name)
{
    SetShader(L"Model/ModelAnimationInstancing.hlsl");
    instanceBuffer = new VertexBuffer(instanceDatas, sizeof(InstanceData), MAX_INSTANCE);
    frameInstancingBuffer = new FrameInstancingBuffer();
}

ModelAnimatorInstancing::~ModelAnimatorInstancing()
{
    delete instanceBuffer;
    delete frameInstancingBuffer;

    for (Transform* transform : transforms)
        delete transform;
}

void ModelAnimatorInstancing::Update()
{
    UpdateTransforms();
    if (KEY_DOWN(VK_F8))
    {
        isInstance = !isInstance;
    }
}

void ModelAnimatorInstancing::Render()
{
    if (texture == nullptr)
        CreateTexture();

    instanceBuffer->Set(1);

    frameInstancingBuffer->SetVS(4);
    DC->VSSetShaderResources(0, 1, &srv);

    for (ModelMesh* mesh : meshes)
    {
        mesh->RenderInstanced(drawCount);
    }
}

void ModelAnimatorInstancing::GUIRender()
{
    ImGui::Text("DrawCount : %d", drawCount);

    for (Transform* transform : transforms)
    {
        transform->GUIRender();
    }
}

Transform* ModelAnimatorInstancing::Add()
{
    Transform* transform = new Transform();
    transform->SetTag(name + "_" + to_string(transforms.size()));
    transforms.push_back(transform);

    return transform;
}

void ModelAnimatorInstancing::PlayClip(UINT instanceID, int clip, float playRate, float takeTime)
{
    frameInstancingBuffer->Get().motions[instanceID].next.clip = clip;
    frameInstancingBuffer->Get().motions[instanceID].next.playRate = playRate;
    frameInstancingBuffer->Get().motions[instanceID].takeTime = takeTime;
    frameInstancingBuffer->Get().motions[instanceID].runningTime = 0.0f;

    ModelClip* modelClip = clips[frameInstancingBuffer->Get().motions[instanceID].next.clip];

    frameInstancingBuffer->Get().motions[instanceID].duration = modelClip->frameCount /
        (modelClip->tickPerSecond * frameInstancingBuffer->Get().motions[instanceID].cur.playRate);

}

Matrix ModelAnimatorInstancing::GetTransformByNode(UINT instanceID, int nodeIndex)
{
    if (texture == nullptr) return XMMatrixIdentity();

    Matrix curAnim;

    {//CurAnim
        Frame& curFrame = frameInstancingBuffer->Get().motions[instanceID].cur;

        Matrix cur = nodeTransforms[curFrame.clip].transform[curFrame.curFrame][nodeIndex];
        Matrix next = nodeTransforms[curFrame.clip].transform[curFrame.curFrame + 1][nodeIndex];

        curAnim = Lerp(cur, next, curFrame.time);
    }

    {//NextAnim
        Frame& nextFrame = frameInstancingBuffer->Get().motions[instanceID].next;

        if (nextFrame.clip == -1)
        {
            return curAnim * transforms[instanceID]->GetWorld();
            //return curAnim * instanceDatas[instanceID].world;

        }

        Matrix cur = nodeTransforms[nextFrame.clip].transform[nextFrame.curFrame][nodeIndex];
        Matrix next = nodeTransforms[nextFrame.clip].transform[nextFrame.curFrame + 1][nodeIndex];

        Matrix nextAnim = Lerp(cur, next, nextFrame.time);

        float tweenTime = frameInstancingBuffer->Get().motions[instanceID].tweenTime;

        return Lerp(curAnim, nextAnim, tweenTime) * transforms[instanceID]->GetWorld();
    }

    return Matrix();
}


void ModelAnimatorInstancing::UpdateFrame(UINT instanceID, Motion& motion)
{
    // 현재 클립 실행
    {
        ModelClip* clip = clips[motion.cur.clip];

        //motion.duration = clip->frameCount / (clip->tickPerSecond * motion.cur.playRate);

        motion.runningTime += motion.cur.playRate * DELTA;
        motion.cur.time += clip->tickPerSecond * motion.cur.playRate * DELTA;

        if (motion.cur.time >= 1.0f)
        {
            if (clip->isLoop)
            {
                motion.cur.curFrame++;

                motion.cur.curFrame %= (clip->frameCount - 1);
                motion.cur.time -= 1.0f;
            }
            else
            {
                if (motion.cur.curFrame == clip->frameCount - 1)
                {

                    motion.cur.time -= 1.0f;
                }
                else
                {
                    motion.cur.curFrame++;

                    motion.cur.curFrame = min(motion.cur.curFrame, clip->frameCount - 2);
                    motion.cur.time -= 1.0f;
                }

            }
        }
    }

    // 다음 클립 처리 (보간)
    {
        if (motion.next.clip < 0) return;

        ModelClip* clip = clips[motion.next.clip];

        motion.tweenTime += DELTA / motion.takeTime;
        if (motion.tweenTime >= 1.0f)
        {
            motion.cur = motion.next;
            motion.tweenTime = 0.0f;

            motion.next.clip = -1;
            motion.next.curFrame = 0;
            motion.next.time = 0.0f;
            return;
        }

        motion.next.time += clip->tickPerSecond * motion.next.playRate * DELTA;

        if (motion.next.time >= 1.0f)
        {
            motion.next.curFrame = (motion.next.curFrame + 1) % (clip->frameCount - 1);
            motion.next.time -= 1.0f;
        }
    }
}

void ModelAnimatorInstancing::UpdateTransforms()
{
    drawCount = 0;

    FOR(transforms.size())
    {
        if (transforms[i]->Active())
        {
            //if (!CAM->ContainPoint(transforms[i]->GlobalPos())) continue;

            UpdateFrame(i, frameInstancingBuffer->Get().motions[i]);
            transforms[i]->UpdateWorld();

            instanceDatas[drawCount].world = XMMatrixTranspose(transforms[i]->GetWorld());
            instanceDatas[drawCount].index = i;
            drawCount++;
        }
    }

    instanceBuffer->Update(instanceDatas, drawCount);
}
