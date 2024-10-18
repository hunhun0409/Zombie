#pragma once

class TerrainEditor : public GameObject
{
private:
    enum BrushType
    {
        CIRCLE, SQUARE,
    };
    enum BrushHardness
    {
        SOLID, SMOOTH,
    };
    enum EditType
    {
        HEIGHT, ALPHA,
    };

    typedef VertexUVNormalTangentAlpha VertexType;

    const float MAX_HEIGHT = 50;
    const float MIN_HEIGHT = 0;

    const UINT MAX_SIZE = 512;

    class BrushBuffer : public ConstBuffer
    {
    private:
        struct Brush
        {
            int type = 0;
            Float3 pickingPos = { 0,0,0 };

            float range = 1.0f;
            Float3 color = { 0, 1, 0 };
        };
    public:
        BrushBuffer() : ConstBuffer(&brush, sizeof(Brush))
        {
        }

        Brush& Get() { return brush; }

    private:
        Brush brush;
    };

    class RayBuffer : public ConstBuffer
    {
    private:
        struct Data
        {
            Float3 pos;
            UINT triangleSize;

            Float3 dir;
            float padding;
        };
    public:
        RayBuffer() : ConstBuffer(&data, sizeof(Data))
        {
        }

        Data& Get() { return data; }

    private:
        Data data;
    };

    struct InputDesc
    {
        Float3 v0, v1, v2;
    };

    struct OutputDesc
    {
        int picked;
        float distance;
    };

public:
    TerrainEditor();
    ~TerrainEditor();

    void Update();
    void Render() override;
    void GUIRender() override;

private:
    void MakeMesh();
    void MakeNormal();
    void MakeTangent();
    void MakeComputeData();

    Vector3 Picking();
    bool ComputePicking(Vector3& pos);

    void Resize();
    void UpdateHeight();

    void AdjustHeight();
    void AdjustAlpha();

    void SaveHeight(string file);
    void SaveHeightMap();
    void LoadHeight(string file);
    void LoadHeightMap();

    void SaveAlpha(string file);
    void SaveAlphaMap();
    void LoadAlpha(string file);
    void LoadAlphaMap();

    void Save(string file);
    void Load(string file);

    void SaveDialog();
    void LoadDialog();

    void SelectMap(string name, UINT index);
private:
    string name;
    string editName;
    string file;

    UINT uvScale = 1;

    string projectPath;

    UINT width, height;
    UINT triangleSize;

    UINT fixWidth, fixHeight;

    float adjustValue = 10.0f;
    BrushType brushType = CIRCLE;
    BrushHardness brushHardness = SOLID;
    EditType editType = ALPHA;

    UINT selectMap = 0;

    Vector3 pickingPos;

    Mesh<VertexType>* mesh;
    BrushBuffer* brushBuffer;
    RayBuffer* rayBuffer;
    StructuredBuffer* structuredBuffer;

    vector<InputDesc> inputs;
    vector<OutputDesc> outputs;

    Texture* heightMap;

    Texture* maps[4];
    Texture* normalMaps[4];

    string alphaMapFile;
    string heightMapFile;

    ComputeShader* computeShader;
};