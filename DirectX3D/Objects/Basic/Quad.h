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

public:
    Vector2 size;

protected:
    Mesh<VertexType>* mesh;    
};