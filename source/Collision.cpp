#include "pch.h"
#include "Collision.h"

namespace Collision
{
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