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
    if (m_offset + m_block_size >= TRI_SUBDIVIDE_STACK_SIZE)
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
    if (m_offset < m_block_size)
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
    if (offset >= m_offset) debug_print("Index %d is beyond end offset %d", index, m_offset);

    return m_data + offset;
}

bool NewShatterScript(CStruct* pStruct, CScript* pScript)
{
    debug_print("NewShatterScript...\n");
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

    debug_print("Name %s area %f variance %f spread %f life %f bounce %f bounce_amp %f\n", FindChecksumName(name), area, variance, spread, life, bounce, bounce_amp);

    if (name)
    {
        SuperSector* sector = SuperSector::GetSuperSector(name);
        if (sector)
        {
            area = min(1000.0f, area);
            area = max(5000.0f, area);
            ShatterSetParams(velocity, area, variance, spread, life, bounce, bounce_amp);
            ShatterSuperSector(sector);
        }
        else
        {
            debug_print("Couldn't find sector %s\n", FindChecksumName(name));
            return QScript::ShatterScript(pStruct, pScript);
        }
    }
    else
        debug_print("Need name param " __FUNCTION__);
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
    targetShatterArea[2] = shatterAreaTest *= 0.9f;
    targetShatterArea[3] = shatterAreaTest *= 1.1f;
    targetShatterArea[4] = shatterAreaTest *= 0.75f;

    if (super_sector->mesh)
    {
        DWORD stride = super_sector->mesh->splits[0].stride;
        //debug_print("SuperSector %p normals %p color_offset %p uv_offset %p numVErts %d\n", super_sector, super_sector->normals, super_sector->color_offset, super_sector->uv_offset, super_sector->numVertices);
        //BYTE* p_vert_data = new BYTE[super_sector->numVertices * stride];
        /*for (DWORD i = 0; i < super_sector->numVertices; i++)
        {
            *(D3DXVECTOR3*)&(p_vert_data[i * stride]) = super_sector->vertices[i];
            if (super_sector->normals)
            {
                debug_print("Adding normal %d/%d\n", i+1, super_sector->numVertices);
                *(D3DXVECTOR3*)&(p_vert_data[i * stride + 12]) = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
            }
            if (super_sector->color_offset)
            {
                debug_print("Adding color %d/%d\n", i + 1, super_sector->numVertices);
                *(DWORD*)&(p_vert_data[i * stride + (DWORD)super_sector->color_offset - (DWORD)super_sector->vertices]) = super_sector->color_offset[i];
            }
            if (super_sector->uv_offset)
            {
                debug_print("Adding uv %d/%d\n", i + 1, super_sector->numVertices);
                *(DWORD*)&(p_vert_data[i * stride + (DWORD)super_sector->uv_offset - (DWORD)super_sector->vertices]) = super_sector->uv_offset[i * 2];
                *(DWORD*)&(p_vert_data[i * stride + (DWORD)super_sector->uv_offset - (DWORD)super_sector->vertices + 1]) = super_sector->uv_offset[i * 2 + 1];
            }
        }*/

        debug_print("Sector %p stride %X\n", sector, stride);
        for (DWORD m = 0; m < sector->numSplits; ++m)
        {
            CSector::MaterialSplit* mesh = &sector->splits[m];
            Mesh::MaterialSplit* split = &super_sector->mesh->splits[m];

            stride = split->stride;
            BYTE* p_vert_data;
            debug_print("Mesh %p\n", mesh);
            if (mesh->numIndices >= 3)
            {

                debug_print("Mesh %s Split %d/%d\n", FindChecksumName(super_sector->name), m + 1, sector->numSplits);
                split->vertexBuffer->GetProxyInterface()->Lock(split->baseIndex * stride/*0*/, 0, (void**)&p_vert_data, D3DLOCK_READONLY);
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

                //debug_print("TargetShatterArea %f numSplits %d numIndices %d stide %X\n", shatterAreas[m], sector->numSplits, mesh->numIndices, stride);
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
                        if (area_squared > targetShatterArea[4])
                        {
                            // We will need to subdivide - each subdivision will result in an area one quarter the previous area
                            // (and thusly the square of the area will be one sixteenth the previous area).
                            int num_extra_tris = 1;
                            while (area_squared > targetShatterArea[4])
                            {
                                num_extra_tris *= 4;
                                area_squared *= 0.25f;// (1.0f / 16.0f);
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

                debug_print("Worst case scenario %d tris\n", valid_tris);

                // Create a tracking structure for this mesh.
                ShatterData* p_shatter = new ShatterData(super_sector, &super_sector->mesh->splits[m], valid_tris + 1);
                //p_shatter->numTris = valid_tris;
                BYTE* p_write_vertex = p_shatter->verts;
                DWORD details_index = 0;

                Vertex spread_center = Vertex(shatterVelocity * -shatterSpreadFactor);
                float base_speed = shatterVelocity.Length();

                spread_center += *Game::skater->GetHitPoint();//(bboxMax + bboxMin) * 0.5f;//
                //debug_print("Got hit point\n");

                // Process-subdivide the entire stack.
                BYTE* p_copy_vertex = p_write_vertex;
                /*DWORD numTris = subdivide_tri_stack(&p_write_vertex, super_sector, targetShatterArea[0]);// targetShatterArea[rand() % 5]);
                while (numTris)
                {
                    numTris = subdivide_tri_stack(&p_write_vertex, super_sector, targetShatterArea[0]);//  targetShatterArea[rand() % 5]);
                }*/
                while (subdivide_tri_stack(&p_write_vertex, super_sector, targetShatterArea[4/*rand() % 5*/], p_shatter->numTris, valid_tris));//  );

                debug_print("Going to allocate %d tris\n", p_shatter->numTris);
                p_shatter->Allocate();
                //p_shatter->numTris--;
                //p_shatter->numTris = ((DWORD)p_write_vertex - (DWORD)p_copy_vertex) / stride / 3;

                while (p_copy_vertex < p_write_vertex)
                {

                    //debug_print("p_copy_vertex %p p_write_vertex %p details_index %d/%d\n", p_copy_vertex, p_write_vertex, details_index+1, valid_tris);
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

                debug_print("Going to push...\n");
                p_shatter->shatter_point = *Game::skater->GetPosition();
                /*p_shatter->shatteredTris = new int[p_shatter->numTris];
                BYTE* p_vert_data = p_shatter->verts;
                DWORD stride = p_shatter->split->stride;
                D3DXVECTOR3* p_v0 = (D3DXVECTOR3*)(p_vert_data);
                D3DXVECTOR3* p_v1 = (D3DXVECTOR3*)(p_vert_data + stride);
                D3DXVECTOR3* p_v2 = (D3DXVECTOR3*)(p_vert_data + (2 * stride));
                p_shatter->numShatteredTris = 0;

                for (int i = 0; i < p_shatter->numTris; i++)
                {
                    //if (Collision::TriangleIntersectingSphere(*p_v0, *p_v1, *p_v2, p_shatter->shatter_point, 200.0f))
                    
                    if(fabsf((*(Vertex*)&p_shatter->shatter_point).Distance(*(Vertex*)p_v0)) < 200.0f || fabsf((*(Vertex*)&p_shatter->shatter_point).Distance(*(Vertex*)p_v1)) < 200.0f || fabsf((*(Vertex*)&p_shatter->shatter_point).Distance(*(Vertex*)p_v2)) < 200.0f)
                    {
                        p_shatter->shatteredTris[p_shatter->numShatteredTris] = i;
                        p_shatter->numShatteredTris++;

                        p_v0 = (D3DXVECTOR3*)(((BYTE*)p_v0) + (stride * 3));
                        p_v1 = (D3DXVECTOR3*)(((BYTE*)p_v1) + (stride * 3));
                        p_v2 = (D3DXVECTOR3*)(((BYTE*)p_v2) + (stride * 3));
                    }
                }*/
                shatterObjects.push_back(p_shatter);
                debug_print("Pushed...\n");
                super_sector->mesh->splits[m].vertexBuffer->GetProxyInterface()->Unlock();
                debug_print("Unlocked\n");
            }
        }
        //debug_print("Going to delete p_data\n");
        //delete[] p_vert_data;
    }
    //debug_print("Finished\n");
}

bool rendering = false;

/*void UpdateShatterObjects()
{
    rendering = true;
    static DWORD lastFrameCount;

    if (lastFrameCount != Gfx::frameCounter && Game::skater)
    {
        lastFrameCount = Gfx::frameCounter;

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
        }
    }
    rendering = false;
}*/

void SetupBlendedMode(WORD flags);
bool ShouldBlend(SuperSector* sector)
{
#ifdef _DEBUG
    if (sector->flags & SuperSector::Blend::Enabled)
    {
        SetupBlendedMode(sector->flags);
        return true;
    }
#endif
    return false;
}

DWORD alpha_enabled, old_factor;

void SetupBlendedMode(WORD flag)
{
#ifdef _DEBUG
    Gfx::pDevice->GetRenderState(D3DRS_BLENDFACTOR, &old_factor);

    switch (flag & SuperSector::Blend::Enabled)
    {
    case SuperSector::Blend::Red:
        Gfx::pDevice->SetRenderState(D3DRS_BLENDFACTOR, D3DXCOLOR(255, 0, 0, 255));
        break;
    case SuperSector::Blend::Green:
        Gfx::pDevice->SetRenderState(D3DRS_BLENDFACTOR, D3DXCOLOR(0, 255, 0, 255));
        break;
    case SuperSector::Blend::Blue:
        Gfx::pDevice->SetRenderState(D3DRS_BLENDFACTOR, D3DXCOLOR(0, 0, 255, 255));
        break;
    case SuperSector::Blend::Mix:
        Gfx::pDevice->SetRenderState(D3DRS_BLENDFACTOR, D3DXCOLOR(0, 255, 128, 255));
        break;

    }

    Gfx::pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
#endif
}

void RestoreBlendedMode()
{
#ifdef _DEBUG
    Gfx::pDevice->SetRenderState(D3DRS_BLENDFACTOR, old_factor);
#endif
}

extern std::vector<SuperSector*> hit_sectors;
void RenderBlendedSectors()
{
#ifdef _DEBUG
    DWORD old_ref, z_enabled, z_func, src_blend, alpha_enabled;
    Gfx::pDevice->GetRenderState(D3DRS_ALPHAREF, &old_ref);
    Gfx::pDevice->GetRenderState(D3DRS_SRCBLEND, &src_blend);
    Gfx::pDevice->GetRenderState(D3DRS_ZENABLE, &z_enabled);
    Gfx::pDevice->GetRenderState(D3DRS_ZFUNC, &z_func);
    Gfx::pDevice->GetRenderState(D3DRS_ALPHABLENDENABLE, &alpha_enabled);
    Gfx::pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    Gfx::pDevice->SetRenderState(D3DRS_ALPHAREF, 8);

    Gfx::pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
    Gfx::pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

    Gfx::pDevice->SetTexture(0, NULL);
    p_current_texture(0) = 0;
    Gfx::pDevice->SetTexture(1, NULL);
    Gfx::pDevice->SetTexture(2, NULL);
    Gfx::pDevice->SetTexture(3, NULL);
    
    //Gfx::pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
    
    //Gfx::pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_NEVER);

    Gfx::pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_BLENDFACTOR);
    for (DWORD i = 0; i < hit_sectors.size(); i++)
    {
        SetupBlendedMode(hit_sectors[i]->flags);
        hit_sectors[i]->mesh->Render();
    }

    RestoreBlendedMode();
    Gfx::pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, alpha_enabled);
    Gfx::pDevice->SetRenderState(D3DRS_ALPHAREF, old_ref);
    Gfx::pDevice->SetRenderState(D3DRS_SRCBLEND, src_blend);
    Gfx::pDevice->SetRenderState(D3DRS_ZFUNC, z_func);
#endif
}
void __stdcall RenderShatterObjects()
{
    rendering = true;
    static DWORD lastFrameCount;

    if (shatterObjects.size())
    {
        //Make sure we update once per frame
        bool update = false;
        if (lastFrameCount != Gfx::frameCounter)
        {
            lastFrameCount = Gfx::frameCounter;
            update = true;
        }

        //Make sure textures are set to zero
        Gfx::pDevice->SetTexture(0, NULL);
        p_current_texture(0) = 0;
        Gfx::pDevice->SetTexture(1, NULL);
        Gfx::pDevice->SetTexture(2, NULL);
        Gfx::pDevice->SetTexture(3, NULL);

        DWORD old, old_state;
        Gfx::pDevice->GetFVF(&old);

        old_state = p_current_renderstate(D3DRS_CULLMODE);
        DWORD old_alpha = p_current_renderstate(D3DRS_ALPHABLENDENABLE);
        DWORD old_alpha2 = p_current_renderstate(D3DRS_ALPHATESTENABLE);

        Gfx::pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
        Gfx::pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
        Gfx::pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);

        Gfx::SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
        Gfx::SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
        Gfx::SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

        float framelength = Game::skater->GetFrameLength() * Gfx::shatter_speed;

        for (DWORD i = 0; i < shatterObjects.size(); i++)
        {
            if (update)
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
            }

            shatterObjects[i]->Render();
        }

        Gfx::pDevice->SetRenderState(D3DRS_CULLMODE, old_state);
        Gfx::pDevice->SetFVF(old);
        Gfx::pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, old_alpha);
        Gfx::pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, old_alpha2);
    }
    rendering = false;
}

void __stdcall RenderExtra()
{
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

    RenderBlendedSectors();
    RenderShatterObjects();

}

void ClearHitSectors();
void UnloadShatterObjects()//004F9C09
{
    while (rendering)
        Sleep(10);
    shatterObjects.clear();
    ClearHitSectors();
}

__declspec(naked) void Render_Naked()
{
    static DWORD pCall = 0x004F9C80;
    static DWORD pJmp = 0x004F9C0E;
   // _asm pushad;
    if (p_render_scene)
        RenderExtra();
   // _asm popad;
    _asm call[pCall];
    _asm jmp[pJmp];
}