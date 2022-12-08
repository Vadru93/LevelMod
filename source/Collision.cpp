#include "pch.h"
#include "Collision.h"
#include "Objects\SuperSectors.h"

bool BBox::Within_2D(const RwLine& line)
{
        return ((line.start[X] > min[X]) && /*(line.start[Y] > min[Y]) &&*/ (line.start[Z] > min[Z]) &&
            (line.start[X] < max[X]) && /*(line.start[Y] < max[Y]) &&*/ (line.start[Z] < max[Z]) &&
            (line.end[X] > min[X]) && /*(line.end[Y] > min[Y]) &&*/ (line.end[Z] > min[Z]) &&
                (line.end[X] < max[X]) && /*(line.end[Y] < max[Y]) &&*/ (line.end[Z] < max[Z]));
}

bool BBox::Intersect(const BBox& bbox)
{
    if (((min[X] > bbox.max[X]) || (bbox.min[X] > max[X]) ||
        (min[Z] > bbox.max[Z]) || (bbox.min[Z] > max[Z]) ||
        (min[Y] > bbox.max[Y]) || (bbox.min[Y] > max[Y])))
        return false;
    return true;
}

bool BBox::Intersect_2D(const BBox& bbox)
{
    if (((min[X] > bbox.max[X]) || (bbox.min[X] > max[X]) ||
        (min[Z] > bbox.max[Z]) || (bbox.min[Z] > max[Z])))
        return false;
    return true;
}

namespace Collision
{
    ::SuperSector* pCollCache[1024];
    DWORD coll_cache_idx = 0;
    DWORD operationId = 0;
    CollCache defaultCollCache;
    WORD pInterFaces[1024];
    DWORD inter_face_idx;
    DWORD numInterFaces;
    BBox cache_bbox;
    CollCache* spine_cache;
    CollCache* trigger_cache;
#define EPSILON 0.000001f

    Flags& operator+(const Flags& f1, const Flags& f2)
    {
        WORD result = (WORD)f1 | (WORD)f2;
        return *(Flags*)&result;
    }

    void CollCache::Update(RwLine& line, bool definite_mask)
    {
        RpWorld* world = RwViewer::Instance()->GetCurrentWorld();
        NxPlugin* plg = world->GetWorldPluginData();
        Collision::Manager* cld_manager = plg->GetManager();

        cld_manager->UpdateCache(line, this, definite_mask);
    }

    CollCache::CollCache(RwLine& line)
    {
        Update(line);
    }

    bool intersect_triangle(
        D3DXVECTOR3& start, D3DXVECTOR3& dir, D3DXVECTOR3& A, D3DXVECTOR3& B, D3DXVECTOR3& C, float& t,
        float& u, float& v, D3DXVECTOR3& N
    ) {
        D3DXVECTOR3 E1 = B - A;
        D3DXVECTOR3 E2 = C - A;
        D3DXVec3Cross(&N, &E1, &E2);
        float det = -D3DXVec3Dot(&dir, &N);
        float invdet = 1.0f / det;
        D3DXVECTOR3 AO = start - A;
        D3DXVECTOR3 DAO;
        D3DXVec3Cross(&DAO, &AO, &dir);
        u = D3DXVec3Dot(&E2, &DAO) * invdet;
        v = -D3DXVec3Dot(&E1, &DAO) * invdet;
        t = D3DXVec3Dot(&AO, &N) * invdet;
        return (det >= 1e-6 && t >= 0.0 && u >= 0.0 && v >= 0.0 && (u + v) <= 1.0);
    }


    bool TriangleIntersection(const D3DXVECTOR3* v1, const D3DXVECTOR3* v2, const D3DXVECTOR3* v3, const D3DXVECTOR3* rayPos, const D3DXVECTOR3* rayDir, float & out_distance)
    {
        D3DXVECTOR3 edge1, edge2, tvec, pvec, qvec;
        float det, inv_det;

        edge1 = *v2 - *v1;
        edge2 = *v3 - *v1;

        D3DXVec3Cross(&pvec, rayDir, &edge2);
        det = D3DXVec3Dot(&edge1, &pvec);

        if (det > -EPSILON && det < EPSILON)
            return false;

        inv_det = 1.0f / det;

        tvec = *rayPos - *v1;
        float u = D3DXVec3Dot(&tvec, &pvec) * inv_det;
        if (u < 0.0f)
            return false;

        D3DXVec3Cross(&qvec, &tvec, &edge1);
        float v = D3DXVec3Dot(rayDir, &qvec) * inv_det;
        if (v < 0.0f || u + v > 1.0f)
            return false;

        float t = D3DXVec3Dot(&edge2, &qvec) * inv_det;
        if (t < 0.00001f)
            return false;

        out_distance = t;
        return true;
    }

    __inline __declspec (noalias) bool CollisionPLG::TraverseBranch(const Branch* const __restrict branch, const RwLine& line, const Vertex& dir, const ::SuperSector* const __restrict sector, CollData& data) const
    {
        //axis is optimized for easy access of x, y and z
        //x + 0 bytes = x
        //x + 4 bytes = y
        //x + 8 bytes = z
        WORD axis = branch->axis;
        //align the line start and end point with the branch
        float axis_aligned_start = *(float*)((BYTE*)(&line.start.x) + axis);
        float axis_aligned_end = *(float*)((BYTE*)(&line.end.x) + axis);

        //debug_print("Axis %d branchmax % f branchmin % f linemax % f linemin % f\n", axis, branch->max, branch->min, axis_aligned_start, axis_aligned_end);

        //compare to mid, go left or right?
        //optimization suggestion: change BSP collision tree to store middle point directly instead of storing min and max
        //this will use 4 less bytes per node and use few less instructions per node call
        //this should improve collision detection speed since less bytes will lead to less cache misses
        //and the removed instructions will obviusly be faster code, how much? who knows...
        //RW 3.4 and later has this optimization
        float mid = (branch->max - branch->min) * 0.5f + branch->min;

        if (axis_aligned_start < mid && axis_aligned_end < mid)
        {
            //debug_print("Going Left\n");
            if (branch->leftType == 2)
                return TraverseBranch(&branches[branch->leftIndex], line, dir, sector, data);
            else
                return CollideWithLine(leafs[branch->leftIndex], *(Vertex*)&line.start, dir, sector, data);
        }
        else if (axis_aligned_start >= mid && axis_aligned_end >= mid)
        {
            //debug_print("Going Righ\n");
            if (branch->rightType == 2)
                return TraverseBranch(&branches[branch->rightIndex], line, dir, sector, data);
            else
                return CollideWithLine(leafs[branch->rightIndex], *(Vertex*)&line.start, dir, sector, data);
        }
        else
        {
            //How often are we going both ways?
            //Maybe can optimize the tree creation in LevelEditor to make it happen less often
            //debug_print("Going Both\n");
            bool bCollided_left, bCollided_right;
            if (branch->leftType == 2)
                bCollided_left = TraverseBranch(&branches[branch->leftIndex], line, dir, sector, data);
            else
                bCollided_left = CollideWithLine(leafs[branch->leftIndex], *(Vertex*)&line.start, dir, sector, data);

            if (branch->rightType == 2)
                bCollided_right = TraverseBranch(&branches[branch->rightIndex], line, dir, sector, data);
            else
                bCollided_right = CollideWithLine(leafs[branch->rightIndex], *(Vertex*)&line.start, dir, sector, data);
            return bCollided_left || bCollided_right;
        }


        //don't remember if this was some wild idea I had or if it's actually how RW does it??
        //either way it seems to be a bad implementation, keeping it here just in case...
        
        /*float start_min = *(float*)((BYTE*)(&line.start.x) + axis) - branch->min;
        float end_min = *(float*)((BYTE*)(&line.end.x) + axis) - branch->min;
        if ((*(int*)&start_min < 0) && (*(int*)&end_min < 0))
        {
            //debug_print("Going Left\n");
            if (branch->leftType == 2)
                return TraverseBranch(&branches[branch->leftIndex], line, dir, sector, data);
            else
                return CollideWithLine(leafs[branch->leftIndex], *(Vertex*)&line.start, dir, sector, data);
        }
        else
        {
            float start_max = *(float*)((BYTE*)(&line.start.x) + axis) - branch->max;
            float end_max = *(float*)((BYTE*)(&line.end.x) + axis) - branch->max;
            if (*(int*)&start_max < 0 || *(int*)&end_max < 0)
            {
                //debug_print("Going Both\n");
                bool bCollided_left, bCollided_right;
                if (branch->leftType == 2)
                    bCollided_left = TraverseBranch(&branches[branch->leftIndex], line, dir, sector, data);
                else
                    bCollided_left = CollideWithLine(leafs[branch->leftIndex], *(Vertex*)&line.start, dir, sector, data);

                if (branch->rightType == 2)
                    bCollided_right = TraverseBranch(&branches[branch->rightIndex], line, dir, sector, data);
                else
                    bCollided_right = CollideWithLine(leafs[branch->rightIndex], *(Vertex*)&line.start, dir, sector, data);
                return bCollided_left || bCollided_right;
            }
            else
            {
                //debug_print("Going Righ\n");
                if (branch->rightType == 2)
                    return TraverseBranch(&branches[branch->rightIndex], line, dir, sector, data);
                else
                    return CollideWithLine(leafs[branch->rightIndex], *(Vertex*)&line.start, dir, sector, data);
            }
        }*/
        return false;
    }

    __inline __declspec (noalias) bool CollisionPLG::CollideWithLine(const Vertex& start, const Vertex& dir, const ::SuperSector* const __restrict sector, CollData& data) const
    {
#pragma omp parallel for
        for (auto idx = 0; idx < numFaces; idx++)
        {
            DWORD collFlags = (WORD)sector->pCollisionFlags[idx];
            if (!(collFlags & (WORD)data.ignore_1) && !(~collFlags & (WORD)data.ignore_0))
            {
                RpTriangle& face = sector->triangles[idx];
                Vertex v0 = sector->vertices[face.a];
                Vertex v1 = sector->vertices[face.b];
                Vertex v2 = sector->vertices[face.c];

                float distance;/* , u, v;
                Vertex normal;*/
                if (TriangleIntersection(&v0, &v1, &v2, &start, &dir, distance))//if (intersect_triangle(*(D3DXVECTOR3*)&line.start, rayDir, v0, v1, v2, distance, u, v, normal))
                {
                    if (data.unk > distance)
                    {

                        data.unk = distance;
                        data.collided = true;
                        data.checksum = sector->name;
                        //data.p_sector = sector;
                        data.index = faces[idx];
                        data.terrain = face.matIndex;
                        data.collFlags = (Collision::Flags)collFlags;
                        data.v0 = v0;
                        data.v1 = v1;
                        data.v2 = v2;

                    }
                }
            }
        }

        return data.collided;
    }

    __inline __declspec (noalias) bool CollisionPLG::CollideWithLine(const Leaf& leaf, const Vertex& start, const Vertex& dir, const ::SuperSector* const __restrict sector, CollData& data) const
    {
        DWORD last = leaf.numFaces + leaf.idx;
        for (DWORD idx = leaf.idx; idx < last; idx++)
        {
            DWORD collFlags = (WORD)sector->pCollisionFlags[faces[idx]];
            if (!(collFlags & (WORD)data.ignore_1) && !(~collFlags & (WORD)data.ignore_0))
            {
                RpTriangle& face = sector->triangles[faces[idx]];
                Vertex v0 = sector->vertices[face.a];
                Vertex v1 = sector->vertices[face.b];
                Vertex v2 = sector->vertices[face.c];

                float distance;/* , u, v;
                Vertex normal;*/
                if (TriangleIntersection(&v0, &v1, &v2, &start, &dir, distance))//if (intersect_triangle(*(D3DXVECTOR3*)&line.start, rayDir, v0, v1, v2, distance, u, v, normal))
                {
                    if (data.unk > distance)
                    {

                        data.unk = distance;
                        data.collided = true;
                        data.checksum = sector->name;
                        //data.p_sector = sector;
                        data.index = faces[idx];
                        data.terrain = face.matIndex;
                        data.collFlags = (Collision::Flags)collFlags;
                        data.v0 = v0;
                        data.v1 = v1;
                        data.v2 = v2;

                    }
                }
            }
        }
        return data.collided;
    }

    __declspec (noalias) bool FindFirstCollision(const RwLine& line, CollData& data, bool update_cache)
    {
        data.unk = FLT_MAX;
        data.collided = false;

        Vertex rayDir = line.end - line.start;


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

        RwLine extended_line;
        Vertex dir = line.end - line.start;
        dir.Normalize();
        dir.Scale(0.5f);

        extended_line.start[X] = line.start[X] - dir.x;
        extended_line.start[Y] = line.start[Y] - dir.y;
        extended_line.start[Z] = line.start[Z] - dir.z;
        extended_line.end[X] = line.end[X] + dir.x;
        extended_line.end[Y] = line.end[Y] + dir.y;
        extended_line.end[Z] = line.end[Z] + dir.z;


        RpWorld* world = RwViewer::Instance()->GetCurrentWorld();
        NxPlugin* plg = world->GetWorldPluginData();
        Collision::Manager* cld_manager = plg->GetManager();

        CollCache* cache = cld_manager->GetIntersectingWorldSectors(line, data, update_cache);

        for (const ::SuperSector* const* __restrict sectors = cache->GetSuperSectors(); const ::SuperSector* const __restrict sector = *sectors; sectors++)
        {
            if (sector == NULL)
                return data.collided;

            //is the line intersecting the sector bbox?
            if (line_bbox.Intersect((*(BBox*)&sector->bboxMax)))
            {
                //Is the sector killed since last cache update?
                if (sector->state != 6)
                {
                    //is the line intersecting any of the sector triangles?
                    if (sector->GetCollisionPlugin()->CollideWithLine(extended_line, rayDir, sector, data))
                    {
                        Vertex edge1 = data.v0 - data.v1;
                        Vertex edge2 = data.v1 - data.v2;
                        data.normal = CrossProduct(&edge1, &edge2);
                        data.normal.Normalize();
                        data.point = rayDir;
                        data.point.Scale(data.unk);
                        data.point += line.start;
                        //debug_print("Colliding with sector %s\n", FindChecksumName(sector->name));
                        return true;
                    }
                }
            }
        }
        //did not collide with any sectors
        return false;
    }

    __declspec (noalias) bool FindNearestCollision(const RwLine& line, CollData& data, bool update_cache)
    {
        data.unk = FLT_MAX;
        data.collided = false;

        Vertex rayDir = line.end - line.start;


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

        RwLine extended_line;
        Vertex dir = line.end - line.start;
        dir.Normalize();
        dir.Scale(0.5f);

        extended_line.start[X] = line.start[X] - dir.x;
        extended_line.start[Y] = line.start[Y] - dir.y;
        extended_line.start[Z] = line.start[Z] - dir.z;
        extended_line.end[X] = line.end[X] + dir.x;
        extended_line.end[Y] = line.end[Y] + dir.y;
        extended_line.end[Z] = line.end[Z] + dir.z;


        RpWorld* world = RwViewer::Instance()->GetCurrentWorld();
        NxPlugin* plg = world->GetWorldPluginData();
        Collision::Manager* cld_manager = plg->GetManager();

        CollCache* cache = cld_manager->GetIntersectingWorldSectors(line, data, update_cache);
//#pragma omp parallel for throwing a compiler error for some reason...
        for(/*int i=0; i<cache->GetNumSuperSectors(); i++)//*/::SuperSector** sectors = cache->GetSuperSectors(); ::SuperSector* sector = *sectors;  sectors++)
        {
            //::SuperSector* __restrict sector = cache->GetSuperSector(i);
            if (sector == NULL)
                return data.collided;

            //is the line intersecting the sector bbox?
            if (line_bbox.Intersect((*(BBox*)&sector->bboxMax)))
            {
                //Is the sector killed since last cache update?
                if (sector->state != 6)
                {
                    //is the line intersecting any of the sector triangles?
                    if (sector->GetCollisionPlugin()->CollideWithLine(extended_line, rayDir, sector, data))
                    {
                        //debug_print("Colliding with sector %s\n", FindChecksumName(sector->name));
                    }
                }

            }
        }
        if (data.collided)
        {
            Vertex edge1 = data.v0 - data.v1;
            Vertex edge2 = data.v1 - data.v2;
            data.normal = CrossProduct(&edge1, &edge2);
            data.normal.Normalize();
            data.point = rayDir;
            data.point.Scale(data.unk);
            data.point += line.start;
        }
        return data.collided;
    }


    bool TriangleIntersectingSphere(D3DXVECTOR3 A, D3DXVECTOR3 B, D3DXVECTOR3 C, D3DXVECTOR3 P, float R)
    {
        A = A - P;
        B = B - P;
        C = C - P;
        float rr = R * R;
        D3DXVECTOR3 V;
        D3DXVECTOR3 temp1 = (B - A);
        D3DXVECTOR3 temp2 = (C - A);
        D3DXVec3Cross(&V, &temp1, &temp2);
        float d = D3DXVec3Dot(&A, &V);
        float e = D3DXVec3Dot(&V, &V);
        bool sep1 = d * d > rr * e;
        float aa = D3DXVec3Dot(&A, &A);
        float ab = D3DXVec3Dot(&A, &B);
        float ac = D3DXVec3Dot(&A, &C);
        float bb = D3DXVec3Dot(&B, &B);
        float bc = D3DXVec3Dot(&B, &C);
        float cc = D3DXVec3Dot(&C, &C);
        bool sep2 = (aa > rr) && (ab > aa) && (ac > aa);
        bool sep3 = (bb > rr) && (ab > bb) && (bc > bb);
        bool sep4 = (cc > rr) && (ac > cc) && (bc > cc);
        D3DXVECTOR3 AB = B - A;
        D3DXVECTOR3 BC = C - B;
        D3DXVECTOR3 CA = A - C;
        float d1 = ab - aa;
        float d2 = bc - bb;
        float d3 = ac - cc;
        float e1 = D3DXVec3Dot(&AB, &AB);
        float e2 = D3DXVec3Dot(&BC, &BC);
        float e3 = D3DXVec3Dot(&CA, &CA);
        D3DXVECTOR3 Q1 = A * e1 - d1 * AB;
        D3DXVECTOR3 Q2 = B * e2 - d2 * BC;
        D3DXVECTOR3 Q3 = C * e3 - d3 * CA;
        D3DXVECTOR3 QC = C * e1 - Q1;
        D3DXVECTOR3 QA = A * e2 - Q2;
        D3DXVECTOR3 QB = B * e3 - Q3;
        bool sep5 = (D3DXVec3Dot(&Q1, &Q1) > rr * e1 * e1) && (D3DXVec3Dot(&Q1, &QC) > 0);
        bool sep6 = (D3DXVec3Dot(&Q2, &Q2) > rr * e2 * e2) && (D3DXVec3Dot(&Q2, &QA) > 0);
        bool sep7 = (D3DXVec3Dot(&Q3, &Q3) > rr * e3 * e3) && (D3DXVec3Dot(&Q3, &QB) > 0);
        bool separated = sep1 && sep2 && sep3 && sep4 && sep5 && sep6 && sep7;
        return !separated;
    }

    bool TriangleInsideSphere(D3DXVECTOR3 A, D3DXVECTOR3 B, D3DXVECTOR3 C, D3DXVECTOR3 P, float R)
    {
        A = A - P;
        B = B - P;
        C = C - P;
        float rr = R * R;
        D3DXVECTOR3 V;
        D3DXVECTOR3 temp1 = (B - A);
        D3DXVECTOR3 temp2 = (C - A);
        D3DXVec3Cross(&V, &temp1, &temp2);
        float d = D3DXVec3Dot(&A, &V);
        float e = D3DXVec3Dot(&V, &V);
        bool sep1 = d * d > rr * e;

        float aa = D3DXVec3Dot(&A, &A);
        float ab = D3DXVec3Dot(&A, &B);
        float ac = D3DXVec3Dot(&A, &C);
        float bb = D3DXVec3Dot(&B, &B);
        float bc = D3DXVec3Dot(&B, &C);
        float cc = D3DXVec3Dot(&C, &C);
        bool sep2 = (aa > rr) && (ab > aa) && (ac > aa);
        bool sep3 = (bb > rr) && (ab > bb) && (bc > bb);
        bool sep4 = (cc > rr) && (ac > cc) && (bc > cc);
        D3DXVECTOR3 AB = B - A;
        D3DXVECTOR3 BC = C - B;
        D3DXVECTOR3 CA = A - C;
        float d1 = ab - aa;
        float d2 = bc - bb;
        float d3 = ac - cc;
        float e1 = D3DXVec3Dot(&AB, &AB);
        float e2 = D3DXVec3Dot(&BC, &BC);
        float e3 = D3DXVec3Dot(&CA, &CA);
        D3DXVECTOR3 Q1 = A * e1 - d1 * AB;
        D3DXVECTOR3 Q2 = B * e2 - d2 * BC;
        D3DXVECTOR3 Q3 = C * e3 - d3 * CA;
        D3DXVECTOR3 QC = C * e1 - Q1;
        D3DXVECTOR3 QA = A * e2 - Q2;
        D3DXVECTOR3 QB = B * e3 - Q3;
        bool sep5 = (D3DXVec3Dot(&Q1, &Q1) > rr * e1 * e1) && (D3DXVec3Dot(&Q1, &QC) > 0);
        bool sep6 = (D3DXVec3Dot(&Q2, &Q2) > rr * e2 * e2) && (D3DXVec3Dot(&Q2, &QA) > 0);
        bool sep7 = (D3DXVec3Dot(&Q3, &Q3) > rr * e3 * e3) && (D3DXVec3Dot(&Q3, &QB) > 0);
        bool separated = sep1 || sep2 || sep3 || sep4 || sep5 || sep6 || sep7;
        return !separated;
    }
};
