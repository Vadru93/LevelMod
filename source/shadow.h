#pragma once
#include "defines.h"
#include "Model.h"


extern Skater* Game::skater;


struct ShadowMap
{
    Model* model;
    D3DXMATRIX lightView;
    D3DXMATRIX lightProj;
    IDirect3DTexture9* shadow;

    ShadowMap(Model* mdl)
    {
        model = mdl;
        if (D3DXCreateTexture(Gfx::pDevice, 256, 256, 1, D3DUSAGE_RENDERTARGET, D3DFMT_D24S8, D3DPOOL_MANAGED, &shadow) != D3D_OK)
            MessageBox(0, "Failed to create shadow", "", 0);
    }

    void Update(float distance)
    {
        D3DXMATRIX inverse;
        D3DXMatrixInverse(&inverse, NULL, Game::skater->GetMatrix());

        D3DXVECTOR3 camPos = *(D3DXVECTOR3*)&inverse._41;

        Vertex position = model->pos;
        Vertex lightDir = *(Vertex*)&(position - Gfx::sun_position);
        lightDir.Normalize();

        bool cast_shadow = false;
        Game::skater->Store();
        D3DXVECTOR3 shadow_pos = position + 40000.0f * lightDir;
        Game::skater->SetRay(Gfx::sun_position, shadow_pos);
        if (Game::skater->CollisionCheck(Collision::Flags::Skatable))
        {
            cast_shadow = true;
            shadow_pos = *Game::skater->GetHitPoint();
        }
        Game::skater->Restore();
        if (cast_shadow)
        {
            D3DXMatrixLookAtLH(&lightView, &Gfx::sun_position, &shadow_pos, &D3DXVECTOR3(0, 0, 1));
            D3DXMatrixOrthoLH(&lightProj, 96.0f, 96.0f, 1.0f, 128.0f);

            //Set the view and projection matrices to match the lights "view"
            Gfx::pDevice->SetTransform(D3DTS_VIEW, &lightView);
            Gfx::pDevice->SetTransform(D3DTS_PROJECTION, &lightProj);

            //Render model onto the shadow texture
            IDirect3DSurface9* renderTarget;
            shadow->GetSurfaceLevel(0, &renderTarget);
            Gfx::pDevice->SetRenderTarget(0, renderTarget);
            Gfx::pDevice->Clear(0, NULL, D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, 0, 1.0f, 0);
            //model->Render();

            //Release target to decrease vp count
            renderTarget->Release();

        }
        /*float fDistance;
        float faNear[3];
        float faFar[3];
        float faDiff[3];
        D3DXVECTOR3 vaAxis[3];
        D3DXVECTOR3 tmp = D3DXVECTOR3();
        D3DXVECTOR3 vCenterPosition = D3DXVECTOR3();

        int i, k;
        for (i = 0; i < 3; i++)
        {
            faNear[i] = FLT_MAX;
            faFar[i] = -FLT_MAX;
        }
        D3DXVec3Normalize(&vaAxis[2], &lightDir);
        D3DXVec3Cross(&tmp, &D3DXVECTOR3(0,0,1), &vaAxis[2]);
        D3DXVec3Normalize(&vaAxis[0], &tmp);
        D3DXVec3Cross(&tmp, &vaAxis[2], &vaAxis[0]);
        D3DXVec3Normalize(&vaAxis[1], &tmp);

        for (i = 0; i < 3; i++)
        {
            faDiff[i] = faFar[i] - faNear[i];
            vCenterPosition += vaAxis[i] * (faNear[i] + faFar[i]);
        }
        vCenterPosition *= 0.5f;

        float ZNear = 100.0f;
        float fLightZFar = faDiff[2] + ZNear;
        D3DXVECTOR3 vLightPosition = vCenterPosition - vaAxis[2] * (faDiff[2] * 0.5f + ZNear);

        IDirect3DSurface9* old_surface = NULL, * old_stencil = NULL;
        D3DXMATRIX old_view, old_proj;

        Gfx::pDevice->GetTransform(D3DTS_VIEW, &old_view);
        Gfx::pDevice->GetTransform(D3DTS_PROJECTION, &old_proj);
        Gfx::pDevice->GetRenderTarget(0, &old_surface);
        Gfx::pDevice->GetDepthStencilSurface(&old_stencil);

        Gfx::pDevice->SetTransform(D3DTS_VIEW, lightView);
        Gfx::pDevice->SetTransform(D3DTS_PROJECTION, lightProj);
        Gfx::pDevice->SetRenderTarget(0, renderTarget);
        Gfx::pDevice->SetDepthStencilSurface(renderSurface);
        */
    }
};
extern std::vector<ShadowMap> shadows;

void RenderShadows()
{
#define D3DCOLORWRITEENABLE_ALL D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_ALPHA
    Gfx::pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0);
    for (DWORD i = 0; i < shadows.size(); i++)
    {
        shadows[i].Update(500.0f);
    }
    Gfx::pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_ALL);

    //Gfx::pDevice->SetVertexShader(ShadowBufferStaticGeomVS);
    //Gfx::pDevice->SetPixelShader(ShadowBufferStaticGeomPS);
    Gfx::pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, 1);
    Gfx::pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, 1);
    Gfx::pDevice->SetTexture(0, NULL);
    Gfx::pDevice->SetTexture(1, NULL);
    Gfx::pDevice->SetTexture(2, NULL);
    Gfx::pDevice->SetTexture(3, NULL);

    D3DXMATRIX viewMatrix, projMatrix, dest_matrix;
    Gfx::pDevice->GetTransform(D3DTS_VIEW, &viewMatrix);
    Gfx::pDevice->GetTransform(D3DTS_PROJECTION, &projMatrix);

    viewMatrix.m[3][0] = 0.0f;
    viewMatrix.m[3][1] = 0.0f;
    viewMatrix.m[3][2] = 0.0f;
    viewMatrix.m[3][3] = 1.0f;

    D3DXMatrixMultiply(&dest_matrix, &viewMatrix, &projMatrix);
    Gfx::pDevice->SetVertexShaderConstantF(0, (float*)&dest_matrix, 4);

    for (DWORD i = 0; i < shadows.size(); i++)
    {
        D3DXMATRIX final = shadows[i].lightView * shadows[i].lightProj;
        Gfx::pDevice->SetTexture(1, shadows[i].shadow);

        Gfx::pDevice->SetSamplerState(1, D3DSAMP_ADDRESSU, D3DTADDRESS_BORDER);
        Gfx::pDevice->SetSamplerState(1, D3DSAMP_ADDRESSV, D3DTADDRESS_BORDER);
        Gfx::pDevice->SetSamplerState(1, D3DSAMP_BORDERCOLOR, 0xFFFFFFFF);
        Gfx::pDevice->SetSamplerState(1, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
        Gfx::pDevice->SetSamplerState(1, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);

        Gfx::pDevice->SetVertexShaderConstantF(4, (float*)&final, 4);

        //set_blend_mode(vBLEND_MODE_MODULATE_COLOR);

        //model->Render();
    }

    Gfx::pDevice->SetVertexShader(NULL);
    Gfx::pDevice->SetPixelShader(NULL);
    //Gfx::pDevice->SetRenderTarget(0, old_target);
    Gfx::pDevice->SetTexture(1, NULL);

}
