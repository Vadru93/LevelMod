#ifndef COLLISION_H
#define COLLISION_H
namespace Collision
{

    //Collision chache: 0085A4C4
    enum
    {
        IGNORE0 = true,
    };
    enum class Flags : WORD
    {
        None = 0,
        Wallride = 0x0004,
        Vert = 0x0008,
        Hollow = 0x0010,
        Trigger = 0x0040,
        Skatable = 0x0080,
    };

    struct Line
    {
        Vector start;
        Vector end;

        Line()
        {
            start.Set();
            end.Set();
        }
    };

    struct Surface
    {
        Vector normal;
        Vector point;
    };

    struct SuperSector;

    struct CollData
    {
        void* cache;
        //4
        Vertex normal;
        //10
        DWORD padding;
        //14
        float unk;
        //18
        Vertex point;
        //24
        int index;//Or terrain?
        //28
        //Vertex* vertices[3];
        BYTE unk1[0x20];
        bool collided;
        DWORD checksum;

        Collision::Flags ignore_1;
        Collision::Flags ignore_0;

        DWORD unk_flags;//00 01 00 00
        DWORD terrain;//Maybe?
        void* pRandomFunclocation;
        SuperSector* p_sector;
        void* node_data;//Not sure?
        DWORD unk2[4];
        float unk3[2];

        Surface surface;

        BYTE safe_guard[200];
        //BYTE unk3[200];
    };

    struct CFeeler : public Line
    {
        CollData cld;

        CFeeler()
        {
            cld.ignore_1 = Collision::Flags::Hollow;
            cld.ignore_0 = Collision::Flags::None;

            cld.surface.normal.Set();
            cld.surface.point.Set();
        }

        CFeeler(D3DXVECTOR3& const _start, D3DXVECTOR3& const _end, Collision::Flags ignore_1 = Collision::Flags::Hollow, Collision::Flags ignore_0 = Collision::Flags::None)
        {
            start.Set(_start.x, _start.y, _start.z);
            end.Set(_end.x, _end.y, _end.z);

            cld.ignore_1 = ignore_1;
            cld.ignore_0 = ignore_0;

            cld.surface.normal.Set();
            cld.surface.point.Set();
        }

        void FlipDirection()
        {
            D3DXVECTOR3 temp = *(D3DXVECTOR3*)&start;
            *(D3DXVECTOR3*)&start = *(D3DXVECTOR3*)&end;
            *(D3DXVECTOR3*)&end = temp;
        }

        void SetIgnore(Collision::Flags ignore_1 = Collision::Flags::Hollow, Collision::Flags ignore_0 = Collision::Flags::None)
        {
            cld.ignore_1 = ignore_1;
            cld.ignore_0 = ignore_0;
        }

        void SetLine(Vertex& _start, Vertex& _end)
        {
            *(Vertex*)&start = _start;
            *(Vertex*)&end = _end;
        }

        bool GetCollision()
        {
            typedef bool(__thiscall* const pGetCollision)(CFeeler* pThis);
            return pGetCollision(0x00496EA0)(this);
        }
    };

    bool TriangleIntersectingSphere(D3DXVECTOR3 A, D3DXVECTOR3 B, D3DXVECTOR3 C, D3DXVECTOR3 P, float R);
    bool TriangleInsideSphere(D3DXVECTOR3 A, D3DXVECTOR3 B, D3DXVECTOR3 C, D3DXVECTOR3 P, float R);
};
#endif