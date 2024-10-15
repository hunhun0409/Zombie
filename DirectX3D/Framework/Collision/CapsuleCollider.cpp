#include "Framework.h"


CapsuleCollider::CapsuleCollider(float radius, float height, int stackCount, int sliceCount)
    :radius(radius), height(height), stackCount(stackCount), sliceCount(sliceCount)
{
    type = Type::CAPSULE;

    MakeMesh();
    
    ColliderManager::Get()->Add(this);

}

bool CapsuleCollider::IsRayCollision(IN Ray ray, OUT Contact* contact)
{
    UpdateWorld();

    Vector3 direction = Up();
    Vector3 pa = GlobalPos() - direction * Height() * 0.5f;
    Vector3 pb = GlobalPos() + direction * Height() * 0.5f;

    float r = Radius();

    //ray origin
    Vector3 ro = ray.pos;
    Vector3 rd = ray.dir;

    Vector3 ba = pb - pa;
    Vector3 oa = ro - pa;

    float baba = Dot(ba, ba);
    float bard = Dot(ba, rd);
    float baoa = Dot(ba, oa);
    float rdoa = Dot(rd, oa);
    float oaoa = Dot(oa, oa);

    float a = baba - bard * bard;
    float b = baba * rdoa - baoa * bard;
    float c = baba * oaoa - baoa * baoa - r * r * baba;

    float h = b * b - a * c;

    if (h >= 0.0f)
    {
        float t = (-b - sqrt(h)) / a;

        float distance = baoa + t * bard;

        if (distance > 0.0f && distance < baba)
        {
            if (contact)
            {
                contact->distance = distance;
                contact->hitPoint = ray.pos + ray.dir * t;
            }
            return true;

        }

        Vector3 oc = (distance < -0.0f) ? oa : ro - pb;
        b = Dot(rd, oc);
        c = Dot(oc, oc) - r * r;
        h = b * b - c;

        if (h > 0.0f)
        {
            if (contact)
            {
                contact->distance = distance;
                contact->hitPoint = ray.pos + ray.dir * t;
            }
            return true;
        }
    }


    return false;

}

bool CapsuleCollider::IsBoxCollision(BoxCollider* collider)
{
    BoxCollider::ObbDesc box;
    collider->GetObb(box);

    Vector3 direction = Up();
    Vector3 pa = GlobalPos() - direction * Height() * 0.5f;
    Vector3 pb = GlobalPos() + direction * Height() * 0.5f;

    Vector3 closestPointToSphere = box.pos;
    Vector3 pointOnLine = ClosestPointOnLine(pa, pb, box.pos);

    for (UINT i = 0; i < 3; i++)
    {
        Vector3 direction = pointOnLine - box.pos;

        float length = Dot(box.axis[i], direction);

        float mult = (length < 0.0f) ? -1.0f : 1.0f;

        length = min(abs(length), box.halfSize[i]);

        closestPointToSphere += box.axis[i] * length * mult;
    }

    pointOnLine = ClosestPointOnLine(pa, pb, closestPointToSphere);

    float distance = Distance(pointOnLine, closestPointToSphere);

    return distance <= Radius();
}

bool CapsuleCollider::IsSphereCollision(SphereCollider* collider)
{
    Vector3 direction = Up();
    Vector3 pa = GlobalPos() - direction * Height() * 0.5f;
    Vector3 pb = GlobalPos() + direction * Height() * 0.5f;

    Vector3 P = collider->GlobalPos();

    Vector3 pointOnLine = ClosestPointOnLine(pa, pb, P);

    float distance = Distance(P, pointOnLine);

    return distance <= (Radius() + collider->Radius());

}

bool CapsuleCollider::IsCapsuleCollision(CapsuleCollider* collider)
{
    bool isCollisionU = false;
    bool isCollisionD = false;

    {
        Vector3 aDirection = Up();
        Vector3 aA = GlobalPos() - aDirection * Height() * 0.5f;
        Vector3 aB = GlobalPos() + aDirection * Height() * 0.5f;

        Vector3 bDirection = collider->Up();
        Vector3 bA = collider->GlobalPos() - bDirection * collider->Height() * 0.5f;
        Vector3 bB = collider->GlobalPos() + bDirection * collider->Height() * 0.5f;

        Vector3 v0 = bA - aA;
        Vector3 v1 = bB - aA;
        Vector3 v2 = bA - aB;
        Vector3 v3 = bB - aB;

        float d0 = Dot(v0, v0);
        float d1 = Dot(v1, v1);
        float d2 = Dot(v2, v2);
        float d3 = Dot(v3, v3);

        Vector3 bestA;
        if (d2 < d0 || d2 < d1 || d3 < d0 || d3 > d1)
            bestA = aB;
        else
            bestA = aA;

        Vector3 bestB = ClosestPointOnLine(bA, bB, bestA);
        bestA = ClosestPointOnLine(aA, aB, bestB);
        bestB = ClosestPointOnLine(bA, bB, bestA);

        float distance = Distance(bestA, bestB);

        isCollisionU = distance <= (Radius() + collider->Radius());

    }

    {
        Vector3 aDirection = Down();
        Vector3 aA = GlobalPos() - aDirection * Height() * 0.5f;
        Vector3 aB = GlobalPos() + aDirection * Height() * 0.5f;

        Vector3 bDirection = collider->Up();
        Vector3 bA = collider->GlobalPos() - bDirection * collider->Height() * 0.5f;
        Vector3 bB = collider->GlobalPos() + bDirection * collider->Height() * 0.5f;

        Vector3 v0 = bA - aA;
        Vector3 v1 = bB - aA;
        Vector3 v2 = bA - aB;
        Vector3 v3 = bB - aB;

        float d0 = Dot(v0, v0);
        float d1 = Dot(v1, v1);
        float d2 = Dot(v2, v2);
        float d3 = Dot(v3, v3);

        Vector3 bestA;
        if (d2 < d0 || d2 < d1 || d3 < d0 || d3 > d1)
            bestA = aB;
        else
            bestA = aA;

        Vector3 bestB = ClosestPointOnLine(bA, bB, bestA);
        bestA = ClosestPointOnLine(aA, aB, bestB);
        bestB = ClosestPointOnLine(bA, bB, bestA);

        float distance = Distance(bestA, bestB);

        isCollisionD = distance <= (Radius() + collider->Radius());

    }

    return isCollisionU || isCollisionD;
}

void CapsuleCollider::Update()
{
    if (isChanged)
    {
        MakeMesh();
        isChanged = false;
    }
    Transform::UpdateWorld();
}

void CapsuleCollider::GUIRender()
{
    ImGui::PushID(this);

    ImGui::Text(tag.c_str());
    
    isChanged = false;
    isChanged |= ImGui::DragFloat("Height", &height, 0.1f, 100.0f);
    isChanged |= ImGui::DragFloat("Radius", &radius, 0.1f, 100.0f);

    if (ImGui::Button("Save"))
        Save();

    ImGui::SameLine();

    if (ImGui::Button("Load"))
        Load();
    Transform::GUIRender();

    ImGui::PopID();
}

void CapsuleCollider::Save()
{
    BinaryWriter* writer = new BinaryWriter("TextData/Transforms/" + tag + ".capsule");

    writer->Float(radius);
    writer->Float(height);

    delete writer;
}

void CapsuleCollider::Load()
{
    Transform::Load();

    BinaryReader* reader = new BinaryReader("TextData/Transforms/" + tag + ".capsule");

    radius = reader->Float();
    height = reader->Float();
    isChanged = true;

    delete reader;
}

AABB CapsuleCollider::GetAABB()
{
    Vector3 center = GlobalPos();
    float scaledRadius = Radius();  // This already considers global scale
    float scaledHeight = Height();  // This already considers global scale
    float halfHeight = scaledHeight * 0.5f;

    // Calculate the extent of the capsule
    Vector3 extent(scaledRadius, halfHeight + scaledRadius, scaledRadius);

    // Get the capsule's up vector (assuming Y is up)
    Vector3 up = Up() * halfHeight;

    // Calculate min and max points
    Vector3 minPoint = Vector3::Min(center - up - extent, center + up - extent);
    Vector3 maxPoint = Vector3::Max(center - up + extent, center + up + extent);

    return AABB(minPoint, maxPoint);
}

void CapsuleCollider::MakeMesh()
{
    vector<Vertex>& vertices = mesh->Vertices();

    float phiStep = XM_PI / stackCount;
    float thetaStep = XM_2PI / sliceCount;
    float halfHeight = height * 0.5f;

    vertices.clear();
    vertices.reserve((sliceCount + 1) * (stackCount + 2));

    for (int i = 0; i <= stackCount / 2; ++i)
    {
        float phi = i * phiStep;

        for (int j = 0; j <= sliceCount; ++j)
        {
            Vertex vertex;
            float theta = j * thetaStep;

            float x = radius * sinf(phi) * cosf(theta);
            float y = radius * cosf(phi) + halfHeight;
            float z = radius * sinf(phi) * sinf(theta);

            vertex.pos = Vector3(x, y, z);

            vertices.push_back(vertex);
        }
    }

    for (int i = stackCount / 2; i >= 0; --i)
    {
        float phi = i * phiStep;

        for (int j = 0; j <= sliceCount; ++j)
        {
            Vertex vertex;
            float theta = j * thetaStep;

            float x = radius * sinf(phi) * cosf(theta);
            float y = -radius * cosf(phi) - halfHeight;
            float z = radius * sinf(phi) * sinf(theta);

            vertex.pos = Vector3(x, y, z);


            vertices.push_back(vertex);
        }
    }

    vector<UINT>& indices = mesh->Indices();
    indices.clear();

    int quarter = sliceCount / 4;
    for (int y = 0; y <= stackCount; y++)
    {
        for (int x = 0; x < sliceCount; x++)
        {
            if (x % quarter == 0)
            {
                indices.push_back((sliceCount + 1) * y + x);
                indices.push_back((sliceCount + 1) * (y + 1) + x);
            }

            if (y == stackCount / 2 || y == stackCount / 2 + 1)
            {
                indices.push_back((sliceCount + 1) * y + x);
                indices.push_back((sliceCount + 1) * y + x + 1);
            }

        }
    }

    mesh->CreateMesh();
}
