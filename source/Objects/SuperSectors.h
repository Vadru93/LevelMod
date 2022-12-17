#pragma once
#ifndef SUPERSECTOR_H
#define SUPERSECTOR_H
#undef NO_DEFINES
#include "Script\Checksum.h"
#include "Objects\Mesh.h"
#include "Script\Node.h"
#include "RenderWare\RenderWare.h"


//Collision stuff 00501CE0
struct MovingObject;
namespace Collision
{
    struct CollisionPLG;
    struct Leaf;
    struct CollData;
};
extern void RemoveMovingObject(SuperSector* sector);
extern bool updatingObjects;
//00491820
//0051E060
//#pragma pack(1)
EXTERN struct SuperSector
{
    //determine if the object has any trigger polys, wallride polys, vert polys, shadow polys or is completly non collideable
    enum Flags : WORD
    {
        Default = 0xFFFF,
        NoCollision = 0,
        Wallride = 0x0004,
        Vert = 0x0008,
        Hollow = 0x0010,
        Trigger = 0x0040,
        Skatable = 0x0080,
        Collideable = 0x008C,//maybe use 0xBF??
    };
    union
    {
        WORD req_flags;;
        int FFFFFFFF;//Always -1 maybe morph target index?
    };
    //4
    RpTriangle* triangles;
    //8
    D3DXVECTOR3* vertices;
    //14
    D3DXVECTOR3* normals;
    //20
    float* uv_offset;
    //24
    float* uv_offset2;
    //28
    DWORD* color_offset;
    //2C
    Mesh* mesh;
    //30
    /* Atomics in this sectors */
        /* The pointers are frigged so they look like they are pointing to
           Atomics when they are pointing to here */
    DWORD* pUnk[6];//Here should be pointer to atomic data, but since bsp is made in a bad way it only points to itself
    //48
    D3DXVECTOR3 bboxMax;
    //4C
    D3DXVECTOR3 bboxMin;
    //50
    void* pUnk1;//Always NULL?
    //54
    CSector* sector;
    //58
    void* pipeline;//Always NULL?
    //5C
    WORD flags;//Should be matbaseindex, but now it's flag
    //5E
    WORD numVertices;
    //60
    WORD numIndices;
    //62
    WORD padding2;//maybe flags?

    //Th3 extension
    //64
    Collision::CollisionPLG* pCollisionPLG;//Collision BBTree
    //68
    DWORD* pUnk11;//Always NULL?
    //6C
    DWORD unk_flag;//always 6?
    //70
    BYTE state;
    BYTE padding3[3];
    Collision::Flags* pCollisionFlags;
    DWORD null1;
    DWORD null2;
    BYTE operationId;
    BYTE padding4[3];
    /*DWORD* pUnk12;//Always NULL?
    WORD* pCollisionFlags;//the flags for skatable, trigger etc
    DWORD* pUnk13;//bunch of 0xFF and random data
    DWORD* pUnk14;//bunch of 00
    BYTE unknown2;//Used when collision checking
    BYTE padding2[2];*/
    DWORD pUnknown;
    DWORD* pUnk16;//bunch of floats, maybe something about collision?
    DWORD name;//crc32
    DWORD* pUnk17;//similar to pUnk16


    struct Color
    {
        BYTE r;
        BYTE g;
        BYTE b;
        BYTE a;
    };

    bool IsCollideable()
    {
        for (auto i = 0; i < numIndices; i++)
        {
            if(!((WORD)pCollisionFlags[i] & (WORD)Collision::Flags::Hollow))
                return true;
        }
        return false;
    }

    //bool CollideWithLine(Collision::Leaf& leaf, Vertex& start, Vertex& dir, SuperSector* sector, Collision::CollData& data);

    Collision::CollisionPLG* const __restrict GetCollisionPlugin() const
    {
        return pCollisionPLG;
    }

    BBox* GetBBox()
    {
        return (BBox*)&bboxMax;
    }

    Color* GetColors()
    {
        return (Color*)color_offset;
    }

    void Update()
    {
        sector->Update();
    }

    //004fea30 00412230
    static DWORD GetSuperSectorName(DWORD checksum)
    {
        SuperSector* sector = GetSuperSector(checksum);
        if (sector)
            return checksum;
        return 0;
    }

    DWORD GetNumSplits()
    {
        return sector ? sector->numSplits : 0;
    }

    EXTERN static SuperSector* GetSuperSector(DWORD checksum)
    {
        //code to get index from checksum
        static DWORD* pSectors = 0;
        _asm mov eax, [0x0085A4B8];
        _asm mov eax, [eax];
        _asm test eax, eax;
        _asm jne con;
        _asm xor eax, eax;
        _asm ret;
    con:
        _asm mov edx, checksum;
        _asm and edx, 0x000FFFF;//Added 1 byte more to reduce collision in list
        _asm lea edx, [edx + edx * 2];
        _asm lea ecx, [eax + edx * 4];
        _asm mov pSectors, ecx;

        //Added for early exit, this optimization will make 2 less comparision per SuperSector, per frame
        if (*pSectors == checksum)//Checksum match
        {
            pSectors++;//skip 4 bytes to get the pointer
            return (SuperSector*)*pSectors;//return the pointer
        }

        pSectors += 2;
        pSectors = *(DWORD**)pSectors;

        //the SuperSectors are stored in a list 
        //each item is 12 bytes, the first 4 bytes is checksum, following 4 bytes is pointer to the SuperSector and last 4 bytes is link to next item(if it have collision)
        //since 2 or more checksums can have the same index we need to loop until we get a checksum match
        //if we find an uninitialized item before we get a checksum match it means the checksum is not in the list
        while (pSectors != 0)// && (DWORD)pSectors > 0x0040000 && (DWORD)pSectors < 0x10000000 && (DWORD)pSectors != 0x3F800000)//Continue until found an uninitialized item
        {
            if (*pSectors == checksum)//Checksum match
            {
                pSectors++;//skip 4 bytes to get the pointer
                return (SuperSector*)*pSectors;//return the pointer
            }

            pSectors += 2;
            pSectors = *(DWORD**)pSectors;
        }
        //MessageBox(0, "return NULL", "", 0);//Checksum is not in the list
        return NULL;
    }

    void SetPosition(const D3DXVECTOR3& position)
    {
        D3DXVECTOR3 pos = (bboxMax + bboxMin) / 2.0f;

        if (pos != position)
        {
            pos = position - pos;

            bboxMax += pos;
            bboxMin += pos;

            for (DWORD i = 0; i < numVertices; i++)
            {
                vertices[i] += pos;
            }
        }
    }

    void MoveToNode(DWORD checksum)
    {
        CStructHeader* node = Node::GetNodeStruct(checksum);
        if (node)
        {
            D3DXVECTOR3* pos = node->GetVector(Checksums::Position);
            if (pos)
                SetPosition(*pos);
            else
                SetPosition(D3DXVECTOR3(0, 0, 0));
        }
        else
            debug_print("Couldn't find Node %s in " __FUNCTION__ "\n");
    }

    //Used in the scripts create/kill/shatter/visible/invisible, the state will get updated the next frame
    void SetState(MeshState state)
    {

        typedef void(__cdecl* const pSetMeshState)(DWORD index, DWORD state, CScript* pScript);
        CScript pScript;
        pSetMeshState(0x00418DD0)(Node::GetNodeIndex(name), state, &pScript);
    }
};

namespace Collision
{

    extern CollCache defaultCollCache;
    extern DWORD operationId;

    extern BBox cache_bbox;
    class CollCache
    {
        BBox bbox;
        DWORD req_flags;
        DWORD idx;
        ::SuperSector* sectors[1024];

        friend class Manager;
    public:
        CollCache()
        {
            //impossible bbox
            bbox.max = Vertex(-FLT_MAX, -FLT_MAX, -FLT_MAX);
            bbox.min = Vertex(FLT_MAX, FLT_MAX, FLT_MAX);

            //set default requirements to all collideable flags
            req_flags = ::SuperSector::Flags::Collideable;
            
            //zero everything else
            ZeroMemory(&idx, sizeof(CollCache)-sizeof(BBox)-4);
        }

        void Clear()
        {
            CollCache();
        }

        //Set the CollCache requirement flag
        //all sectors that does not meet this requirement is ignored
        void SetRequirement(Collision::Flags req)
        {
            req_flags = (WORD)req;
        }

        void Update(RwLine& line, bool definite_mask = false);

        CollCache* GetCache(RwLine & line, bool definite_mask = false)
        {
            if (!bbox.Within_2D(line))
            {
                if (!defaultCollCache.bbox.Within_2D(line))
                    Update(line, definite_mask);
                else
                    return &defaultCollCache;
            }
            return this;
        }

        ::SuperSector* __restrict GetSuperSector(DWORD index) const
        {
            return sectors[index];
        }

        const DWORD GetNumSuperSectors() const
        {
            return idx;
        }

        ::SuperSector** GetSuperSectors()
        {
            sectors[idx] = NULL;
            return sectors;
        }

        CollCache(RwLine& line);
    };

    class Manager;
    class Sector
    {

    private:
        BBox bbox;
        DWORD			    numSectors;
        ::RpWorldSector**   super_sectors;

        void Sort()
        {
            for (DWORD i = 0; i < numSectors - 1; i++)
            {
                DWORD max = i;
                for (DWORD j = i + 1; j < numSectors; j++)
                {
                    if (super_sectors[max]->bboxMax.y < super_sectors[j]->bboxMax.y)
                        max = j;
                }
                if (max != i)
                {
                    ::RpWorldSector* temp = super_sectors[i];
                    super_sectors[i] = super_sectors[max];
                    super_sectors[max] = temp;
                }
            }
        }

        friend Manager;
    };

    class Manager
    {
        ::BBox			    world_bbox;
        float				sector_width;
        float 				sector_depth;
        int 				num_sectors_x;
        int 				num_sectors_z;
        //The world collision is divided up in 20*20 2d bboxes
        //Each bbox will contain around 1-50 SuperSectors
        //In 95% of cases we will only need to collide with 1 2d bbox
        //And in the worst case scenario we will need to collide with 3 2d bboxes, which takes up to twice as long
        Sector		        sectors[20][20];

    public:

        //We need to restore the new custom flags to default value, else we will have a crash when unloading the level
        void RestoreWorldSectorFlags()
        {
            for (int x = 0; x < num_sectors_x; x++)
            {
                for (int z = 0; z < num_sectors_z; z++)
                {
                    Sector& sector = sectors[x][z];

                    for (DWORD i = 0; i < sector.numSectors; i++)
                    {
                        sector.super_sectors[i]->req_flags = ::SuperSector::Flags::Default;
                    }
                }
            }
        }

        //Updates the world sector flags so we can ignore certain sectors and optimize our collision detection
        //it's only called once every time you change level
        void UpdateWorldSectorFlags()
        {
            //This method will loop throught he same sectors multiply times and is very inefficient
            //it's not so important since it's only called once every level change
            //however should find a better way to loop through all sectors...
            for (int x = 0; x < num_sectors_x; x++)
            {
                for (int z = 0; z < num_sectors_z; z++)
                {
                    Sector& sector = sectors[x][z];

                    for (DWORD i = 0; i < sector.numSectors; i++)
                    {
                        ::SuperSector* s = sector.super_sectors[i]; 
                        debug_print("%p %X\n", s, s->req_flags);

                        //Does the sector have collision plugin and is it collideable?
                        if (!s->pCollisionPLG || s->pCollisionPLG == INVALID_PTR || !(s->unk_flag & 6))
                        {
                            s->req_flags = ::SuperSector::Flags::NoCollision;
                            continue;
                        }

                        //Add all polies flags to the sector flag
                        for (int j = 0; j < s->numIndices; j++)
                        {
                            s->req_flags |= (WORD)s->pCollisionFlags[j];
                        }

                        //If trigger flag is not set and all polies are flagged as hollow the sector has no collision
                        if (!(s->req_flags & ::SuperSector::Flags::Trigger) && !(s->req_flags & ::SuperSector::Flags::Collideable))
                            s->req_flags = ::SuperSector::Flags::NoCollision;
                    }
                }
            }
        }

        void SortWorldSectors()
        {
            for (int x = 0; x < num_sectors_x; x++)
            {
                for (int z = 0; z < num_sectors_z; z++)
                {
                    Sector& sector = sectors[x][z];
                    sector.Sort();
                }
            }
        }
        __declspec(noalias) CollCache* GetIntersectingWorldSectors(const RwLine& line, CollData & data, bool update_cache = false) const
        {
            if (data.cache)
            {
                if (update_cache)
                {
                    if (!data.cache->bbox.Within_2D(line))
                    {
                        if (!defaultCollCache.bbox.Within_2D(line))
                            UpdateCache(line, data.cache);
                        else
                            return &defaultCollCache;
                    }
                }
                return data.cache;
            }

            if (!defaultCollCache.bbox.Within_2D(line))
                UpdateCache(line, &defaultCollCache);
            return &defaultCollCache;
        }
        /*::SuperSector** GetIntersectingWorldSectors(RwLine& line)
        {
            Collision::cache_bbox.max = Vertex(-FLT_MAX, -FLT_MAX, -FLT_MAX);
            Collision::cache_bbox.min = Vertex(FLT_MAX, FLT_MAX, FLT_MAX);
            RwLine test_line;
            Vertex dir = line.end - line.start;
            dir.Normalize();
            dir.Scale(0.5f);

            test_line.start[X] = line.start[X] - dir.x;
            test_line.start[Y] = line.start[Y] - dir.y;
            test_line.start[Z] = line.start[Z] - dir.z;
            test_line.end[X] = line.end[X] + dir.x;
            test_line.end[Y] = line.end[Y] + dir.y;
            test_line.end[Z] = line.end[Z] + dir.z;


            float x_offset = test_line.start[X] - world_bbox.min[X];
            float z_offset = test_line.start[Z] - world_bbox.min[Z];
            int start_x_box = (int)(x_offset / sector_width);
            int start_z_box = (int)(z_offset / sector_depth);

            start_x_box < 0 ? start_x_box = 0 : start_x_box >= num_sectors_x ? start_x_box = num_sectors_x-1 : start_x_box = start_x_box;
            start_z_box < 0 ? start_z_box = 0 : start_z_box >= num_sectors_z ? start_z_box = num_sectors_z-1 : start_z_box = start_z_box;

            x_offset = test_line.end[X] - world_bbox.min[X];
            z_offset = test_line.end[Z] - world_bbox.min[Z];
            int end_x_box = (int)(x_offset / sector_width);
            int end_z_box = (int)(z_offset / sector_depth);

            end_x_box < 0 ? end_x_box = 0 : end_x_box >= num_sectors_x ? end_x_box = num_sectors_x-1 : end_x_box = end_x_box;
            end_z_box < 0 ? end_z_box = 0 : end_z_box >= num_sectors_z ? end_z_box = num_sectors_z-1 : end_z_box = end_z_box;

            //Optimization if in same Sector
            if (start_x_box == end_x_box && start_z_box == end_z_box)
            {
                Sector* sector = &sectors[start_x_box][start_z_box];

                for(coll_cache_idx = 0; coll_cache_idx < sector->numSectors; coll_cache_idx++)
                {
                    ::RpWorldSector* world_sector = sector->super_sectors[coll_cache_idx];

                    //Skip if kill flag is set
                    if (!world_sector->unk_flag & 6 || world_sector->state & 6)
                        continue;
                    if (Collision::cache_bbox.max.x < sector->bbox.max.x) Collision::cache_bbox.max.x = sector->bbox.max.x;
                    if (Collision::cache_bbox.max.y < sector->bbox.max.y) Collision::cache_bbox.max.y = sector->bbox.max.y;
                    if (Collision::cache_bbox.max.z < sector->bbox.max.z) Collision::cache_bbox.max.z = sector->bbox.max.z;
                    if (Collision::cache_bbox.min.x > sector->bbox.min.x) Collision::cache_bbox.min.x = sector->bbox.min.x;
                    if (Collision::cache_bbox.min.y > sector->bbox.min.y) Collision::cache_bbox.min.y = sector->bbox.min.y;
                    if (Collision::cache_bbox.min.z > sector->bbox.min.z) Collision::cache_bbox.min.z = sector->bbox.min.z;
                    pCollCache[coll_cache_idx] = world_sector;
                }
                coll_cache_idx++;
                pCollCache[coll_cache_idx] = NULL;
                return pCollCache;
            }

            if (start_x_box > end_x_box)
            {
                float temp = start_x_box;
                start_x_box = end_x_box;
                end_x_box = temp;
            }
            if (start_z_box > end_z_box)
            {
                float temp = start_z_box;
                start_z_box = end_z_box;
                end_z_box = temp;
            }

            //New operation ID
            operationId++;

            //Set CollCache Index to zero
            coll_cache_idx = 0;

            //Now get Sectors inside the line bbox start - end
            for (int i = start_x_box; i <= end_x_box; i++)
            {
                for (int j = start_z_box; j <= end_z_box; j++)
                {
                    for (DWORD k = 0; k < sectors[i][j].numSectors; k++)
                    {
                        ::SuperSector* cs = sectors[i][j].super_sectors[k];

                        //OperationId is used so we only add each SuperSector once
                        if (cs->pUnknown != operationId)
                        {
                            //Skip if kill flag is set
                            if (!cs->unk_flag & 6 || cs->state & 6)
                                continue;

                            if (coll_cache_idx < 1023)
                            {
                                if (Collision::cache_bbox.max.x < sectors[i][j].bbox.max.x) Collision::cache_bbox.max.x = sectors[i][j].bbox.max.x;
                                if (Collision::cache_bbox.max.y < sectors[i][j].bbox.max.y) Collision::cache_bbox.max.y = sectors[i][j].bbox.max.y;
                                if (Collision::cache_bbox.max.z < sectors[i][j].bbox.max.z) Collision::cache_bbox.max.z = sectors[i][j].bbox.max.z;
                                if (Collision::cache_bbox.min.x > sectors[i][j].bbox.min.x) Collision::cache_bbox.min.x = sectors[i][j].bbox.min.x;
                                if (Collision::cache_bbox.min.y > sectors[i][j].bbox.min.y) Collision::cache_bbox.min.y = sectors[i][j].bbox.min.y;
                                if (Collision::cache_bbox.min.z > sectors[i][j].bbox.min.z) Collision::cache_bbox.min.z = sectors[i][j].bbox.min.z;
                                pCollCache[coll_cache_idx++] = cs; 
                                cs->pUnknown = operationId;
                            }
                        }

                    }
                }
            }
            pCollCache[coll_cache_idx] = NULL;
            return pCollCache;
        }*/

        __declspec(noalias) void UpdateCache(const RwLine& line, CollCache* const __restrict cache, bool definite_mask = false) const
        {
            D3DXVECTOR3* p = Game::skater->GetPosition();
            debug_print("%p\n", p);
            debug_print("Updating cache\n");
            //Set impossible BBOX
            cache->bbox.max = Vertex(-FLT_MAX, -FLT_MAX, -FLT_MAX);
            cache->bbox.min = Vertex(FLT_MAX, FLT_MAX, FLT_MAX);

            //Set CollCache Index to zero
            cache->idx = 0;

            Line test_line;
            Vertex dir = *(Vertex*)&line.end - *(Vertex*)&line.start;
            dir.Normalize();
            dir.Scale(0.5f);

            test_line.start[X] = line.start[X] - dir.x;
            test_line.start[Y] = line.start[Y] - dir.y;
            test_line.start[Z] = line.start[Z] - dir.z;
            test_line.end[X] = line.end[X] + dir.x;
            test_line.end[Y] = line.end[Y] + dir.y;
            test_line.end[Z] = line.end[Z] + dir.z;

            BBox line_bbox;
            line_bbox.max.x = line.end.x > line.start.x ? line.end.x : line.start.x;
            line_bbox.max.x += 0.5f;
            line_bbox.max.y = line.end.y > line.start.y ? line.end.y : line.start.y;
            line_bbox.max.y += 0.5f;
            line_bbox.max.z = line.end.z > line.start.z ? line.end.z : line.start.z;
            line_bbox.max.z += 0.5f;

            line_bbox.min.x = line.end.x < line.start.x ? line.end.x : line.start.x;
            line_bbox.min.x -= 0.5f;
            line_bbox.min.y = line.end.y < line.start.y ? line.end.y : line.start.y;
            line_bbox.min.y -= 0.5f;
            line_bbox.min.z = line.end.z < line.start.z ? line.end.z : line.start.z;
            line_bbox.min.z -= 0.5f;

            float x_offset = test_line.start[X] - world_bbox.min[X];
            float z_offset = test_line.start[Z] - world_bbox.min[Z];
            int start_x_box = (int)(x_offset / sector_width);
            int start_z_box = (int)(z_offset / sector_depth);
            /*cache->bbox.min.x = world_bbox.min.x + (float)start_x_box * sector_width;
            cache->bbox.min.z = world_bbox.min.z + (float)start_z_box * sector_width;*/

            start_x_box < 0 ? start_x_box = 0 : start_x_box >= num_sectors_x ? start_x_box = num_sectors_x - 1 : start_x_box = start_x_box;
            start_z_box < 0 ? start_z_box = 0 : start_z_box >= num_sectors_z ? start_z_box = num_sectors_z - 1 : start_z_box = start_z_box;

            x_offset = test_line.end[X] - world_bbox.min[X];
            z_offset = test_line.end[Z] - world_bbox.min[Z];
            int end_x_box = (int)((x_offset / sector_width) /*+ 0.5f*/);
            int end_z_box = (int)((z_offset / sector_depth) /*+ 0.5f*/);
            /*cache->bbox.max.x = world_bbox.min.x + (float)end_x_box * sector_width;
            cache->bbox.max.z = world_bbox.min.z + (float)end_z_box * sector_width;*/

            end_x_box < 0 ? end_x_box = 0 : end_x_box >= num_sectors_x ? end_x_box = num_sectors_x - 1 : end_x_box = end_x_box;
            end_z_box < 0 ? end_z_box = 0 : end_z_box >= num_sectors_z ? end_z_box = num_sectors_z - 1 : end_z_box = end_z_box;

            //Optimization if in same Sector, will return up to 30% faster
            if (start_x_box == end_x_box && start_z_box == end_z_box)
            {
                //MessageBox(0, 0, 0, 0);
                //Optimize array access
                const Sector* sector = &sectors[start_x_box][start_z_box];

                for (DWORD i = 0; i < sector->numSectors; i++)
                {
                    ::RpWorldSector* world_sector = sector->super_sectors[i];

                    //skip if non collide flag is set, or requirement is not met
                    if (!(world_sector->req_flags & cache->req_flags))
                        continue;

                    //since we are casting several rays when scanning for spine transfer targets
                    //we should try to raycast as few SuperSectors as possible
                    //so instead of raycasting all SuperSectors in the divided sector
                    //we mask out SuperSectors that are not intersecting with our line
                    //this should speedup the scanning drastically
                    if (definite_mask && !line_bbox.Intersect_2D(*world_sector->GetBBox()))
                        continue;

                    if (cache->bbox.max.x < sector->bbox.max.x) cache->bbox.max.x = sector->bbox.max.x;
                    if (cache->bbox.max.y < sector->bbox.max.y) cache->bbox.max.y = sector->bbox.max.y;
                    if (cache->bbox.max.z < sector->bbox.max.z) cache->bbox.max.z = sector->bbox.max.z;
                    if (cache->bbox.min.x > sector->bbox.min.x) cache->bbox.min.x = sector->bbox.min.x;
                    if (cache->bbox.min.y > sector->bbox.min.y) cache->bbox.min.y = sector->bbox.min.y;
                    if (cache->bbox.min.z > sector->bbox.min.z) cache->bbox.min.z = sector->bbox.min.z;
                    cache->sectors[cache->idx++] = world_sector;
                }
                cache->sectors[cache->idx] = NULL;
                return;
            }

            //Optimize Array access order
            if (start_x_box > end_x_box)
            {
                int temp = start_x_box;
                start_x_box = end_x_box;
                end_x_box = temp;
                /*temp = cache->bbox.min.x;
                cache->bbox.min.x = cache->bbox.max.x;
                cache->bbox.max.x = temp;*/
            }
            if (start_z_box > end_z_box)
            {
                int temp = start_z_box;
                start_z_box = end_z_box;
                end_z_box = temp;
                /*temp = cache->bbox.min.z;
                cache->bbox.min.z = cache->bbox.max.z;
                cache->bbox.max.z = temp;*/
            }

            /*cache->bbox.min.x = world_bbox.min.x + (float)start_x_box * sector_width;
            cache->bbox.min.z = world_bbox.min.z + (float)start_z_box * sector_width;*/

            //New operation ID
            operationId++;

            //Now get Sectors inside the line bbox start - end
            for (int i = start_x_box; i <= end_x_box; i++)
            {
                for (int j = start_z_box; j <= end_z_box; j++)
                {
                    //Optimize array access
                    const Collision::Sector & sector = sectors[i][j];

                    if (cache->bbox.max.x < sector.bbox.max.x) cache->bbox.max.x = sector.bbox.max.x;
                    if (cache->bbox.max.y < sector.bbox.max.y) cache->bbox.max.y = sector.bbox.max.y;
                    if (cache->bbox.max.z < sector.bbox.max.z) cache->bbox.max.z = sector.bbox.max.z;
                    if (cache->bbox.min.x > sector.bbox.min.x) cache->bbox.min.x = sector.bbox.min.x;
                    if (cache->bbox.min.y > sector.bbox.min.y) cache->bbox.min.y = sector.bbox.min.y;
                    if (cache->bbox.min.z > sector.bbox.min.z) cache->bbox.min.z = sector.bbox.min.z;


                    for (DWORD k = 0; k < sector.numSectors; k++)
                    {
                        ::SuperSector* cs = sector.super_sectors[k];

                        //OperationId is used so we only add each SuperSector once
                        if (cs->pUnknown != operationId)
                        {
                            //skip if non collide flag is set, or requirement is not met
                            if (!(cs->req_flags & cache->req_flags))
                                continue;

                            //since we are casting several rays when scanning for spine transfer targets
                            //we should try to raycast as few SuperSectors as possible
                            //so instead of raycasting all SuperSectors in the divided sector
                            //we mask out SuperSectors that are not intersecting with our line
                            //this should speedup the scanning drastically
                            if (definite_mask && !line_bbox.Intersect_2D(*cs->GetBBox()))
                                continue;

                            assert(cache->idx < 1023, "Too many SuperSectors in cache...");
                            cache->sectors[cache->idx++] = cs;
                            cs->pUnknown = operationId;
                        }

                    }
                }
            }
            cache->sectors[cache->idx] = NULL;
            return;
        }
    };
};
//#pragma pop(pack)
//bool second = true;

struct MovingObject
{
    enum
    {
        // not used
        // MOVINGOBJ_STATUS_IDLE 				= 0,

        MOVINGOBJ_STATUS_ROTX = (1 << 0),
        MOVINGOBJ_STATUS_ROTY = (1 << 1),
        MOVINGOBJ_STATUS_ROTZ = (1 << 2),
        MOVINGOBJ_STATUS_MOVETO = (1 << 3),
        MOVINGOBJ_STATUS_ON_PATH = (1 << 4),
        MOVINGOBJ_STATUS_QUAT_ROT = (1 << 5),
        MOVINGOBJ_STATUS_LOOKAT = (1 << 6), // will be on in addition to rotx, y or z flags.
        MOVINGOBJ_STATUS_HOVERING = (1 << 7),
        MOVINGOBJ_STATUS_JUMPING = (1 << 8),
        MOVINGOBJ_STATUS_FOLLOWING_LEADER = (1 << 9),

        // moved into lock obj component
        // MOVINGOBJ_STATUS_LOCKED_TO_OBJECT	= ( 1 << 10 ),

        // don't do any other processing, until this flag is off:
        // wasn't being used...
        // MOVINGOBJ_STATUS_HIGH_LEVEL 		= ( 1 << 31 ), // running C-code on a high level.
    };//From thug1source, currently not used

    MeshState state;
    //DWORD rotation;
    enum Types
    {
        MOVE_TO_POS,
        MOVE_TO_NODE,
        FOLLOW_PATH_LINKED,
        ANGULAR_VELOCITY,
        FOLLOW_MODEL,
    };
    Types Type;
    float timer;
    float end;
    float speed;
    D3DXVECTOR3 goal;
    D3DXVECTOR3 pos;
    union
    {
        D3DXVECTOR3 angle;
        D3DXVECTOR3 acc;
    };
    D3DXVECTOR3 goalAngle;
    SuperSector* sector;
    CStructHeader* link;
    CScript* pScript;
    D3DXVECTOR3 bboxMax;
    D3DXVECTOR3 bboxMin;
    D3DXVECTOR3* vertices;
    D3DXVECTOR3 acceleration;
    D3DXVECTOR3 deceleration;
    Matrix orient;
    Matrix position;
    Matrix velocity;
    Model* model;


    MovingObject(SuperSector* _sector, Model* mdl);

    MovingObject(SuperSector* _sector, D3DXVECTOR3& _goal, CScript* _pScript)
    {
        debug_print("MOVE_TO_NODE\n");
        state = MeshState::create;
        Type = MOVE_TO_NODE;
        link = NULL;
        sector = _sector;
        this->pScript = _pScript;
        pos = (sector->bboxMax + sector->bboxMin) / 2.0f;
        angle = Vertex(0.0f, 0.0f, 0.0f);
        goal = _goal;
        timer = 0.0f;
        const D3DXVECTOR3& v = (pos - goal);
        float distance = D3DXVec3Length(&v);
        speed = 100.0f;
        end = distance / speed;

        vertices = NULL;
    }

    void Kill()
    {
        state = MeshState::kill;
    }

    MovingObject(SuperSector* _sector, D3DXVECTOR3& _angle, CScript* _pScript, DWORD checksum)
    {
        debug_print("ANGLULAR_VELOCITY\n");
        state = MeshState::create;
        Type = ANGULAR_VELOCITY;
        sector = _sector;
        this->link = Node::GetNodeStruct(checksum);
        pScript = _pScript;
        assert(this->link, "No link in MovingObject...");
        CStructHeader* _position;
        if (!this->link->GetStruct(Checksums::Position, &_position))
            pos = D3DXVECTOR3(0, 0, 0);
        else
            pos = *_position->pVec;


        /*angle = _angle;
        angle *= 0.0015339794921875f;*/
        angle = D3DXVECTOR3(0, 0, 0);
        acceleration = _angle * D3DX_PI / 2048.0f;
        goalAngle = D3DXVECTOR3(0, 0, 0);

        bboxMax = D3DXVECTOR3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
        bboxMin = D3DXVECTOR3(FLT_MAX, FLT_MAX, FLT_MAX);
        //vertices = new D3DXVECTOR3[sector->numVertices];
        vertices = NULL;

        for (DWORD i = 0; i < sector->numVertices; i++)
        {
            if (sector->vertices[i].x > bboxMax.x)
                bboxMax.x = sector->vertices[i].x;
            if (sector->vertices[i].x < bboxMin.x)
                bboxMin.x = sector->vertices[i].x;

            if (sector->vertices[i].y > bboxMax.y)
                bboxMax.y = sector->vertices[i].y;
            if (sector->vertices[i].y < bboxMin.y)
                bboxMin.y = sector->vertices[i].y;

            if (sector->vertices[i].z > bboxMax.z)
                bboxMax.z = sector->vertices[i].z;
            if (sector->vertices[i].z < bboxMin.z)
                bboxMin.z = sector->vertices[i].z;
            //vertices[i] = sector->vertices[i] - pos;
        }
        sector->bboxMax = bboxMax;
        sector->bboxMin = bboxMin;
        bboxMax -= pos;
        bboxMin -= pos;
        pos = (sector->bboxMax + sector->bboxMin) / 2.0f;
        D3DXMatrixTranslation(&position, pos.x, pos.y, pos.z);
        D3DXMatrixIdentity(&orient);
        D3DXMatrixIdentity(&velocity);
    }

    /*bool SetUpLookAtPos(const Vertex& lookToPos, const Vertex& currentPos, int headingAxis, int rotAxis, float threshold)
    {
        Vertex pathHeading = lookToPos - currentPos;

        angle[rotAxis] = 0.0f;
        goalAngle[rotAxis] = GetAngle(orient, pathHeading, headingAxis, rotAxis);
        if (fabs(goalAngle[rotAxis]) > threshold)
        {
            rotation |= (MOVINGOBJ_STATUS_ROTX << rotAxis);
            return true;
        }

        return false;
    }*/


    MovingObject(SuperSector* _sector, D3DXVECTOR3& _pos, D3DXVECTOR3& _goal, CScript* _pScript, CStructHeader* _node)
    {
        debug_print("FOLLOW_PATH_LINKED\n");
        Type = FOLLOW_PATH_LINKED;
        state = MeshState::create;
        sector = _sector;

        angle = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        goal = _goal;
        pos = _pos;
        timer = 0.0f;
        const D3DXVECTOR3& v = (pos - goal);
        float distance = D3DXVec3Length(&v);
        speed = 1000.0f;
        end = distance / speed;
        this->link = _node;
        this->pScript = _pScript;
        vertices = NULL;
        bboxMax = sector->bboxMax - pos;
        bboxMin = sector->bboxMin - pos;
        //std::copy(sector->vertices, sector->vertices + sector->numVertices, vertices);
        /*for (DWORD i = 0; i < sector->numVertices; i++)
        {
            vertices[i] = sector->vertices[i];
        }*/
        /*CStructHeader* angles;
        if (!_pScript->node->GetNodeStruct()->GetStruct(Checksums::Angles, &angles))
            angle = D3DXVECTOR3(0, 0, 0);
        else
            angle = *angles->pVec;*/

            /*if (!link->GetStruct(Checksums::Angles, &angles))
                goalAngle = D3DXVECTOR3(0, 0, 0);
            else
                goalAngle = *angles->pVec;*/

                /*CStructHeader* angles;
                if (!_pScript->node->GetNodeStruct()->GetStruct(Checksums::Angles, &angles))
                    angle = D3DXVECTOR3(0, 0, 0);
                else
                    angle = *angles->pVec;
                D3DXMatrixIdentity(&orient);
                D3DXMatrixRotationYawPitchRoll(&orient, angle.y, angle.x, angle.z);*/
        D3DXMATRIX nodeRotation;
        D3DXMatrixIdentity(&nodeRotation);
        D3DXMatrixIdentity(&orient);
        //D3DXVECTOR3 pathHeading = goal - pos;
        //goalAngle = //Vertex(0, GetAngle(orient, *(Vertex*)&pathHeading, Z, Y), 0);// 
        goalAngle = D3DXVECTOR3(0, atan2f((pos.x - goal.x), (pos.z - goal.z)), 0);//D3DXVECTOR3(0, AngleY(orient, pos, goal), 0);
    //angle.y = D3DX_PI;
    //goalAngle += angle;

    /*D3DXVECTOR3 lookAt = goalAngle - angle;
                debug_print("current lookAt %f %f %f\n", lookAt.x, lookAt.y, lookAt.z);
                D3DXVec3Normalize(&lookAt, &lookAt);
                lookAt *= delta;
                debug_print("delta lookAt %f %f %f\n", lookAt.x, lookAt.y, lookAt.z);*/
                /*(*(Matrix*)&orient).RotateYLocal(goalAngle.y);
                (*(Matrix*)&orient).OrthoNormalizeAbout2(Y);*/

                /*direction = goal - pos;
                D3DXVec3Normalize(&Velocity, &direction);
                Velocity *= speed * delta;

                D3DXMatrixTranslation(&nodeTranslation, Velocity.x + pos.x, Velocity.y + pos.y, Velocity.z + pos.z);
                D3DXMatrixMultiply(&world, &nodeRotation, &nodeTranslation)*/


        bboxMax = D3DXVECTOR3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
        bboxMin = D3DXVECTOR3(FLT_MAX, FLT_MAX, FLT_MAX);
        sector->bboxMax = D3DXVECTOR3(FLT_MAX, FLT_MAX, FLT_MAX);
        sector->bboxMin = D3DXVECTOR3(-FLT_MAX, -FLT_MAX, -FLT_MAX);

        for (DWORD i = 0; i < sector->numVertices; i++)
        {
            sector->vertices[i] -= pos;
            sector->vertices[i] = Transform(sector->vertices[i], orient);
            sector->vertices[i] += pos;
            if (bboxMax.x < sector->vertices[i].x)
                bboxMax.x = sector->vertices[i].x;
            if (bboxMin.x > sector->vertices[i].x)
                bboxMin.x = sector->vertices[i].x;

            if (bboxMax.y < sector->vertices[i].y)
                bboxMax.y = sector->vertices[i].y;
            if (bboxMin.y > sector->vertices[i].y)
                bboxMin.y = sector->vertices[i].y;

            if (bboxMax.z < sector->vertices[i].z)
                bboxMax.z = sector->vertices[i].z;
            if (bboxMin.z > sector->vertices[i].z)
                bboxMin.z = sector->vertices[i].z;
        }

        sector->bboxMax = bboxMax;
        sector->bboxMin = bboxMin;

        /*bboxMax = D3DXVECTOR3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
        bboxMin = D3DXVECTOR3(FLT_MAX, FLT_MAX, FLT_MAX);
        vertices = new D3DXVECTOR3[sector->numVertices];

        for (DWORD i = 0; i < sector->numVertices; i++)
        {
            vertices[i] = sector->vertices[i] - pos;
            if (bboxMax.x < sector->vertices[i].x)
                bboxMax.x = sector->vertices[i].x;
            if (bboxMin.x > sector->vertices[i].x)
                bboxMin.x = sector->vertices[i].x;

            if (bboxMax.y < sector->vertices[i].y)
                bboxMax.y = sector->vertices[i].y;
            if (bboxMin.y > sector->vertices[i].y)
                bboxMin.y = sector->vertices[i].y;

            if (bboxMax.z < sector->vertices[i].z)
                bboxMax.z = sector->vertices[i].z;
            if (bboxMin.z > sector->vertices[i].z)
                bboxMin.z = sector->vertices[i].z;
        }
        sector->bboxMax = bboxMax;
        sector->bboxMin = bboxMin;
        bboxMax -= pos;
        bboxMin -= pos;*/

        //pos = (sector->bboxMax + sector->bboxMin) / 2.0f;
        angle = goalAngle;
        debug_print("Generated new MovingObject pos %f %f %f goal %f %f %f goalAngle %f speed %f\n", pos.x, pos.y, pos.z, goal.x, goal.y, goal.z, goalAngle.y, speed);
    }


    ~MovingObject()
    {
        while (updatingObjects)
            Sleep(10);
        if (vertices && !InvalidReadPtr(vertices) && !InvalidReadPtr(vertices, sector->numVertices * sizeof(D3DXVECTOR3)))
            delete[] vertices;
        vertices = NULL;
        sector = NULL;
    }


    bool Update(float delta);


};

EXTERN extern std::vector<MovingObject> movingObjects;//List of Objects on the move
#endif