#ifndef COLLISION_H
#define COLLISION_H

struct RwLine
{
    Vertex start;
    Vertex end;
};

namespace Collision
{
    struct Line;
};
typedef struct SuperSector;

struct BBox
{
    Vertex max;
    Vertex min;

    bool Within_2D(const RwLine& line);
    bool Intersect_2D(const BBox& bbox);
    bool Intersect(const BBox& bbox);
};


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


    Flags& operator+(const Flags& f1, const Flags& f2);

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

    typedef struct CollCache;

    extern CollCache* spine_cache;
    extern CollCache* trigger_cache;

    struct SuperSector;

    struct CollData
    {
        CollCache* cache;
        //4
        bool collided;
        BYTE alg[3];
        //8
        float unk;//distance?
        //C
        Vertex normal;
        //18
        Vertex point;
        //24
        BYTE unk1[0x10];
        //34
        Collision::Flags ignore_1;
        //36
        Collision::Flags ignore_0;
        //38
        Collision::Flags collFlags;
        //3A
        BYTE unk2[0xA];
        //44
        SuperSector* sector;
        //48
        bool trigger;
        BYTE align[3];
        //4C
        DWORD checksum;
        //50
        DWORD index;
        //54
        DWORD unk4;
        //58
        void* pRandomFunction;

        const ::SuperSector* __restrict p_sector;
        void* node_data;//Not sure?

        Surface surface;
        Vertex v0;
        Vertex v1;
        Vertex v2;

        BYTE safe_guard[20];
        //BYTE unk3[200];

        CollData(Collision::Flags ignore0 = Collision::Flags::Skatable, Collision::Flags ignore1 = Collision::Flags::Hollow)
        {
            cache = NULL;
            ignore_1 = ignore1;
            ignore_0 = ignore0;
        }

        CollData(CollCache* _cache, Collision::Flags ignore0 = Collision::Flags::Skatable, Collision::Flags ignore1 = Collision::Flags::Hollow)
        {
            cache = _cache;
            ignore_1 = ignore1;
            ignore_0 = ignore0;
        }
    };

    struct Branch
    {
        WORD axis;
        BYTE leftType;
        BYTE rightType;
        WORD leftIndex;
        WORD rightIndex;
        float max;
        float min;
    };

    struct Leaf
    {
        WORD numFaces;
        WORD idx;
    };
    extern WORD pInterFaces[1024];
    extern DWORD numInterFaces;

    class CollisionPLG
    {
        DWORD bHasTree;
        void* __restrict tree;//points to numLeafs
        DWORD numFaces;
        WORD* __restrict faces;
        DWORD numLeafs;
        Branch* __restrict branches;
        Leaf* __restrict leafs;
        DWORD unk;//Always NULL?

        /*__inline __declspec (noalias) void AddLeaf(const Leaf& leaf)
        {
            debug_print("Adding Leaf %d faces\n", leaf.numFaces);
            DWORD last = leaf.numFaces + leaf.idx;
            for (DWORD idx = leaf.idx; idx < last; idx++)
            {
                pInterFaces[numInterFaces++] = faces[idx];
            }
        }*/


        __inline __declspec (noalias) bool CollideWithLine(const Leaf& leaf, const Vertex& start, const Vertex& dir, const ::SuperSector* const __restrict sector, CollData& data) const;
        __inline __declspec (noalias) bool CollideWithLine(const Vertex& start, const Vertex& dir, const ::SuperSector* const __restrict sector, CollData& data) const;
        __inline __declspec (noalias) bool TraverseBranch(const Branch* const __restrict branch, const RwLine& line, const Vertex& dir, const ::SuperSector* const __restrict sector, CollData& data) const;
        __inline __declspec (noalias) bool FirstCollideWithLine(const Leaf& leaf, const Vertex& start, const Vertex& dir, const ::SuperSector* const __restrict sector, CollData& data) const;
        __inline __declspec (noalias) bool FirstCollideWithLine(const Vertex& start, const Vertex& dir, const ::SuperSector* const __restrict sector, CollData& data) const;
        __inline __declspec (noalias) bool FirstTraverseBranch(const Branch* const __restrict branch, const RwLine& line, const Vertex& dir, const ::SuperSector* const __restrict sector, CollData& data) const;

    public:
        __declspec (noalias) bool CollideWithLine(const RwLine& line, const Vertex& dir, const ::SuperSector* const __restrict sector, CollData& data) const
        {
            numInterFaces = 0;
            bool collided = false;

            if (bHasTree)
            {
                if (branches)
                {
                    debug_print("Branches\n");
                    collided = TraverseBranch(&branches[0], line, dir, sector, data);
                }
                else
                {
                    debug_print("No branches\n");
                    collided = CollideWithLine(line.start, dir, sector, data);
                }
            }
            else
                collided = CollideWithLine(line.start, dir, sector, data);

            if (collided)
                debug_print("collided with supersector %s\n", FindChecksumName(data.checksum, false));
            return collided;
        }
        __declspec (noalias) bool FirstCollideWithLine(const RwLine& line, const Vertex& dir, const ::SuperSector* const __restrict sector, CollData& data) const
        {
            numInterFaces = 0;
            bool collided = false;

            if (bHasTree)
            {
                if (branches)
                {
                    debug_print("Branches\n");
                    collided = TraverseBranch(&branches[0], line, dir, sector, data);
                }
                else
                {
                    debug_print("No branches\n");
                    collided = FirstCollideWithLine(line.start, dir, sector, data);
                }
            }
            else
                collided = FirstCollideWithLine(line.start, dir, sector, data);

            if (collided)
                debug_print("collided with supersector %s\n", FindChecksumName(data.checksum, false));
            return collided;
        }
    };

    typedef class CollCache;
    __declspec (noalias) bool FindNearestCollision(const RwLine& line, CollData& data, bool update_cache = false);
    __declspec (noalias) bool FindFirstCollision(const RwLine& line, CollData& data, bool update_cache = false);

    struct CFeeler : public Line
    {
        CollData cld;

        CFeeler() : cld(Collision::Flags::None, Collision::Flags::Hollow)
        {
            cld.surface.normal.Set();
            cld.surface.point.Set();
        }

        CFeeler(D3DXVECTOR3& const _start, D3DXVECTOR3& const _end, Collision::Flags ignore_1 = Collision::Flags::Hollow, Collision::Flags ignore_0 = Collision::Flags::None) : cld(ignore_0, ignore_1)
        {
            start.Set(_start.x, _start.y, _start.z);
            end.Set(_end.x, _end.y, _end.z);

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
            return FindFirstCollision(*(RwLine*)&start, cld);
            /*typedef bool(__thiscall* const pGetCollision)(CFeeler* pThis);
            return pGetCollision(0x00496EA0)(this);*/
        }
    };

    __inline __declspec (noalias) bool TriangleIntersectingSphere(D3DXVECTOR3 A, D3DXVECTOR3 B, D3DXVECTOR3 C, D3DXVECTOR3 P, float R);
    bool TriangleInsideSphere(D3DXVECTOR3 A, D3DXVECTOR3 B, D3DXVECTOR3 C, D3DXVECTOR3 P, float R);
};
#endif