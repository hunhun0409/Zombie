#pragma once

class Quad : public GameObject
{
private:
    typedef VertexUV VertexType;

public:
    Quad(Vector2 size);    
    Quad(wstring file);
    ~Quad();

    void Render();
    virtual void SetRender() override;

private:
    void MakeMesh();

private:
    Vector2 size;

    Mesh<VertexType>* mesh;    
};