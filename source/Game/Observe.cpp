#undef NO_DEFINES
#define _CRT_SECURE_NO_WARNINGS
#include "pch.h"
#include "Observe.h"

EXTERN ObserveMode* pObserve = NULL;
DWORD time_pressed_x = 0;

void ObserveMode::Enter()
{
    timeNext = 0;
    observing = true;
    if (skater)
        cameraAngle = atan2f((skater->GetPosition()->x - camera->x), (skater->GetPosition()->z - camera->z));
}

void ObserveMode::UpdateInfo(bool firstTime)
{
    //MessageBox(0, "", "", 0);
    first = (void*)*(DWORD*)(*(DWORD*)0x008E2498 + 0x13C);
    last = (void*)*(DWORD*)(*(DWORD*)0x008E2498 + 0x140);
    if (firstTime)
        current = first;
    camera = GetCamera();
    if (current && !InvalidReadPtr(current) && *(DWORD*)current == 0x0058E504 && *(DWORD*)((DWORD)current + 4) != 0xFFFFFFFF)
    {
        skater = (Skater*)*(DWORD*)((DWORD)current + 20);
        if (firstTime)
        {
            currentDistance = *(D3DXVECTOR3*)camera - *(D3DXVECTOR3*)skater->GetPosition();
            if (currentDistance.x < 0.0)
                currentDistance.x *= -1;
            if (currentDistance.z < 0.0)
                currentDistance.z *= -1;
        }
        //D3DXVec3Length(&currentDistance, &currentDistance);

        /*pos = (Vertex*)(*(DWORD*)((DWORD)current + 20) + 0x18);
        velocity = (Vertex*)(*(DWORD*)((DWORD)current + 20) + 0x334);*/
        char* c = (char*)((DWORD)current + 32);
        DWORD i = 0;
        while (*c != 0)
        {
            name[i] = *c;
            c++;
            i++;
        }
        name[i] = 0;
    }
}

void ObserveMode::Next(DWORD time)
{
    timeNext = time;
    /*static const DWORD ptr = 0x008E1E90;
    if (InvalidReadPtr((void*)ptr))
        return;
    DWORD camMode = *(DWORD*)ptr + 0xC0;
    camMode = *(DWORD*)camMode + 0xC;
    if (InvalidReadPtr((void*)camMode))
        return;
    camMode = *(DWORD*)camMode + 0x10;
    if (InvalidReadPtr((void*)camMode))
        return;
    camMode = *(DWORD*)camMode + 0x580;
    if (InvalidReadPtr((void*)camMode))
        return;*/
        /*DWORD camMode = (DWORD)current + 0x4DD;
        camMode = *(DWORD*)camMode + 0x38c;
        camMode = *(DWORD*)camMode + 0x882c;*/
        /*static const DWORD ptr = 0x008E1E90;
        if (InvalidReadPtr((void*)ptr))
            return;
        DWORD camMode = *(DWORD*)ptr + 0x9C;
        camMode = *(DWORD*)camMode + 0x34;
        if (InvalidReadPtr((void*)camMode))
            return;
        camMode = *(DWORD*)camMode + 0xC;
        if (InvalidReadPtr((void*)camMode))
            return;
        camMode = *(DWORD*)camMode + 0xC;
        if (InvalidReadPtr((void*)camMode))
            return;
        camMode = *(DWORD*)camMode + 0x580;
        if (InvalidReadPtr((void*)camMode))
            return;*/
    DWORD camMode = GetCamModeAddress();//Skater::GetCamModeAddress();
    *(DWORD*)camMode = 0;//Set camera to follow your own skater

    first = (void*)*(DWORD*)(*(DWORD*)0x008E2498 + 0x13C);//pointer to first skater=you
    last = (void*)*(DWORD*)(*(DWORD*)0x008E2498 + 0x140);//pointer to last skater

    //check if the player we currently observe exists
    if (PlayerExist())
    {
        if (current == last)
        {
            current = first;
            UpdateInfo();
        }

        //sanity check
        else if (*(DWORD*)*(DWORD*)((DWORD)current + 12) == 0x0058E504 && *(DWORD*)(*(DWORD*)((DWORD)current + 12) + 4) != 0xFFFFFFFF)
        {
            //Get the next pointer
            current = *(void**)((DWORD)current + 12);
            UpdateInfo();
        }
    }
    else
    {
        current = first;
        UpdateInfo();
    }
    debug_print("NewObserver: %s\n", name);
}

void ObserveMode::Update()
{
    //debug_print("Update camera\n");
    static Vertex oldOffset;

    if (PlayerExist())
    {
        //debug_print("Player Exists\n");
        /*static const DWORD ptr = 0x008E1E90;
        if (InvalidReadPtr((void*)ptr))
            return;
        DWORD camMode = *(DWORD*)ptr + 0x9C;
        camMode = *(DWORD*)camMode + 0x34;
        if (InvalidReadPtr((void*)camMode))
            return;
        camMode = *(DWORD*)camMode + 0xC;
        if (InvalidReadPtr((void*)camMode))
            return;
        camMode = *(DWORD*)camMode + 0xC;
        if (InvalidReadPtr((void*)camMode))
            return;
        camMode = *(DWORD*)camMode + 0x580;
        if (InvalidReadPtr((void*)camMode))
            return;*/

            //Set camera mode to free roaming
            /*DWORD camMode = (DWORD)current + 0x4DD;
            camMode = *(DWORD*)camMode + 0x38c;
            sprintf(msg, "%X", camMode);
            MessageBox(0, msg, msg, 0);
            camMode = *(DWORD*)camMode + 0x882c;*/
        DWORD camMode = GetCamModeAddress();// Skater::GetCamModeAddress();
        *(DWORD*)camMode = 2;
        //debug_print("Got CamMode\n");


        //Get camera matrix
        float* matrix = (float*)((DWORD)camera - 0x30);



        //Set camera to be same as skater position
        *camera = *(Vertex*)skater->GetPosition();
        //currentDistance is the distance stored from camera when enterobserver was called
        camera->y += currentDistance.y + 25.0f;

        //Get the velocity and move the camera accordingly
        Vertex offset;
        offset = *(Vertex*)skater->GetVelocity();
        if ((fabsf(offset.x) + fabsf(offset.z)) != 0.0f)//Is speed greater than 0?
        {

            float normalizer = sqrtf((offset.x * offset.x) + (offset.z * offset.z));
            if (normalizer)
            {
                offset.x /= normalizer;
                offset.z /= normalizer;
                //Check if we are in vert, then we need to invert distance
                if (!skater->InVert())
                {
                    offset.x *= (currentDistance.x + currentDistance.z) * 1.5f;
                    offset.z *= (currentDistance.x + currentDistance.z) * 1.5f;
                }
                else
                {
                    offset.x *= -(currentDistance.x + currentDistance.z) * 1.5f;
                    offset.z *= -(currentDistance.x + currentDistance.z) * 1.5f;
                }
                if ((fabsf(offset.x) + fabsf(offset.z)) >= 50.0f)
                {
                    camera->x -= offset.x;
                    camera->z -= offset.z;
                    oldOffset = offset;
                }
                else
                {
                    camera->x -= oldOffset.x;
                    camera->z -= oldOffset.z;
                }
            }
            else
            {
                camera->x -= oldOffset.x;
                camera->z -= oldOffset.z;
            }
        }
        else
        {
            camera->x -= oldOffset.x;
            camera->z -= oldOffset.z;
        }

        //Get angle between camera and skater
        cameraAngle = atan2f((skater->GetPosition()->x - camera->x), (skater->GetPosition()->z - camera->z));

        Vertex oldPos = *camera;
        //Temporarly set Position to zero
        *camera = Vertex(0, 0, 0);

        //Rotate and normalize
        ((Matrix*)matrix)->RotateYLocal(cameraAngle);
        ((Matrix*)matrix)->OrthoNormalizeAbout2(Y);
        D3DXMATRIX yaw;
        D3DXMatrixRotationYawPitchRoll((D3DXMATRIX*)matrix, cameraAngle, 8.0f * D3DX_PI / 180.0f, 0);
        //D3DXMatrixMultiply((D3DXMATRIX*)matrix, (D3DXMATRIX*)matrix , &yaw);
        //Set back camera position
        *camera = oldPos;

    }
}

void ObserveMode::InterpolateCamera()
{
    static float didit = false;
    float* matrix = (float*)((DWORD)camera - 0x30);



    /*float oldAngle = asin(*(float*)((DWORD)matrix + 8));

    Vertex direction = *(Vertex*)((DWORD)matrix + 32);

    Vertex target = *pos;
    direction.x += target.x;
    direction.y += target.y;
    direction.z += target.z;
    float normalizer = sqrtf(direction.x*direction.x + direction.y*direction.y + direction.z*direction.z);
    direction.x /= normalizer;
    direction.y /= normalizer;
    direction.z /= normalizer;
    Vertex diff;
    diff.x = target.x - camera->x;
    diff.y = target.y - camera->y;
    diff.z = target.z - camera->z;
    normalizer = sqrtf(diff.x*diff.x + diff.y*diff.y + diff.z*diff.z);
    diff.x /= normalizer;
    diff.y /= normalizer;
    diff.z /= normalizer;
    Vertex reverseAngle;
    reverseAngle = dot(diff, direction);
    float angle = acos(reverseAngle);*/

    /*Vertex normalizedCam = oldCamera;
    float normalizer = sqrtf(normalizedCam.x*normalizedCam.x + normalizedCam.y*normalizedCam.y + normalizedCam.z*normalizedCam.z);
    normalizedCam.x /= normalizer;
    normalizedCam.y /= normalizer;
    normalizedCam.z /= normalizer;
    Vertex normalizedPos = *pos;
    normalizer = sqrtf(normalizedPos.x*normalizedPos.x + normalizedPos.y*normalizedPos.y + normalizedPos.z*normalizedPos.z);
    normalizedPos.x /= normalizer;
    normalizedPos.y /= normalizer;
    normalizedPos.z /= normalizer;

    float angle = acos(normalizedCam.x*normalizedPos.x + normalizedCam.y*normalizedPos.y + normalizedCam.z*normalizedPos.z);

    Vertex axis;
    axis.x = normalizedCam.y*normalizedPos.z - normalizedPos.y*normalizedCam.z;
    axis.y = normalizedCam.z*normalizedPos.x - normalizedPos.z*normalizedCam.x;
    axis.z = normalizedCam.x*normalizedPos.y - normalizedPos.x*normalizedCam.y;
    normalizer = sqrtf(axis.x*axis.x + axis.y*axis.y + axis.z*axis.z);
    axis.y /= normalizer;*/

    /*float oldAngle = asinf(*(float*)((DWORD)matrix + 8));
    angle /= oldAngle;*/
    /*angle = sinf(angle);
    angle /= *(float*)((DWORD)matrix + 8);*/
    //skater pos - camera pos
    float x = skater->GetPosition()->x - camera->x;
    float z = skater->GetPosition()->z - camera->z;
    static bool first = false;
    static bool done = false;

    float angle = atan2f(z, x);//calculating angle between skater and camera
    float skaterAngle = angle;
    angle += 0.03f;//for rounding errors
    float oldAngle = (*(float*)((DWORD)matrix + 8) + 1.0f) * 3.14159f * 0.5f;//getting old camera angle
    if (*(float*)matrix < 0.0f)
        oldAngle = -oldAngle;
    if (oldAngle >= 3.141f)
        oldAngle = 3.141f;//oldAngle = angle - 0.025f;
    else if (oldAngle <= -3.141f)
    {
        oldAngle = -3.141f;
    }

    if ((angle <= 0.08f && angle >= -0.08f))
    {
        if (done)
        {
            first = true;
            done = false;
        }
        else
        {
            first = false;
            if (angle > 0.0f)
                angle = 0.0f;
            else
                angle = 0.0f;
        }
    }
    else if (angle >= 3.1f)
    {
        if (done)
        {
            first = true;
            done = false;
        }
        else
        {
            first = false;
            angle = 3.1f;
        }
    }
    else if (angle <= -3.1f)
    {
        if (done)
        {
            first = true;
            done = false;
        }
        else
        {
            first = false;
            angle = -3.1f;
        }
    }
    else
    {
        first = false;
        done = false;
    }

    if ((fabsf(fabsf(oldAngle) - fabsf(angle)) <= 0.08f && fabsf(oldAngle - angle) < 1.5f))
    {
        //debug_print("too small\n");
        return;
    }


    //debug_print("angle: %f old angle: %f x: %f z %f\n", angle, oldAngle, x, z);
    /*bool nulling = false;
    if (angle>=3.14f || angle<=-3.14f)
    nulling = true;
    BYTE reversing = 0;
    if (oldAngle < 0.0f && angle > 0.0f)
    reversing = 1;
    else
    if (oldAngle > 0.0f && angle < 0.0f)
    reversing = 2;*/

    angle -= oldAngle;

    debug_print("oldAngle(%f) deltaAngle(%f) skaterAngle(%f)\n", oldAngle, angle, angle + oldAngle);

    if (angle >= 3.1f || angle <= -3.1f)
    {
        angle += oldAngle;

        /*angle += oldAngle;
        if (angle > 0.0f && oldAngle < 0.0f)
        {

        angle = ((angle - 3.141f) + (3.141f + oldAngle));

        }
        else if (angle < 0.0f && oldAngle > 0.0f)
        {
        angle = -((angle + 3.141f) + (3.141f - oldAngle));
        }
        else
        {*/
        if (angle > 0.0f && oldAngle < 0.0f)
        {
            float absAng = fabsf(oldAngle);
            if (fabsf(angle) > absAng)
            {
                angle -= absAng;
                angle = -angle;
            }
            else
                angle -= absAng;


        }
        else if (angle < 0.0f && oldAngle > 0.0f)
        {
            float absAng = fabsf(oldAngle);
            if (fabsf(angle) > absAng)
            {
                angle += absAng;
                angle = -angle;
            }
            else
                angle += absAng;
            //debug_print("reversing with angle %f old(%f) angl(%f)\n", angle, oldAngle, (atan2f(z, x) + 0.03f));

        }
        else
        {
            angle -= oldAngle;
            angle = 3.141f - fabsf(angle);
            //angle = -angle;
        }


        //}
        if ((angle <= 0.04f && angle >= -0.04f))
            return;
    }

    /*if (angle >= 3.141f)
    {
    if(oldAngle <= -3.141f)
    angle = 0.0f;
    else
    angle = -angle;
    debug_print("reversing cause angle is 3.14\n");
    }
    else if (angle <= -3.141f)
    {
    if(oldAngle >= 3.141f)
    angle = 0.0f;
    else
    angle = -angle;
    debug_print("reversing cause angle is 3.14\n");
    }*/

    if (!first)
    {
        if (angle)
            angle /= 1.5f;
        if (angle >= 0.04f)
        {
            if (angle > 0.08f)
                angle = 0.04f;
        }
        else if (angle <= -0.04f)
        {
            if (angle < -0.08f)
                angle = -0.04f;
        }
        else
        {
            return;
            //debug_print("nulling cause smaller than 0.02\n");
        }
    }
    if (angle == 0.0f)
        return;
    /*else
    {
    if (skaterAngle>0.0f)
    angle += 0.04f;
    if (skaterAngle<0.0f)
    angle -= 0.04f;
    }*/
    /*

    if (angle >= 0.1f)
    angle = 0.08f;
    if (angle <= -0.1f)
    angle = -0.08f;

    if (reversing == 1)
    angle = 0.08f;
    else if (reversing)
    angle = -0.08f;
    if (nulling)
    angle = 0.0f;
    if (angle > 0.0f)
    {
    if (angle < 0.02f)
    angle = 0.0f;
    }
    else if (angle > -0.02f)
    angle = 0.0f;
    */

    /*angle = atan2f(z, x);//calculating angle between skater and camera
    angle += 0.03f;//for rounding errors
    oldAngle = (*(float*)((DWORD)matrix + 8) + 1.0f)*3.14159f*0.5f;//getting old camera angle
    if ((angle-oldAngle)>=3.1f || (angle-oldAngle)<=-3.1f)
    {
    if (angle > 0.0f && oldAngle < 0.0f)
    {
    float absAng = fabsf(oldAngle);
    if (fabsf(angle)>absAng)
    {
    angle -= absAng;
    angle = -angle;
    }
    else
    angle -= absAng;


    }
    else if (angle < 0.0f && oldAngle > 0.0f)
    {
    float absAng = fabsf(oldAngle);
    if (fabsf(angle) > absAng)
    {
    angle += absAng;
    angle = -angle;
    }
    else
    angle += absAng;
    //debug_print("reversing with angle %f old(%f) angl(%f)\n", angle, oldAngle, (atan2f(z, x) + 0.03f));

    }
    }
    else
    angle -= oldAngle;
    if (angle>0.0f)
    {
    if (angle >= 1.6f)
    angle = 1.6f;
    else if (angle >= 1.2f)
    angle = 1.2f;
    else if (angle >= 1.0f)
    angle = 1.0f;
    else if (angle >= 0.8f)
    angle = 0.8f;
    else if (angle >= 0.6f)
    angle = 0.6f;
    else if (angle >= 0.4f)
    angle = 0.4f;
    else if (angle >= 0.2f)
    angle = 0.2f;
    else if (angle >= 0.08f)
    angle = 0.08f;
    else if (angle >= 0.06f)
    angle = 0.06f;
    else if (angle >= 0.04f)
    angle = 0.04f;
    else if (angle >= 0.02f)
    angle = 0.02f;
    else
    angle = 0.0f;
    }
    else if (angle < 0.0f)
    {
    if (angle <= -1.6f)
    angle = -1.6f;
    else if (angle <= -1.2f)
    angle = -1.2f;
    else if (angle <= -1.0f)
    angle = -1.0f;
    else if (angle <= -0.8f)
    angle = -0.8f;
    else if (angle <= -0.6f)
    angle = -0.6f;
    else if (angle <= -0.4f)
    angle = -0.4f;
    else if (angle <= -0.2f)
    angle = -0.2f;
    else if (angle <= -0.1f)
    angle = -0.1f;
    else if (angle <= -0.08f)
    angle = -0.08f;
    else if (angle <= -0.06f)
    angle = -0.06f;
    else if (angle <= -0.04f)
    angle = -0.04f;
    else if (angle <= -0.02f)
    angle = -0.02f;
    else
    angle = 0.0f;
    }
    else
    return;*/
    if (((oldAngle + angle) >= 1.54f && (oldAngle + angle) <= 1.6f) || ((oldAngle + angle) <= -1.54f && (oldAngle + angle) >= -1.6f) || ((oldAngle + angle) >= 3.11f && (oldAngle + angle) <= 3.17f) || ((oldAngle + angle) <= -3.11f && (oldAngle + angle) >= -3.17f))
        angle *= 1.5f;
    if (((oldAngle + angle) >= 1.54f && (oldAngle + angle) <= 1.6f) || ((oldAngle + angle) <= -1.54f && (oldAngle + angle) >= -1.6f) || ((oldAngle + angle) >= 3.11f && (oldAngle + angle) <= 3.17f) || ((oldAngle + angle) <= -3.11f && (oldAngle + angle) >= -3.17f))
        angle *= 0.5f;
    if (((oldAngle + angle) >= 1.54f && (oldAngle + angle) <= 1.6f) || ((oldAngle + angle) <= -1.54f && (oldAngle + angle) >= -1.6f) || ((oldAngle + angle) >= 3.11f && (oldAngle + angle) <= 3.17f) || ((oldAngle + angle) <= -3.11f && (oldAngle + angle) >= -3.17f))
        angle *= 4.0f;
    if (((oldAngle + angle) >= 1.54f && (oldAngle + angle) <= 1.6f) || ((oldAngle + angle) <= -1.54f && (oldAngle + angle) >= -1.6f) || ((oldAngle + angle) >= 3.11f && (oldAngle + angle) <= 3.17f) || ((oldAngle + angle) <= -3.11f && (oldAngle + angle) >= -3.17f))
        angle *= 0.5f;
    if (((oldAngle + angle) >= 1.54f && (oldAngle + angle) <= 1.6f) || ((oldAngle + angle) <= -1.54f && (oldAngle + angle) >= -1.6f) || ((oldAngle + angle) >= 3.11f && (oldAngle + angle) <= 3.17f) || ((oldAngle + angle) <= -3.11f && (oldAngle + angle) >= -3.17f))
        return;
    /*if (((angle + oldAngle) >= 3.13f || (angle + oldAngle) <= -3.13f))
      angle *= 0.8f;
      if (((angle + oldAngle) >= 3.13f || (angle + oldAngle) <= -3.13f))
      angle *= 0.8f;
      if (((angle + oldAngle) >= 3.13f || (angle + oldAngle) <= -3.13f))
      angle *= 0.8f;*/
    if ((angle + oldAngle) >= 3.13f)
        angle -= (angle + oldAngle - 3.13f) - 0.02f;
    else if ((angle + oldAngle) <= -3.13f)
        angle += (angle + oldAngle + 3.13f) + 0.02f;
    /*if (((oldAngle + angle) >= 1.54f && (oldAngle + angle) <= 1.6f) || ((oldAngle + angle) <= -1.54f && (oldAngle + angle) >= -1.6f) || ((oldAngle + angle) >= 3.11f && (oldAngle + angle) <= 3.17f) || ((oldAngle + angle) <= -3.11f && (oldAngle + angle) >= -3.17f))
    angle *= 0.8f;
    if (((oldAngle + angle) >= 1.54f && (oldAngle + angle) <= 1.6f) || ((oldAngle + angle) <= -1.54f && (oldAngle + angle) >= -1.6f) || ((oldAngle + angle) >= 3.11f && (oldAngle + angle) <= 3.17f) || ((oldAngle + angle) <= -3.11f && (oldAngle + angle) >= -3.17f))
    angle *= 1.5f;
    if (((oldAngle + angle) >= 1.54f && (oldAngle + angle) <= 1.6f) || ((oldAngle + angle) <= -1.54f && (oldAngle + angle) >= -1.6f) || ((oldAngle + angle) >= 3.11f && (oldAngle + angle) <= 3.17f) || ((oldAngle + angle) <= -3.11f && (oldAngle + angle) >= -3.17f))
    return;*/

    float rotation[16] = { 1.0f, 0.0f, sinf(angle), 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, -sinf(angle), 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f };


    float xn = *(float*)matrix * rotation[0];
    float yn = *(float*)((DWORD)matrix + 4) * rotation[4];
    float zn = *(float*)((DWORD)matrix + 8) * rotation[8];
    *(float*)matrix = xn + yn + zn;

    xn = *(float*)((DWORD)matrix) * rotation[1];
    yn = *(float*)((DWORD)matrix + 4) * rotation[5];
    zn = *(float*)((DWORD)matrix + 8) * rotation[9];
    *(float*)((DWORD)matrix + 4) = xn + yn + zn;

    xn = *(float*)((DWORD)matrix) * rotation[2];
    yn = *(float*)((DWORD)matrix + 4) * rotation[6];
    zn = *(float*)((DWORD)matrix + 8) * rotation[10];
    *(float*)((DWORD)matrix + 8) = xn + yn + zn;

    xn = *(float*)((DWORD)matrix + 16) * rotation[0];
    yn = *(float*)((DWORD)matrix + 20) * rotation[4];
    zn = *(float*)((DWORD)matrix + 24) * rotation[8];
    *(float*)((DWORD)matrix + 16) = xn + yn + zn;

    xn = *(float*)((DWORD)matrix + 16) * rotation[1];
    yn = *(float*)((DWORD)matrix + 20) * rotation[5];
    zn = *(float*)((DWORD)matrix + 24) * rotation[9];
    *(float*)((DWORD)matrix + 20) = xn + yn + zn;

    xn = *(float*)((DWORD)matrix + 16) * rotation[2];
    yn = *(float*)((DWORD)matrix + 20) * rotation[6];
    zn = *(float*)((DWORD)matrix + 24) * rotation[10];
    *(float*)((DWORD)matrix + 24) = xn + yn + zn;

    xn = *(float*)((DWORD)matrix + 32) * rotation[0];
    yn = *(float*)((DWORD)matrix + 36) * rotation[4];
    zn = *(float*)((DWORD)matrix + 40) * rotation[8];
    *(float*)((DWORD)matrix + 32) = xn + yn + zn;

    xn = *(float*)((DWORD)matrix + 32) * rotation[1];
    yn = *(float*)((DWORD)matrix + 36) * rotation[5];
    zn = *(float*)((DWORD)matrix + 40) * rotation[9];
    *(float*)((DWORD)matrix + 36) = xn + yn + zn;

    xn = *(float*)((DWORD)matrix + 32) * rotation[2];
    yn = *(float*)((DWORD)matrix + 36) * rotation[6];
    zn = *(float*)((DWORD)matrix + 40) * rotation[10];
    *(float*)((DWORD)matrix + 40) = xn + yn + zn;

    xn = *(float*)((DWORD)matrix + 48) * rotation[0];
    yn = *(float*)((DWORD)matrix + 52) * rotation[4];
    zn = *(float*)((DWORD)matrix + 56) * rotation[8];
    *(float*)((DWORD)matrix + 48) = xn + yn + zn;

    xn = *(float*)((DWORD)matrix + 48) * rotation[1];
    yn = *(float*)((DWORD)matrix + 52) * rotation[5];
    zn = *(float*)((DWORD)matrix + 56) * rotation[9];
    *(float*)((DWORD)matrix + 52) = xn + yn + zn;

    xn = *(float*)((DWORD)matrix + 48) * rotation[2];
    yn = *(float*)((DWORD)matrix + 52) * rotation[6];
    zn = *(float*)((DWORD)matrix + 56) * rotation[10];
    *(float*)((DWORD)matrix + 56) = xn + yn + zn;
    /*float xn = *(float*)matrix*1.0f;
    float yn = 0.0f;
    float zn = 0.0f;
    *(float*)matrix = xn + yn + zn;

    xn = 0.0f;
    yn = *(float*)((DWORD)matrix + 4)*1.0f;
    if (yaw != 0.0f)
    zn = *(float*)((DWORD)matrix + 8) * (-yaw);
    else
    zn = 0.0f;
    *(float*)((DWORD)matrix + 4) = xn + yn + zn;

    xn = 0.0f;
    yn = *(float*)((DWORD)matrix + 4)*yaw;
    zn = *(float*)((DWORD)matrix + 8)*1.0f;
    *(float*)((DWORD)matrix + 8) = xn + yn + zn;

    xn = *(float*)((DWORD)matrix + 16)*1.0f;
    yn = 0.0f;
    zn = 0.0f;
    *(float*)((DWORD)matrix + 16) = xn + yn + zn;

    xn = 0.0f;
    yn = *(float*)((DWORD)matrix + 20)*1.0f;
    if (yaw != 0.0f)
    zn = *(float*)((DWORD)matrix + 24) * (-yaw);
    else
    zn = 0.0f;
    *(float*)((DWORD)matrix + 20) = xn + yn + zn;

    xn = 0.0f;
    yn = *(float*)((DWORD)matrix + 20)*yaw;
    zn = *(float*)((DWORD)matrix + 24)*1.0f;
    *(float*)((DWORD)matrix + 24) = xn + yn + zn;

    xn = *(float*)((DWORD)matrix + 32)*1.0f;
    yn = 0.0f;
    zn = 0.0f;
    *(float*)((DWORD)matrix + 32) = xn + yn + zn;

    xn = 0.0f;
    yn = *(float*)((DWORD)matrix + 36)*1.0f;
    if (yaw != 0.0f)
    zn = *(float*)((DWORD)matrix + 40) * (-yaw);
    else
    zn = 0.0f;
    *(float*)((DWORD)matrix + 36) = xn + yn + zn;

    xn = 0.0f;
    yn = *(float*)((DWORD)matrix + 36)*yaw;
    zn = *(float*)((DWORD)matrix + 40)*1.0f;
    *(float*)((DWORD)matrix + 40) = xn + yn + zn;

    xn = *(float*)((DWORD)matrix + 48)*1.0f;
    yn = 0.0f;
    zn = 0.0f;
    *(float*)((DWORD)matrix + 48) = xn + yn + zn;

    xn = 0.0f;
    yn = *(float*)((DWORD)matrix + 52)*1.0f;
    if (yaw != 0.0f)
    zn = *(float*)((DWORD)matrix + 56) * (-yaw);
    else
    zn = 0.0f;
    *(float*)((DWORD)matrix + 52) = xn + yn + zn;

    xn = 0.0f;
    yn = *(float*)((DWORD)matrix + 52)*yaw;
    zn = *(float*)((DWORD)matrix + 56)*1.0f;
    *(float*)((DWORD)matrix + 56) = xn + yn + zn;*/
}

bool observing = false;
bool EnterObserveMode(CStruct* pParams, CScript* pScript)
{
    Network::NetHandler* net_manager = Network::NetHandler::Instance();
    if (net_manager)
    {
        net_manager->ObserveNextPlayer();
        observing = true;
        time_pressed_x = Game::skater->GetKeyState(KeyState::OLLIE)->GetPressedTime();
    }
    return observing;

    //old code
    /*
    if (pObserve)
    {
        delete pObserve;
        pObserve = NULL;
    }
    pObserve = new ObserveMode;
    pObserve->Enter();
    debug_print("Entered ObserveMode, Observing player: %s\n", pObserve->name);
    return pObserve->current != NULL;
    return true;*/
}

bool ObserveNext(CStruct* pParams, CScript* pScript)
{
    if (observing)
    {
        Network::NetHandler* net_manager = Network::NetHandler::Instance();
        if (net_manager)
            net_manager->ObserveNextPlayer();
    }
    return observing;

    //old code
    /*
    if (pObserve)
    {
        pObserve->Next();
        return true;
    }
    return true;*/
}

bool LeaveObserveMode(CStruct* pParams, CScript* pScript)
{
    if (observing)
    {
        Network::NetHandler* net_manager = Network::NetHandler::Instance();
        if (net_manager)
        {
            net_manager->LeaveObserverMode();
            observing = false;
        }
    }
    return !observing;

    //old code
    /*
    if (pObserve)
    {
        DWORD camMode = pObserve->GetCamModeAddress();// Skater::GetCamModeAddress();
        *(DWORD*)camMode = 0;
        pObserve->Leave();
        delete pObserve;
        pObserve = NULL;
    }
    return true;*/
}
