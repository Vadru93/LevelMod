#pragma once
#ifndef SHATTER_H
#define SHATTER_H
#define NO_DEFINES
#include "d3d9.h"
#include "Objects\SuperSectors.h"


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

    float life;
    float gravity;
    float bounce;
    float bounceAmp;

    ~ShatterData()
    {
        debug_print("Deleting ShatterObject %s\n", FindChecksumName(sector->name));
        delete[] verts;
        delete[]pos;
        delete[]vel;
        delete[]matrices;
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

        for (int i = 0; i < numTris; ++i)
        {
            // To move the shatter pieces:
            // 1) subtract position from each vertex
            // 2) rotate
            // 3) update position with velocity
            // 4) add new position to each vertex

            // The matrix holds 3 vectors at once.
            Matrix m;
            m[X].Set(p_v0->x - pos[i][X], p_v0->y - pos[i][Y], p_v0->z - pos[i][Z]);
            m[Y].Set(p_v1->x - pos[i][X], p_v1->y - pos[i][Y], p_v1->z - pos[i][Z]);
            m[Z].Set(p_v2->x - pos[i][X], p_v2->y - pos[i][Y], p_v2->z - pos[i][Z]);

            m[X].Rotate(matrices[i]);
            m[Y].Rotate(matrices[i]);
            m[Z].Rotate(matrices[i]);

            // Update the position and velocity of the shatter piece, dealing with bouncing if necessary.
            UpdateParameters(i, framelength);

            *(D3DXVECTOR3*)&m[X] += pos[i];
            *(D3DXVECTOR3*)&m[Y] += pos[i];
            *(D3DXVECTOR3*)&m[Z] += pos[i];

            p_v0->x = m[X][X]; p_v0->y = m[X][Y]; p_v0->z = m[X][Z];
            p_v1->x = m[Y][X]; p_v1->y = m[Y][Y]; p_v1->z = m[Y][Z];
            p_v2->x = m[Z][X]; p_v2->y = m[Z][Y]; p_v2->z = m[Z][Z];

            p_v0 = (D3DXVECTOR3*)(((BYTE*)p_v0) + (stride * 3));
            p_v1 = (D3DXVECTOR3*)(((BYTE*)p_v1) + (stride * 3));
            p_v2 = (D3DXVECTOR3*)(((BYTE*)p_v2) + (stride * 3));
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
        debug_print("Allocated memory\n");
    }

    void Allocate()
    {
        pos = new D3DXVECTOR3[numTris];
        vel = new D3DXVECTOR3[numTris];
        matrices = new Matrix[numTris];

        if (pos && vel && matrices)
            debug_print("Memory alllocated successfully\n");
        else
            debug_print("Memory did not allocate...\n");
    }

    void UpdateParameters(int index, float timestep)
    {
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
bool subdivide_tri_stack(BYTE** p_write, SuperSector* sector, float targetShatterArea, int& numTris)
{

    static float dividers[4] = { 0.5f, 0.6f, 0.2f, 0.33f };
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
    }
    return true;
}

#endif