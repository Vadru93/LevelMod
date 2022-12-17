#pragma once
#ifndef SHATTER_H
#define SHATTER_H
#define NO_DEFINES
#include "d3d9.h"
#include "Objects\SuperSectors.h"
#include "Collision.h"

struct RenderableVertex : Vertex
{
    Vertex n;
    BYTE color[4];
    float uv[2];

    RenderableVertex(float _x, float _y, float _z) : Vertex(_x, _y, _z)
    {

    }
};


//#define p_stride 0x24//*(DWORD*)0x0090B8A8

static const float	DEFAULT_AREA_TEST = 288.0f;
static const float	DEFAULT_VELOCITY_VARIANCE = 0.0f;
static const float	DEFAULT_SPREAD_FACTOR = 1.0f;
static const float	DEFAULT_LIFETIME = 4.0f;
static const float	DEFAULT_BOUNCE = -10000.0f;
static const float	DEFAULT_BOUNCE_AMPLITUDE = 0.8f;

struct sTriSubdivideStack
{
    static const int	TRI_SUBDIVIDE_STACK_SIZE = 16 * 1024 * 4;

    void				Reset(void);
    void				Clear(void);
    bool				IsEmpty(void) { return m_offset == 0; }
    void				SetBlockSize(int size) { m_block_size = size; }
    int					GetBlockSize(void) { return m_block_size; }
    void				Pop(void* p_data);
    void				Push(void* p_data);
    const void* Peek(DWORD index);

private:
    int					m_offset;
    int					m_block_size;
    char				m_data[TRI_SUBDIVIDE_STACK_SIZE];
};

Vertex			shatterVelocity;
float				shatterAreaTest = DEFAULT_AREA_TEST * DEFAULT_AREA_TEST;
float				shatterVelocityVariance = DEFAULT_VELOCITY_VARIANCE;
float				shatterSpreadFactor = DEFAULT_SPREAD_FACTOR;
float				shatterLifetime = DEFAULT_LIFETIME;
float				shatterBounce = DEFAULT_BOUNCE;
float				shatterBounceAmplitude = DEFAULT_BOUNCE_AMPLITUDE;
sTriSubdivideStack	triSubdivideStack;
float shatterAreas[3 * 1024];

struct ShatterData
{
    Mesh::MaterialSplit* split;
    SuperSector* sector;
    BYTE* verts;
    D3DXVECTOR3* pos;
    D3DXVECTOR3* vel;
    Matrix* matrices;
    //BYTE* tris;
    int numTris;
    D3DXVECTOR3 shatter_point;
    DWORD numShatteredTris;
    int* shatteredTris;

    float life;
    float gravity;
    float bounce;
    float bounceAmp;

    D3DXVECTOR3* old_pos;
    bool* collided;
    Collision::CollData data;
    Collision::CollCache cache;

    std::vector<RenderableVertex> shattered_pieces;

    ~ShatterData()
    {
        debug_print("Deleting ShatterObject %s\n", FindChecksumName(sector->name));
        delete[] verts;
        delete[]pos;
        delete[]vel;
        delete[]matrices;
        delete[]old_pos;
        delete[]collided;
        //delete[]tris;
        ZeroMemory(this, sizeof(ShatterData));
    }

    void Render()
    {
        //printf("Split %p Material %X\n", split, split->material);
        if (split->material && split->material->texture)
        {
            //debug_print("Going to submit material\n");
            split->material->Submit();

            //debug_print("VertexShader %X stride %X\n", split->vertexShader, split->stride);
            Gfx::pDevice->SetFVF(split->vertexShader);

            /*debug_print("VBuffer %p IBuffer %p stride %X primCount %d vertCount %d\n", split->vertexBuffer->GetProxyInterface(), split->indexBuffer->GetProxyInterface(), split->stride, split->numIndices, split->numVertices);
            Gfx::pDevice->SetStreamSource(0, split->vertexBuffer->GetProxyInterface(), 0, split->stride);
            Gfx::pDevice->SetIndices(split->indexBuffer->GetProxyInterface());
            Gfx::pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, split->baseIndex, 0, split->numVertices, 0, split->numIndices);*/
            //debug_print("Going to Draw\n");
            Gfx::pDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, numTris, verts, split->stride);
            /*if(shattered_pieces.size())
                Gfx::pDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST, shattered_pieces.size() / 3, &shattered_pieces.front(), split->stride);*/
            //debug_print("Finished rendering, sucessfully :)\n");
        }
    }

    void Update(float framelength)
    {
        //debug_print("NewShatter: Going to update %s (delta:%f)\n", FindChecksumName(sector->name), framelength);
        BYTE* p_vert_data = verts;
        DWORD stride = split->stride;

        // Load up initial three vertex pointers.
        D3DXVECTOR3* p_v0 = (D3DXVECTOR3*)(p_vert_data);
        D3DXVECTOR3* p_v1 = (D3DXVECTOR3*)(p_vert_data + stride);
        D3DXVECTOR3* p_v2 = (D3DXVECTOR3*)(p_vert_data + (2 * stride));

//for some reason this breaks the code...
//#pragma omp parallel for
        for (int i = 0; i < numTris; ++i)
        {
            if (!collided[i])
            {
                // To move the shatter pieces:
                // 1) subtract position from each vertex
                // 2) rotate
                // 3) update position with velocity
                // 4) add new position to each vertex

                // The matrix holds 3 vectors at once.
                /*D3DXVECTOR3* p_v0 = (D3DXVECTOR3*)(((BYTE*)base_p_v0) + ((stride * 3) * shatteredTris[i]));
                D3DXVECTOR3* p_v1 = (D3DXVECTOR3*)(((BYTE*)base_p_v1) + ((stride * 3) * shatteredTris[i]));
                D3DXVECTOR3* p_v2 = (D3DXVECTOR3*)(((BYTE*)base_p_v2) + ((stride * 3) * shatteredTris[i]));
                if (true)
                {*/
                Matrix m;
                m[X].Set(p_v0->x - pos[i][X], p_v0->y - pos[i][Y], p_v0->z - pos[i][Z]);
                m[Y].Set(p_v1->x - pos[i][X], p_v1->y - pos[i][Y], p_v1->z - pos[i][Z]);
                m[Z].Set(p_v2->x - pos[i][X], p_v2->y - pos[i][Y], p_v2->z - pos[i][Z]);

                m[X].Rotate(matrices[i]);
                m[Y].Rotate(matrices[i]);
                m[Z].Rotate(matrices[i]);

                // Update the position and velocity of the shatter piece, dealing with bouncing if necessary.
                if (!UpdateParameters(i, framelength, p_v0, p_v1, p_v2))
                {
                    *p_v0 = *(D3DXVECTOR3*)&m[X];// +pos[i];
                    *p_v1 = *(D3DXVECTOR3*)&m[Y];// +pos[i];
                    *p_v2 = *(D3DXVECTOR3*)&m[Z];// +pos[i];
                    Vertex normal = CalculateNormal(p_v0, p_v1, p_v2);
                    debug_print("normal %f %f %f data %f %f %f\n", normal.x, normal.y, normal.z, data.normal.x, data.normal.y, data.normal.z);
                    float angle = fabsf(data.normal.y - normal.y);
                    if (angle > 0.15f && i % 5 == 0)
                    {
                        RenderableVertex v00 = RenderableVertex(p_v0->x, p_v0->y, p_v0->z);
                        RenderableVertex v01(p_v1->x, p_v1->y, p_v1->z);
                        RenderableVertex v02(p_v2->x, p_v2->y, p_v2->z);
                        RenderableVertex v0(p_v0->x + ((p_v1->x - p_v0->x) * 0.5f), p_v0->y + ((p_v1->y - p_v0->y) * 0.5f), p_v0->z + ((p_v1->z - p_v0->z) * 0.5f));
                        RenderableVertex v1(p_v1->x + ((p_v2->x - p_v1->x) * 0.5f), p_v1->y + ((p_v2->y - p_v1->y) * 0.5f), p_v1->z + ((p_v2->z - p_v1->z) * 0.5f));
                        RenderableVertex v2(p_v2->x + ((p_v0->x - p_v2->x) * 0.5f), p_v2->y + ((p_v0->y - p_v2->y) * 0.5f), p_v2->z + ((p_v0->z - p_v2->z) * 0.5f));

                        DWORD uv_offset = 12;
                        //Need to learn how to check if have normals...
                        if (sector->color_offset)
                        {
                            uv_offset += 4;
                            DWORD color_offset = 12;
                            BYTE* p_v0col = (BYTE*)(p_v0 + color_offset);
                            BYTE* p_v1col = (BYTE*)(p_v1 + color_offset);
                            BYTE* p_v2col = (BYTE*)(p_v2 + color_offset);

                            for (int i = 0; i < 4; ++i)
                            {
                                v0.color[i] = p_v0col[i] + (((int)p_v1col[i] - (int)p_v0col[i]) / 2);
                                v1.color[i] = p_v1col[i] + (((int)p_v2col[i] - (int)p_v1col[i]) / 2);
                                v2.color[i] = p_v2col[i] + (((int)p_v0col[i] - (int)p_v2col[i]) / 2);
                            }
                        }

                        // Deal with uv0 (not always present).
                        if (sector->uv_offset)
                        {
                            float* p_v0uv = (float*)(p_v0 + uv_offset);
                            float* p_v1uv = (float*)(p_v1 + uv_offset);
                            float* p_v2uv = (float*)(p_v2 + uv_offset);

                            for (int i = 0; i < 2; ++i)
                            {
                                v0.uv[i] = p_v0uv[i] + ((p_v1uv[i] - p_v0uv[i]) * 0.5f);
                                v1.uv[i] = p_v1uv[i] + ((p_v2uv[i] - p_v1uv[i]) * 0.5f);
                                v2.uv[i] = p_v2uv[i] + ((p_v0uv[i] - p_v2uv[i]) * 0.5f);
                            }
                        }

                        // Push the four new tris onto the stack.
                        v00 += pos[i];
                        v01 += pos[i];
                        v02 += pos[i];
                        v0 += pos[i];
                        v1 += pos[i];
                        v2 += pos[i];

                        shattered_pieces.push_back(v00);
                        shattered_pieces.push_back(v0);
                        shattered_pieces.push_back(v2);

                        shattered_pieces.push_back(v0);
                        shattered_pieces.push_back(v01);
                        shattered_pieces.push_back(v1);

                        shattered_pieces.push_back(v0);
                        shattered_pieces.push_back(v1);
                        shattered_pieces.push_back(v2);

                        shattered_pieces.push_back(v2);
                        shattered_pieces.push_back(v1);
                        shattered_pieces.push_back(v02);

                        *p_v0 = Vertex(0, 0, 0);
                        *p_v1 = Vertex(0, 0, 0);
                        *p_v2 = Vertex(0, 0, 0);
                    }
                    else
                    {
                        *p_v0 += pos[i];
                        *p_v1 += pos[i];
                        *p_v2 += pos[i];
                    }
                }
                else
                {
                    *(D3DXVECTOR3*)&m[X] += pos[i];
                    *(D3DXVECTOR3*)&m[Y] += pos[i];
                    *(D3DXVECTOR3*)&m[Z] += pos[i];

                    Vertex temp0, temp1, temp2;
                    p_v0->x = m[X][X]; p_v0->y = m[X][Y]; p_v0->z = m[X][Z];
                    p_v1->x = m[Y][X]; p_v1->y = m[Y][Y]; p_v1->z = m[Y][Z];
                    p_v2->x = m[Z][X]; p_v2->y = m[Z][Y]; p_v2->z = m[Z][Z];
                }
            }

                p_v0 = (D3DXVECTOR3*)(((BYTE*)p_v0) + (stride * 3));
                p_v1 = (D3DXVECTOR3*)(((BYTE*)p_v1) + (stride * 3));
                p_v2 = (D3DXVECTOR3*)(((BYTE*)p_v2) + (stride * 3));
            //}
        }

        // Also process normals if they exist.
        /*if (sector->normals)
        {
            p_v0 = sector->normals;
            p_v1 = sector->normals + 1;
            p_v2 = sector->normals + 2;

            for (int i = 0; i < numTris; ++i)
            {
                // The matrix holds 3 vectors at once.
                Matrix m;
                m[X].Set(p_v0->x, p_v0->y, p_v0->z);
                m[Y].Set(p_v1->x, p_v1->y, p_v1->z);
                m[Z].Set(p_v2->x, p_v2->y, p_v2->z);

                m[X].Rotate(matrices[i]);
                m[Y].Rotate(matrices[i]);
                m[Z].Rotate(matrices[i]);

                p_v0->x = m[X][X]; p_v0->y = m[X][Y]; p_v0->z = m[X][Z];
                p_v1->x = m[Y][X]; p_v1->y = m[Y][Y]; p_v1->z = m[Y][Z];
                p_v2->x = m[Z][X]; p_v2->y = m[Z][Y]; p_v2->z = m[Z][Z];

                p_v0 = (D3DXVECTOR3*)(((BYTE*)p_v0) + (stride * 3));
                p_v1 = (D3DXVECTOR3*)(((BYTE*)p_v1) + (stride * 3));
                p_v2 = (D3DXVECTOR3*)(((BYTE*)p_v2) + (stride * 3));
            }
        }*/
        //debug_print("Finished updating, now going to render..\n");
    }

    ShatterData(SuperSector* _sector, Mesh::MaterialSplit* _split, int numTriangles)
    {
        /*pos = new D3DXVECTOR3[numTriangles];
        vel = new D3DXVECTOR3[numTriangles];
        matrices = new Matrix[numTriangles];
        numTris = 0;

        tris = new BYTE[numTriangles * 3];
        */
        pos = NULL;
        vel = NULL;
        matrices = NULL;

        verts = new BYTE[numTriangles * 3 * _split->stride];
        numTris = 0;
        sector = _sector;
        split = _split;

        gravity = Gfx::shatter_gravity;
        life = shatterLifetime * Gfx::shatter_life_factor;
        bounce = shatterBounce;
        bounceAmp = shatterBounceAmplitude;

        BBox bbox = *sector->GetBBox();
        RwLine line;
        line.start = bbox.min;
        line.end = bbox.max;
        line.end.y -= 4500.f;
        line.start.x -= 50.0f;
        line.start.z -= 50.0f;
        line.end.x += 50.0f;
        line.end.z += 50.0f;

        cache.Update(line);
        data = Collision::CollData(&cache);


        debug_print("Allocated memory\n");
    }

    void Allocate()
    {
        pos = new D3DXVECTOR3[numTris];
        vel = new D3DXVECTOR3[numTris];
        matrices = new Matrix[numTris];
        old_pos = new D3DXVECTOR3[numTris];
        collided = new bool[numTris];
        ZeroMemory(collided, numTris);

        if (pos && vel && matrices)
            debug_print("Memory alllocated successfully\n");
        else
            debug_print("Memory did not allocate...\n");
    }

    bool UpdateParameters(int index, float timestep, const D3DXVECTOR3* const __restrict v0, const D3DXVECTOR3* const __restrict v1, const D3DXVECTOR3* const __restrict v2)
    {
        old_pos[index] = pos[index];

        pos[index] += vel[index] * timestep;

        if ((pos[index][Y] < bounce) && (vel[index][Y] < 0.0f))
        {
            // Hit the floor. Bounce back up.
            pos[index][Y] = bounce + (bounce - pos[index][Y]);
            pos[index][Y] = vel[index][Y] * -bounceAmp;

            // And figure a new rotation matrix.
            Vertex axis(-1.0f + (2.0f * (float)rand() / RAND_MAX), -1.0f + (2.0f * (float)rand() / RAND_MAX), -1.0f + (2.0f * (float)rand() / RAND_MAX));
            axis.Normalize();
            matrices[index].Ident();
            matrices[index].Rotate(axis, 0.1f * ((float)rand() / RAND_MAX));
        }

        vel[index][Y] -= gravity * timestep;

        if (old_pos[index] != pos[index])
        {
            RwLine line;
            line.start = old_pos[index];
            line.end = pos[index];

            if (Collision::FindFirstCollision(line, data) && data.normal.y > 0.1f)
            {
                /*matrices[index].Ident();
                matrices[index].Rotate(data.normal, 90.0f);*/
                collided[index] = true;
                pos[index] = data.point;
                return false;
            }
            Vertex old_start = line.start;
            line.start.x += v0->x;
            line.start.y += v0->y;
            line.start.z += v0->z;
            Vertex old_end = line.end;
            line.end.x += v0->x;
            line.end.y += v0->y;
            line.end.z += v0->z;
            if (Collision::FindFirstCollision(line, data) && data.normal.y > 0.1f)
            {
                /*matrices[index].Ident();
                matrices[index].Rotate(data.normal, 90.0f);*/
                collided[index] = true;
                pos[index] = data.point;
                return false;
            }
            line.start = old_start;
            line.start.x += v1->x;
            line.start.y += v1->y;
            line.start.z += v1->z;
            line.end = old_end;
            line.end.x += v1->x;
            line.end.y += v1->y;
            line.end.z += v1->z;
            if (Collision::FindFirstCollision(line, data) && data.normal.y > 0.1f)
            {
                /*matrices[index].Ident();
                matrices[index].Rotate(data.normal, 90.0f);*/
                collided[index] = true;
                pos[index] = data.point;
                return false;
            }
            line.start = old_start;
            line.start.x += v2->x;
            line.start.y += v2->y;
            line.start.z += v2->z;
            line.end = old_end;
            line.end.x += v2->x;
            line.end.y += v2->y;
            line.end.z += v2->z;
            if (Collision::FindFirstCollision(line, data) && data.normal.y > 0.1f)
            {
                /*matrices[index].Ident();
                matrices[index].Rotate(data.normal, 90.0f);*/
                collided[index] = true;
                pos[index] = data.point;
                return false;
            }
        }
        return true;
    }
};

void ShatterSetParams(Vertex& velocity, float area_test, float velocity_variance, float spread_factor, float lifetime, float bounce, float bounce_amplitude)
{
    //Replay::WriteShatterParams(velocity, area_test, velocity_variance, spread_factor, lifetime, bounce, bounce_amplitude);

    shatterVelocity = velocity;
    shatterAreaTest = (area_test == 0.0f) ? (DEFAULT_AREA_TEST * DEFAULT_AREA_TEST) : (area_test * area_test);
    shatterVelocityVariance = (velocity_variance == 0.0f) ? DEFAULT_VELOCITY_VARIANCE : velocity_variance;
    shatterSpreadFactor = (spread_factor == 0.0f) ? DEFAULT_SPREAD_FACTOR : spread_factor;
    shatterLifetime = (lifetime == 0.0f) ? DEFAULT_LIFETIME : lifetime;
    shatterBounce = (bounce == 0.0f) ? DEFAULT_BOUNCE : bounce;
    shatterBounceAmplitude = (bounce_amplitude == 0.0f) ? DEFAULT_BOUNCE_AMPLITUDE : bounce_amplitude;
}


std::vector<ShatterData*> shatterObjects;
std::vector<SuperSector*> PointyObjects;
std::vector<ColouredVertex> bbox_rails;


void ShatterSuperSector(SuperSector* super_sector);

bool NewShatterScript(CStruct* pStruct, CScript* pScript);




/******************************************************************/
/*                                                                */
/*                                                                */
/******************************************************************/
bool subdivide_tri_stack(BYTE** p_write, SuperSector* sector, float targetShatterArea, int& numTris, int valid_tris)
{

    static float dividers[3] = { 0.5f, 0.75f, 0.45f };
    float divider = dividers[rand() % 3];
    // Three temporary buffers.
    static BYTE v0[256];
    static BYTE v1[256];
    static BYTE v2[256];

    // Three more temporary buffers.
    static BYTE i01[256];
    static BYTE i12[256];
    static BYTE i20[256];

    // If there are elements on the stack, pop off the top three vertices and subdivide if necessary.
    if (triSubdivideStack.IsEmpty())
    {
        debug_print("triStack empty\n");
        return false;
    }

    D3DXVECTOR3* p_v0 = (D3DXVECTOR3*)v0;
    D3DXVECTOR3* p_v1 = (D3DXVECTOR3*)v1;
    D3DXVECTOR3* p_v2 = (D3DXVECTOR3*)v2;

    // Stack is LIFO, so Pop() off in reverse order.
    triSubdivideStack.Pop(p_v2);
    triSubdivideStack.Pop(p_v1);
    triSubdivideStack.Pop(p_v0);

    // Calculate the area of this tri.
    Vertex p(p_v1->x - p_v0->x, p_v1->y - p_v0->y, p_v1->z - p_v0->z);
    Vertex q(p_v2->x - p_v0->x, p_v2->y - p_v0->y, p_v2->z - p_v0->z);
    Vertex r((p[Y] * q[Z]) - (q[Y] * p[Z]), (p[Z] * q[X]) - (q[Z] * p[X]), (p[X] * q[Y]) - (q[X] * p[Y]));
    float area_squared = r.LengthSqr();

    if (area_squared > targetShatterArea)
    {
        debug_print("subdividing tri\n");
        // We need to subdivide this tri. Calculate the three intermediate points.
        int block_size = triSubdivideStack.GetBlockSize();

        memcpy(i01, v0, block_size);
        memcpy(i12, v1, block_size);
        memcpy(i20, v2, block_size);

        // Deal with positions (always present).
        ((D3DXVECTOR3*)i01)->x = p_v0->x + ((p_v1->x - p_v0->x) * divider);
        ((D3DXVECTOR3*)i01)->y = p_v0->y + ((p_v1->y - p_v0->y) * divider);
        ((D3DXVECTOR3*)i01)->z = p_v0->z + ((p_v1->z - p_v0->z) * divider);

        ((D3DXVECTOR3*)i12)->x = p_v1->x + ((p_v2->x - p_v1->x) * divider);
        ((D3DXVECTOR3*)i12)->y = p_v1->y + ((p_v2->y - p_v1->y) * divider);
        ((D3DXVECTOR3*)i12)->z = p_v1->z + ((p_v2->z - p_v1->z) * divider);

        ((D3DXVECTOR3*)i20)->x = p_v2->x + ((p_v0->x - p_v2->x) * divider);
        ((D3DXVECTOR3*)i20)->y = p_v2->y + ((p_v0->y - p_v2->y) * divider);
        ((D3DXVECTOR3*)i20)->z = p_v2->z + ((p_v0->z - p_v2->z) * divider);

        //Need to learn how to check if have normals...
        if (sector->color_offset)
        {
            DWORD color_offset = (DWORD)sector->color_offset - (DWORD)sector->vertices;
            BYTE* p_v0col = (BYTE*)(v0 + color_offset);
            BYTE* p_v1col = (BYTE*)(v1 + color_offset);
            BYTE* p_v2col = (BYTE*)(v2 + color_offset);
            BYTE* p_i01col = (BYTE*)(i01 + color_offset);
            BYTE* p_i12col = (BYTE*)(i12 + color_offset);
            BYTE* p_i20col = (BYTE*)(i20 + color_offset);

            for (int i = 0; i < 4; ++i)
            {
                p_i01col[i] = p_v0col[i] + (((int)p_v1col[i] - (int)p_v0col[i]) / 2);
                p_i12col[i] = p_v1col[i] + (((int)p_v2col[i] - (int)p_v1col[i]) / 2);
                p_i20col[i] = p_v2col[i] + (((int)p_v0col[i] - (int)p_v2col[i]) / 2);
            }
        }

        // Deal with uv0 (not always present).
        if (sector->uv_offset)
        {
            DWORD uv_offset = (DWORD)sector->uv_offset - (DWORD)sector->vertices;

            float* p_v0uv = (float*)(v0 + uv_offset);
            float* p_v1uv = (float*)(v1 + uv_offset);
            float* p_v2uv = (float*)(v2 + uv_offset);
            float* p_i01uv = (float*)(i01 + uv_offset);
            float* p_i12uv = (float*)(i12 + uv_offset);
            float* p_i20uv = (float*)(i20 + uv_offset);

            for (int i = 0; i < 2; ++i)
            {
                p_i01uv[i] = p_v0uv[i] + ((p_v1uv[i] - p_v0uv[i]) * divider);
                p_i12uv[i] = p_v1uv[i] + ((p_v2uv[i] - p_v1uv[i]) * divider);
                p_i20uv[i] = p_v2uv[i] + ((p_v0uv[i] - p_v2uv[i]) * divider);
            }
        }

        // Push the four new tris onto the stack.
        triSubdivideStack.Push(v0);
        triSubdivideStack.Push(i01);
        triSubdivideStack.Push(i20);

        triSubdivideStack.Push(i01);
        triSubdivideStack.Push(v1);
        triSubdivideStack.Push(i12);

        triSubdivideStack.Push(i01);
        triSubdivideStack.Push(i12);
        triSubdivideStack.Push(i20);

        triSubdivideStack.Push(i20);
        triSubdivideStack.Push(i12);
        triSubdivideStack.Push(v2);

    }
    else
    {
        // Don't need to subdivide this tri.
        int block_size = triSubdivideStack.GetBlockSize();

        // Just copy the tri into the next available slot.
        memcpy(*p_write, v0, block_size);
        *p_write += block_size;
        memcpy(*p_write, v1, block_size);
        *p_write += block_size;
        memcpy(*p_write, v2, block_size);
        *p_write += block_size;
        numTris++;
        if (numTris > valid_tris)
            return false;
    }
    return true;
}

#endif