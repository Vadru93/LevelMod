#pragma once
namespace Gfx
{
    struct Camera
    {
        Matrix		    matrix;							// orientation matrix
        Vector		    pos;						    // camera position
        Matrix		    mat;						    // maybe old matrix?

        //Not sure these exists in th3??
        float			h_fov;							// horizontal field of view angle (degrees)
        float			adj_h_fov;						// screen adjusted horizontal field of view angle (degrees)

        float			near_clip;						// near clip plane
        float			far_clip;						// far clip plane
    };

    struct CameraContainer
    {
        DWORD unk;//probably refcount?
        Camera* pCamera;
    };

    struct Viewport
    {
        Vector rect;
        CameraContainer* pCameraContainer;
    };

    struct ViewportManager
    {
        static ViewportManager* Instance()
        {
            return *(ViewportManager**)0x008e1e78;
        }

        DWORD GetNumActiveCams()
        {
            return *(DWORD*)(0x0058d970 + *(int*)((int)this + 0xbc) * 4);
        }

        Viewport* GetViewport(DWORD index)
        {
            return *(Viewport**)((int)this + index * 4 + 0x6c);
        }

        Camera* GetCamera(DWORD index)
        {
            return GetViewport(index)->pCameraContainer->pCamera;
        }
    };
};