#undef NO_DEFINES
#define _CRT_SECURE_NO_WARNINGS
#include "pch.h"
#include "_math.h"


Vertex& Vertex::Rotate(const Vertex& axis, const float angle)
{

#if 1
    Matrix	mat(*(D3DXVECTOR3*)&axis, angle);
    *this = Vertex(mat.Rotate(*(D3DXVECTOR3*)&*this));
#else
    Quat	quat(*(D3DXVECTOR3*)&axis, angle);
    *this = *(Vertex*)&quat.Rotate(*(D3DXVECTOR3*)&*this);
#endif

    return *this;
}

/*inline Vertex& Vertex::Rotate(const Matrix& mat)
{
    *this = *(Vertex*)&mat.Rotate(*(D3DXVECTOR3*)&*this);

    return *this;
}*/


float GetAngleAbout(Vertex& v1, Vertex& v2, Vertex& Axis)
{
    Axis.Normalize();
    v1.ProjectToPlane(Axis);
    v2.ProjectToPlane(Axis);
    v1.Normalize();
    v2.Normalize();
    Vertex v3 = CrossProduct(&Axis, &v1);
    float v1_dot = D3DXVec3Dot((D3DXVECTOR3*)&v1, (D3DXVECTOR3*)&v2);
    float v3_dot = D3DXVec3Dot((D3DXVECTOR3*)&v3, (D3DXVECTOR3*)&v2);

    float angle = acosf(Clamp(v1_dot, -1.0f, 1.0f));

    if (v3_dot < 0.0f)
    {
        angle = -angle;
    }
    return angle;
}

float	Sgn(float x)
{
    if (x < 0.f)
    {
        return -1.0f;
    }
    else
    {
        return 1.0f;
    }
}


int ClampValue(int value, int min, int max, int outMin, int outMax)
{
    double result = outMin + (value - min) * outMax / max;
    //add 0.5 to round instead of truncate
    return (int)(result + 0.5);
}

inline const float GetSpeed(const Vertex* const __restrict vertex)
{
    const float x = vertex->x;
    const float y = vertex->y;
    const float z = vertex->z;

    return sqrtf(x * x + y * y + z * z);
}



Vertex GetNormal(Vertex* v)
{
    Vertex b;
    float speed = GetSpeed(v);
    if (speed < 0.0001f)
    {
        speed = 1.0f;
    }
    b.x = v->x / speed;
    b.y = v->y / speed;
    b.z = v->z / speed;

    return b;
}


D3DXVECTOR3& Transform(D3DXVECTOR3& V, D3DXMATRIX& M)
{
    static D3DXVECTOR3 Out;
    float x = V.x * M._11 + V.y * M._21 + V.z * M._31 + M._41;
    float y = V.x * M._12 + V.y * M._22 + V.z * M._32 + M._42;
    float z = V.x * M._13 + V.y * M._23 + V.z * M._33 + M._43;
    //float w = V.x * M._14 + V.y * M._24 + V.z * M._34 + M._44; <- don't need scaling for now

    Out.x = x;
    Out.y = y;
    Out.z = z;
    return Out;
}


//Game Function
typedef void(__thiscall* const pOrthoNormalizeAbout)(D3DXMATRIX* pThis, DWORD axis);
void OrthoNormalizeAbout(D3DXMATRIX* matrix, DWORD axis)
{

    pOrthoNormalizeAbout(0x0040F0E0)(matrix, axis);
}


//From thug1src
static int RandSeed;
static int RandA;
static int RandB;

static int RandSeed2;
static int RandC;
static int RandD;

void InitialRand(int a)
{
    RandSeed = a;
    RandA = 314159265;
    RandB = 178453311;
    RandSeed2 = a;
    RandC = 314159265;
    RandD = 178453311;
}

int Rnd(int n)
{
    RandSeed = RandSeed * RandA + RandB;
    RandA = (RandA ^ RandSeed) + (RandSeed >> 4);
    RandB += (RandSeed >> 3) - 0x10101010L;
    return (int)((RandSeed & 0xffff) * n) >> 16;
}

Vector& Vector::Rotate(const Matrix& mat)
{


    *this = mat.Rotate(*this);

    return *this;
}


Vertex CrossProduct(const Vertex* vec1, Vertex* vec2)
{
    Vertex retVec;
    retVec.x = vec1->y * vec2->z - vec1->z * vec2->y;
    retVec.y = vec1->z * vec2->x - vec1->x * vec2->z;
    retVec.z = vec1->x * vec2->y - vec1->y * vec2->x;
    return retVec;
}


float AngleY(D3DXMATRIX& orient, D3DXVECTOR3& from, D3DXVECTOR3& to)
{
    /*D3DXVECTOR3 fromNormalized, toNormalized;
    D3DXVec3Normalize(&fromNormalized, &from);
    D3DXVec3Normalize(&toNormalized, &to);*/


    Vertex tempHeading = Vertex(to - from);
    tempHeading.ProjectToPlane(*(Vertex*)&orient.m[Y]);
    if (!tempHeading.Length())
    {
        // If our rot axis is along Y, for example, the
        // target heading is right above us... Can't pick
        // a rotation!
        return (0.0f);
    }
    tempHeading.Normalize();


    // while we have these two vectors, find the angle between them...
    float angCos = D3DXVec3Dot((D3DXVECTOR3*)&orient.m[Z], &tempHeading);

    // Mick:  contrain Dot product to range -1.0f to +1.0f, since FP innacuracies might
    // make it go outside this range
    // (previously this was done only on NGC.  Not sure why it has started failing now
    // but it seems logical that it would fail occasionally, so this check is necessary) 	
    float	ang = angCos;
    if (ang > 1.0f) ang = 1.0f;
    if (ang < -1.0f) ang = -1.0f;
    float retVal = -acosf(ang);

    Vertex temp = CrossProduct((Vertex*)&orient.m[Z], &tempHeading);

    // check to see if the cross product is in the same quatrant as our rot axis...
    // if so, gots to rotate the other way and shit like that...
    int whichAxis;
    float tempMax = temp.GetMaxAxis(&whichAxis);
    if ((tempMax > 0) == (orient.m[Y][whichAxis] > 0))
    {
        return (-retVal);
    }
    return (retVal);
    //return acosf(Clamp(D3DXVec3Dot(&fromNormalized, &toNormalized), -1.0f, 1.0f)) * 57.29578f * D3DX_PI/180.0f;
}


float GetAngle(const Matrix& currentOrientation, const Vertex& desiredHeading, int headingAxis, int rotAxis)
{

    Vertex tempHeading = desiredHeading;
    tempHeading.ProjectToPlane(currentOrientation[rotAxis]);
    if (!tempHeading.Length())
    {
        // If our rot axis is along Y, for example, the
        // target heading is right above us... Can't pick
        // a rotation!
        return (0.0f);
    }
    tempHeading.Normalize();


    // while we have these two vectors, find the angle between them...
    float angCos = D3DXVec3Dot(&currentOrientation[headingAxis], &tempHeading);

    // Mick:  contrain Dot product to range -1.0f to +1.0f, since FP innacuracies might
    // make it go outside this range
    // (previously this was done only on NGC.  Not sure why it has started failing now
    // but it seems logical that it would fail occasionally, so this check is necessary) 	
    float	ang = angCos;
    if (ang > 1.0f) ang = 1.0f;
    if (ang < -1.0f) ang = -1.0f;
    float retVal = -acosf(ang);

    Vertex temp = CrossProduct(&currentOrientation[headingAxis], &tempHeading);

    // check to see if the cross product is in the same quatrant as our rot axis...
    // if so, gots to rotate the other way and shit like that...
    int whichAxis;
    float tempMax = temp.GetMaxAxis(&whichAxis);
    if ((tempMax > 0) == (currentOrientation[rotAxis][whichAxis] > 0))
    {
        return (-retVal);
    }
    return (retVal);
}