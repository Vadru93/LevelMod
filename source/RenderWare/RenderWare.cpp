#include "pch.h"
#include "RenderWare.h"
#include "Objects\model.h"

void RpClump::Render(void* rwCamera)
{
    //If model is flaged as invisible don't render it
    if (model->flags & 6 || model->flags & 1)
        return;

    model->RenderCallback();
    for (RwLLLink* link = atomicList.link.next; link && link != (RwLLLink*)0xFFFFFFFF && link != (RwLLLink*)&atomicList; link = link->next)
    {
        RpAtomic* atomic = (RpAtomic*)((BYTE*)link - 0x10);
        //atomic->Render(rwCamera);
    }
    model->PostRenderCallback();
}

void RpWorld::RenderVisibleClumps(void* rw_camera)
{
    for (RwLLLink* link = this->clumpList.link.next; link && link != (RwLLLink*)0xFFFFFFFF && link != (RwLLLink*)&this->clumpList; link = link->next)
    {
        RpClump* clump = (RpClump*)((BYTE*)link - 0x10);
        clump->Render(rw_camera);
    }
}