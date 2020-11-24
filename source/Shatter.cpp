#include "pch.h"
#include "Shatter.h"

/******************************************************************/
/*                                                                */
/*                                                                */
/******************************************************************/
void sTriSubdivideStack::Reset(void)
{
    m_offset = 0;
    m_block_size = 0;

    memset(m_data, 0x03, TRI_SUBDIVIDE_STACK_SIZE);
}


/******************************************************************/
/*                                                                */
/*                                                                */
/******************************************************************/
void sTriSubdivideStack::Clear(void)
{
    m_offset = 0;
    m_block_size = 0;
}

/******************************************************************/
/*                                                                */
/*                                                                */
/******************************************************************/
void sTriSubdivideStack::Push(void* p_data)
{
    if(m_offset + m_block_size >= TRI_SUBDIVIDE_STACK_SIZE)
        MessageBox(0, "Increase stack size...", "", 0);

    memcpy(m_data + m_offset, p_data, m_block_size);
    m_offset += m_block_size;
}



/******************************************************************/
/*                                                                */
/*                                                                */
/******************************************************************/
void sTriSubdivideStack::Pop(void* p_data)
{
    if(m_offset < m_block_size)
        MessageBox(0, "Something is wrong", "", 0);

    m_offset -= m_block_size;
    memcpy(p_data, m_data + m_offset, m_block_size);
}

/******************************************************************/
/*                                                                */
/*                                                                */
/******************************************************************/
const void* sTriSubdivideStack::Peek(DWORD index)
{
    int offset = index * m_block_size;
    if(offset >= m_offset) _printf("Index %d is beyond end offset %d", index, m_offset);

    return m_data + offset;
}

bool NewShatterScript(CStruct* pStruct, CScript* pScript)
{
    _printf("NewShatterScript...\n");
    // Need to read some params from the script here.
    float		area = 0.0f;
    float		variance = 0.0f;
    float		spread = 0.0f;
    float		life = 0.0f;
    float		bounce = 0.0f;
    float		bounce_amp = 0.0f;
    float		scale = 1.0f;
    Vertex	velocity;

    if (pStruct->ContainsFlag(Checksum("use_skater_vel")))
    {
        pStruct->GetFloat("scale", &scale);

        *(D3DXVECTOR3*)&velocity = *Game::skater->GetVelocity() * scale;
    }
    else
    {
        // Have to supply vel_x, y, and z.
        pStruct->GetFloat(Checksum("vel_x"), &velocity[X]);
        pStruct->GetFloat(Checksum("vel_y"), &velocity[Y]);
        pStruct->GetFloat(Checksum("vel_z"), &velocity[Z]);
    }
    pStruct->GetFloat(Checksum("area"), &area);
    pStruct->GetFloat(Checksum("variance"), &variance);
    pStruct->GetFloat(Checksum("spread"), &spread);
    pStruct->GetFloat(Checksum("life"), &life);
    pStruct->GetFloat(Checksum("bounce"), &bounce);
    pStruct->GetFloat(Checksum("bounce_amp"), &bounce_amp);

    int name = pStruct->GetChecksum(Checksums::Name);

    _printf("Name %s area %f variance %f spread %f life %f bounce %f bounce_amp %f\n", FindChecksumName(name), area, variance, spread, life, bounce, bounce_amp);

    if (name)
    {
        SuperSector* sector = SuperSector::GetSuperSector(name);
        if (sector)
        {
            area = max(1000.0f, area);
            ShatterSetParams(velocity, area, variance, spread, life, bounce, bounce_amp);
            ShatterSuperSector(sector);
        }
        else
            _printf("Couldn't find sector %s\n", FindChecksumName(name));
    }
    else
        _printf("Need name param " __FUNCTION__);
    return true;
}

void ShatterSuperSector(SuperSector* super_sector)
{
    if (super_sector->state & 2)
        return;
    super_sector->state |= 6;
    triSubdivideStack.Clear();

    CSector* sector = super_sector->sector;


    float targetShatterArea[5];
    targetShatterArea[0] = shatterAreaTest;
    targetShatterArea[1] = shatterAreaTest;
    targetShatterArea[2] = shatterAreaTest *= 0.9;
    targetShatterArea[3] = shatterAreaTest *= 1.1f;
    targetShatterArea[4] = shatterAreaTest *= 0.75f;

    if (super_sector->mesh)
    {
        DWORD stride = super_sector->mesh->splits[0].stride;
        //_printf("SuperSector %p normals %p color_offset %p uv_offset %p numVErts %d\n", super_sector, super_sector->normals, super_sector->color_offset, super_sector->uv_offset, super_sector->numVertices);
        //BYTE* p_vert_data = new BYTE[super_sector->numVertices * stride];
        /*for (DWORD i = 0; i < super_sector->numVertices; i++)
        {
            *(D3DXVECTOR3*)&(p_vert_data[i * stride]) = super_sector->vertices[i];
            if (super_sector->normals)
            {
                _printf("Adding normal %d/%d\n", i+1, super_sector->numVertices);
                *(D3DXVECTOR3*)&(p_vert_data[i * stride + 12]) = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
            }
            if (super_sector->color_offset)
            {
                _printf("Adding color %d/%d\n", i + 1, super_sector->numVertices);
                *(DWORD*)&(p_vert_data[i * stride + (DWORD)super_sector->color_offset - (DWORD)super_sector->vertices]) = super_sector->color_offset[i];
            }
            if (super_sector->uv_offset)
            {
                _printf("Adding uv %d/%d\n", i + 1, super_sector->numVertices);
                *(DWORD*)&(p_vert_data[i * stride + (DWORD)super_sector->uv_offset - (DWORD)super_sector->vertices]) = super_sector->uv_offset[i * 2];
                *(DWORD*)&(p_vert_data[i * stride + (DWORD)super_sector->uv_offset - (DWORD)super_sector->vertices + 1]) = super_sector->uv_offset[i * 2 + 1];
            }
        }*/

        _printf("Sector %p stride %X\n", sector, stride);
        for (DWORD m = 0; m < sector->numSplits; ++m)
        {
            CSector::MaterialSplit* mesh = &sector->splits[m];
            Mesh::MaterialSplit* split = &super_sector->mesh->splits[m];

            stride = split->stride;
            BYTE* p_vert_data;
            _printf("Mesh %p\n", mesh);
            if (mesh->numIndices >= 3)
            {

                _printf("Mesh %s Split %d/%d\n", FindChecksumName(super_sector->name), m + 1, sector->numSplits);
                split->vertexBuffer->GetProxyInterface()->Lock(split->baseIndex*stride/*0*/, 0, (void**)&p_vert_data, D3DLOCK_READONLY);
                D3DXVECTOR3 bboxMax = D3DXVECTOR3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
                D3DXVECTOR3 bboxMin = D3DXVECTOR3(FLT_MAX, FLT_MAX, FLT_MAX);

                for (DWORD i = 0; i < split->numVertices; i++)
                {
                    D3DXVECTOR3& v = *(D3DXVECTOR3*)&(p_vert_data[i * stride]);
                    if (bboxMax.x < v.x)
                        bboxMax.x = v.x;
                    if (bboxMin.x > v.x)
                        bboxMin.x = v.x;

                    if (bboxMax.y < v.y)
                        bboxMax.y = v.y;
                    if (bboxMin.y > v.y)
                        bboxMin.y = v.y;

                    if (bboxMax.z < v.z)
                        bboxMax.z = v.z;
                    if (bboxMin.z > v.z)
                        bboxMin.z = v.z;
                }
 
                //_printf("TargetShatterArea %f numSplits %d numIndices %d stide %X\n", shatterAreas[m], sector->numSplits, mesh->numIndices, stride);
                //MessageBox(0, 0, 0, 0);

                // Set the block size for this mesh.
                triSubdivideStack.SetBlockSize(stride);

                // Get a pointer to the renderable data.

                // First scan through each non-degenerate tri, counting them to see how many verts we'll need.
                // We also have to figure the area of the tris here, since we need to calculate the worst case given the requirements for subdivision.
                DWORD valid_tris = 0;
                DWORD index0;
                DWORD index1 = mesh->indices[0];
                DWORD index2 = mesh->indices[1];
                for (DWORD i = 2; i < mesh->numIndices; ++i)
                {
                    // Wrap the indices round.
                    index0 = index1;
                    index1 = index2;
                    index2 = mesh->indices[i];

                    if ((index0 != index1) && (index0 != index2) && (index1 != index2))
                    {
                        ++valid_tris;

                        D3DXVECTOR3* p_vert0 = (D3DXVECTOR3*)(p_vert_data + (stride * index0));
                        D3DXVECTOR3* p_vert1 = (D3DXVECTOR3*)(p_vert_data + (stride * index1));
                        D3DXVECTOR3* p_vert2 = (D3DXVECTOR3*)(p_vert_data + (stride * index2));

                        // Push this tri onto the stack.
                        triSubdivideStack.Push(p_vert0);
                        triSubdivideStack.Push(p_vert1);
                        triSubdivideStack.Push(p_vert2);

                        // Figure the area of this tri.
                        Vertex p(p_vert1->x - p_vert0->x, p_vert1->y - p_vert0->y, p_vert1->z - p_vert0->z);
                        Vertex q(p_vert2->x - p_vert0->x, p_vert2->y - p_vert0->y, p_vert2->z - p_vert0->z);
                        Vertex r((p[Y] * q[Z]) - (q[Y] * p[Z]), (p[Z] * q[X]) - (q[Z] * p[X]), (p[X] * q[Y]) - (q[X] * p[Y]));
                        float area_squared = r.LengthSqr();
                        if (area_squared > targetShatterArea[0])
                        {
                            // We will need to subdivide - each subdivision will result in an area one quarter the previous area
                            // (and thusly the square of the area will be one sixteenth the previous area).
                            int num_extra_tris = 1;
                            while (area_squared > targetShatterArea[0])
                            {
                                num_extra_tris *= 4;
                                area_squared *= (1.0f / 16.0f);
                            }

                            // This original tri will not be added...
                            --valid_tris;

                            // ...however, the subdivided versions will.
                            valid_tris += num_extra_tris;
                        }
                    }
                }

                if (valid_tris == 0)
                {
                    continue;
                }

                _printf("Worst case scenario %d tris\n", valid_tris);

                // Create a tracking structure for this mesh.
                ShatterData* p_shatter = new ShatterData(super_sector, &super_sector->mesh->splits[m], valid_tris*4);
                //p_shatter->numTris = valid_tris;
                BYTE* p_write_vertex = p_shatter->verts;
                DWORD details_index = 0;

                Vertex spread_center = Vertex(shatterVelocity * -shatterSpreadFactor);
                float base_speed = shatterVelocity.Length();

                spread_center += *Game::skater->GetHitPoint();//(bboxMax + bboxMin) * 0.5f;//
                //_printf("Got hit point\n");

                // Process-subdivide the entire stack.
                BYTE* p_copy_vertex = p_write_vertex;
                /*DWORD numTris = subdivide_tri_stack(&p_write_vertex, super_sector, targetShatterArea[0]);// targetShatterArea[rand() % 5]);
                while (numTris)
                {
                    numTris = subdivide_tri_stack(&p_write_vertex, super_sector, targetShatterArea[0]);//  targetShatterArea[rand() % 5]);
                }*/
                while(subdivide_tri_stack(&p_write_vertex, super_sector, targetShatterArea[rand() % 5], p_shatter->numTris));//  );

                _printf("Going to allocate %d tris\n", p_shatter->numTris);
                p_shatter->Allocate();
                //p_shatter->numTris--;
                //p_shatter->numTris = ((DWORD)p_write_vertex - (DWORD)p_copy_vertex) / stride / 3;

                while (p_copy_vertex < p_write_vertex)
                {

                    //_printf("p_copy_vertex %p p_write_vertex %p details_index %d/%d\n", p_copy_vertex, p_write_vertex, details_index+1, valid_tris);
                    D3DXVECTOR3* p_vert0 = (D3DXVECTOR3*)(p_copy_vertex + (stride * 0));
                    D3DXVECTOR3* p_vert1 = (D3DXVECTOR3*)(p_copy_vertex + (stride * 1));
                    D3DXVECTOR3* p_vert2 = (D3DXVECTOR3*)(p_copy_vertex + (stride * 2));

                    // Calculate position as the midpoint of the three vertices per poly.
                    p_shatter->pos[details_index][X] = (p_vert0->x + p_vert1->x + p_vert2->x) * (1.0f / 3.0f);
                    p_shatter->pos[details_index][Y] = (p_vert0->y + p_vert1->y + p_vert2->y) * (1.0f / 3.0f);
                    p_shatter->pos[details_index][Z] = (p_vert0->z + p_vert1->z + p_vert2->z) * (1.0f / 3.0f);

                    // Calculate the vector <velocity> back from the bounding box of the object. Then use this to figure the 'spread' of the
                    // shards by calculating the vector from this position to the center of each shard.
                    float speed = base_speed + (base_speed * ((shatterVelocityVariance * rand()) / RAND_MAX));
                    p_shatter->vel[details_index] = (Vertex(p_shatter->pos[details_index] - spread_center)).Normalize(speed);

                    Vertex axis(-1.0f + (2.0f * (float)rand() / RAND_MAX), -1.0f + (2.0f * (float)rand() / RAND_MAX), -1.0f + (2.0f * (float)rand() / RAND_MAX));
                    axis.Normalize();
                    p_shatter->matrices[details_index].Ident();
                    p_shatter->matrices[details_index].Rotate(axis, 0.1f * ((float)rand() / RAND_MAX));

                    p_copy_vertex += (stride * 3);

                    ++details_index;
                }

                _printf("Going to push...\n");
                shatterObjects.push_back(p_shatter);
                _printf("Pushed...\n");
                super_sector->mesh->splits[m].vertexBuffer->GetProxyInterface()->Unlock();
                _printf("Unlocked\n");
            }
        }
        //_printf("Going to delete p_data\n");
        //delete[] p_vert_data;
    }
    //_printf("Finished\n");
}

bool rendering = false;

void RenderShatterObjects()
{
    rendering = true;
    static DWORD lastFrameCount;
    extern bool restore_matrix;
    if (restore_matrix)
    {
        restore_matrix = false;
        p_current_texstage(D3DTSS_TEXTURETRANSFORMFLAGS) = D3DTTFF_DISABLE;
        p_current_texstage(D3DTSS_TEXCOORDINDEX) = D3DTSS_TCI_PASSTHRU;

        Gfx::pDevice->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
        Gfx::pDevice->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, D3DTSS_TCI_PASSTHRU);
    }
    extern bool restore_filter;

    if (restore_filter)
    {
        restore_filter = false;
        extern DWORD MaxAnisotropy;
        DWORD Value = MaxAnisotropy ? D3DTEXF_ANISOTROPIC : D3DTEXF_LINEAR;

        Gfx::pDevice->SetTextureStageState(0, (D3DTEXTURESTAGESTATETYPE)D3DTSS_MAGFILTER, Value);
        Gfx::pDevice->SetTextureStageState(0, (D3DTEXTURESTAGESTATETYPE)D3DTSS_MINFILTER, Value);
        Gfx::pDevice->SetTextureStageState(0, (D3DTEXTURESTAGESTATETYPE)D3DTSS_MIPFILTER, Value);

    }


    //Make sure we update and render once per frame
    if (lastFrameCount != Gfx::frameCounter)
    {

        Gfx::pDevice->GetRenderTarget(0, &Gfx::world_rendertarget);
        Gfx::pDevice->GetViewport(&Gfx::world_viewport);
        //Make sure textures are set to zero
        Gfx::pDevice->SetTexture(0, NULL);
        p_current_texture(0) = 0;
        Gfx::pDevice->SetTexture(1, NULL);
        Gfx::pDevice->SetTexture(2, NULL);
        Gfx::pDevice->SetTexture(3, NULL);

        lastFrameCount = Gfx::frameCounter;

        DWORD old, old_state;
        Gfx::pDevice->GetFVF(&old);

        old_state = p_current_renderstate(D3DRS_CULLMODE);
        DWORD old_alpha = p_current_renderstate(D3DRS_ALPHABLENDENABLE);
        DWORD old_alpha2 = p_current_renderstate(D3DRS_ALPHATESTENABLE);

        Gfx::pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
        Gfx::pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
        Gfx::pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);


        if (shatterObjects.size())
        {
            float framelength = Game::skater->GetFrameLength() * Gfx::shatter_speed;

            for (DWORD i = 0; i < shatterObjects.size(); i++)
            {
                shatterObjects[i]->life -= framelength;

                if (shatterObjects[i]->life <= 0)
                {
                    delete shatterObjects[i];
                    shatterObjects[i] = NULL;
                    shatterObjects.erase(shatterObjects.begin() + i);
                    i--;
                    continue;
                }

                shatterObjects[i]->Update(framelength);
                shatterObjects[i]->Render();
            }
        }

        Gfx::pDevice->SetRenderState(D3DRS_CULLMODE, old_state);
        Gfx::pDevice->SetFVF(old);
        Gfx::pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, old_alpha);
        Gfx::pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, old_alpha2);
    }
    rendering = false;
}

void UnloadShatterObjects()
{
    while (rendering)
        Sleep(10);
    shatterObjects.clear();
}

__declspec(naked) void Render_Naked()
{
    static DWORD pCall = 0x004F9C80;
    static DWORD pJmp = 0x004F9C0E;
    _asm pushad;
    if(p_render_scene)
        RenderShatterObjects();
    _asm popad;
    _asm call[pCall];
    _asm jmp[pJmp];
}