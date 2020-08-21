#pragma once
#ifndef __MATH_H
#define __MATH_H
#undef NO_DEFINES
#include "defines.h"
#include <d3dx9.h>
#include <d3dx9core.h>
#include <math.h>
/*#include <d3dx8.h>
#include <d3dx8core.h>*/


int ClampValue(int value, int min, int max, int outMin, int outMax);

inline	float	ClampMax(float v, float max)
{
	if (v > max)
	{
		return max;
	}
	else
	{
		return v;
	}
}

inline	float	ClampMin(float v, float min)
{
	if (v < min)
	{
		return min;
	}
	else
	{
		return v;
	}
}

inline	float	Clamp(float v, float min, float max)
{
	return ClampMin(ClampMax(v, max), min);
}

D3DXVECTOR3& Transform(D3DXVECTOR3& V, D3DXMATRIX& M);


//Game function
void OrthoNormalizeAbout(D3DXMATRIX* matrix, DWORD axis);

//From thug1Source, with some slight modifications
inline	float	Determinant3(float a1, float a2, float a3, float b1, float b2, float b3, float c1, float c2, float c3)
{
	return 	a1 * (b2 * c3 - b3 * c2) -
		b1 * (a2 * c3 - a3 * c2) +
		c1 * (a2 * b3 - a3 * b2);
}

struct Vector : D3DXVECTOR4
{

	void Scale(float scale)
	{
		this->x *= scale;
		this->y *= scale;
		this->z *= scale;
		this->w *= scale;
	}
};

struct Quat
{
	D3DXVECTOR4 quat;


	inline void		SetVector(const float x, const float y, const float z)
	{


		quat.x = x;
		quat.y = y;
		quat.z = z;
	}
	Quat()
	{

	}
	inline Quat(const D3DXVECTOR3& axis, float angle)
	{


		float	mod = D3DXVec3Length(&axis);
		float	ang = angle / 2.0f;

		mod = (mod > 0.0f) ? (1.0f / mod) : 0.0f;
		mod *= sinf(ang);

		SetVector(mod * axis[X], mod * axis[Y], mod * axis[Z]);
		quat.w = cosf(ang);
	}

	Quat(float x, float y, float z, float w)
	{
		quat.x = x;
		quat.y = y;
		quat.z = z;
		quat.w = w;
	}

	Quat& operator *=(Quat& other)
	{
		quat.x *= other.quat.x;
		quat.y *= other.quat.y;
		quat.z *= other.quat.z;
		quat.w *= other.quat.w;
		return *this;
	}

	inline Quat& Invert(void) 							// this = Invert ( this )
	{


		quat.x = -quat.x;
		quat.y = -quat.y;
		quat.z = -quat.z;


		return	*this;
	}

	inline Quat& Invert(const Quat& src) 			// this = Invert ( src )
	{


		quat.x = -src.quat.x;
		quat.y = -src.quat.y;
		quat.z = -src.quat.z;
		quat.w = src.quat.w;

		return	*this;
	}



	inline	D3DXVECTOR3	Rotate(const D3DXVECTOR3& vec) const
	{


		Quat	inv;
		//	Quat 	pt( vec[X], vec[Y], vec[Z], vec[W] );
		Quat 	pt(vec.x, vec.y, vec.z, 1.0f);		// Mick: Setting W to sensible value, otherwise can cause overflow
		Quat 	res = *this;

		inv.Invert(*this);
		res *= pt;
		res *= inv;

		return D3DXVECTOR3(res.quat.x, res.quat.y, res.quat.z);
	}
};



struct Vertex : D3DXVECTOR3
{
	Vertex()
	{
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
	}

	/*Vertex& operator=	(const D3DXVECTOR3& v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
		return *this;
	}

	Vertex& operator=	(D3DXVECTOR3& v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
		return *this;
	}

	D3DXVECTOR3& operator=	(const Vertex& v)
	{
		return *(D3DXVECTOR3*)&v;
	}

	D3DXVECTOR3& operator=	(Vertex& v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
		return *this;
	}*/


	float	GetMaxAxis(int* whichAxis)
	{
		int which_axis;
		if (!whichAxis)
		{
			whichAxis = &which_axis;
		}

		*whichAxis = X;

		if (fabsf(y) > fabsf(x))
		{
			if (fabsf(z) > fabsf(y))
				*whichAxis = Z;
			else
				*whichAxis = Y;
		}
		else if (fabsf(z) > fabsf(x))
		{
			*whichAxis = Z;
		}
		return *this[*whichAxis];
	}

	Vertex(float _x, float _y, float _z)
	{
		this->x = _x;
		this->y = _y;
		this->z = _z;
		//*this = D3DXVECTOR3(_x, _y, _z);
	}

	EXTERN Vertex& Rotate(const Vertex& axis, const float angle);


	void Scale(float scale)
	{
		this->x *= scale;
		this->y *= scale;
		this->z *= scale;
	}

	Vertex& ProjectToPlane(const Vertex& normal)
	{


		float perp_component = D3DXVec3Dot(this, &normal);

		*this -= normal * perp_component;

		return *this;

	}

	Vertex& RotateToPlane(const Vertex& normal)
	{
		// get the length of the vector
		float length = Length();

		// Project the vector onto the plane
		ProjectToPlane(normal);

		// now we've projected it ontot he plane
		// we need to handle the case where it dissapears into a point
		// which would indicate that we were perpendicular to the plane
		// so need to get an arbitary vector in the plane	
		float  projected_length = Length();
		if (projected_length == 0.0f) 		// is this a valid comparision?
		{
			// Rotate vector through -90 degrees about Y then +90 about X
			this->x = -normal.z;
			this->y = normal.x;
			this->z = -normal.y;
		}

		// get unit vector in this direction
		Normalize();

		// multiply by original speed to rotate velocity onto plane
		*this *= length;
		return *this;
	}

	Vertex& RotateToNormal(const Vertex& normal)
	{

		*this = *(Vertex*)&(normal * Length());

		return *this;
	}

	Vertex& ProjectToNormal(const Vertex& normal)
	{

		float dot = D3DXVec3Dot(this, &normal);
		*this = *(Vertex*)&(normal * dot);

		return *this;
	}

	inline Vertex& Normalize(float len)
	{


		float l = Length();
		if (l > 0.0f)
		{
			l = len / l;

			this->x *= l;
			this->y *= l;
			this->z *= l;
		}
		else
		{
			//		Dbg_MsgAssert(0,("Normalizing vector of zero length"));
		}
		return *this;
	}

	void Normalize()
	{
		float l = Length();

		if (l > 0.0f)
		{
			l = 1.0f / l;

			this->x *= l;
			this->y *= l;
			this->z *= l;
		}
		else
		{

			this->x *= 0.5f;
			this->y *= 0.5f;
			this->z *= 0.5f;
			//MessageBox(0,("Normalizing vector of zero length"), "", 0);
		}
	}

	float LengthSqr()
	{
		return this->x * this->x + this->y * this->y + this->z * this->z;
	}

	float Length()
	{
		return sqrtf(LengthSqr());
	}

	/*D3DXVECTOR3* operator = (Vertex* v)
	{
		return (D3DXVECTOR3*)v;
	}

	Vertex& operator -= (const Vertex& v)
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return *this;
	}

	Vertex& operator += (const Vertex& v)
	{
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}

	Vertex& operator *= (float f)
	{
		x *= f;
		y *= f;
		z *= f;
		return *this;
	}

	Vertex&
		operator /= (float f)
	{
		float fInv = 1.0f / f;
		x *= fInv;
		y *= fInv;
		z *= fInv;
		return *this;
	}


	// unary operators
	Vertex operator + () const
	{
		return *this;
	}

	Vertex operator - () const
	{
		return Vertex(-x, -y, -z);
	}


	// binary operators
	Vertex operator + (const Vertex& v) const
	{
		return Vertex(x + v.x, y + v.y, z + v.z);
	}

	Vertex operator - (const Vertex& v) const
	{
		return Vertex(x - v.x, y - v.y, z - v.z);
	}

	Vertex operator * (float f) const
	{
		return Vertex(x * f, y * f, z * f);
	}

	Vertex operator / (float f) const
	{
		float fInv = 1.0f / f;
		return Vertex(x * fInv, y * fInv, z * fInv);
	}

	bool
		operator == (const Vertex& v) const
	{
		return x == v.x && y == v.y && z == v.z;
	}

	bool operator != (const Vertex& v) const
	{
		return x != v.x || y != v.y || z != v.z;
	}*/

};


Vertex CrossProduct(const Vertex* vec1, Vertex* vec2);

struct Matrix : D3DXMATRIX
{



	Matrix& CreateRotateMatrix(Matrix& mat, D3DXVECTOR3& axis, const float angle)
	{

		D3DXVECTOR3	unitAxis = axis;
		(*(Vertex*)&unitAxis).Normalize();

		float oneMinusCosine = 1.0f - cosf(angle);

		D3DXVECTOR3	leading;

		leading[X] = 1.0f - (unitAxis[X] * unitAxis[X]);
		leading[Y] = 1.0f - (unitAxis[Y] * unitAxis[Y]);
		leading[Z] = 1.0f - (unitAxis[Z] * unitAxis[Z]);
		leading *= oneMinusCosine;

		D3DXVECTOR3	crossed;

		crossed[X] = (unitAxis[Y] * unitAxis[Z]);
		crossed[Y] = (unitAxis[Z] * unitAxis[X]);
		crossed[Z] = (unitAxis[X] * unitAxis[Y]);
		crossed *= oneMinusCosine;

		unitAxis *= sinf(angle);

		mat.m[RIGHT][X] = 1.0f - leading[X];
		mat.m[RIGHT][Y] = crossed[Z] + unitAxis[Z];
		mat.m[RIGHT][Z] = crossed[Y] - unitAxis[Y];
		mat.m[RIGHT][W] = 0.0f;

		mat.m[UP][X] = crossed[Z] - unitAxis[Z];
		mat.m[UP][Y] = 1.0f - leading[Y];
		mat.m[UP][Z] = crossed[X] + unitAxis[X];
		mat.m[UP][W] = 0.0f;

		mat.m[AT][X] = crossed[Y] + unitAxis[Y];
		mat.m[AT][Y] = crossed[X] - unitAxis[X];
		mat.m[AT][Z] = 1.0f - leading[Z];
		mat.m[AT][W] = 0.0f;

		mat.m[POS][X] = 0.0f;
		mat.m[POS][Y] = 0.0f;
		mat.m[POS][Z] = 0.0f;
		mat.m[POS][W] = 1.0f;

		return mat;
	}
	Matrix()
	{
		D3DXMatrixIdentity(this);
	}

	Matrix(D3DXVECTOR3& axis, const float angle)
	{
		CreateRotateMatrix(*this, axis, angle);
	}

	inline D3DXVECTOR3			Rotate(const D3DXVECTOR3& src) const
	{


		D3DXVECTOR3	v;

		v[X] = src[X] * m[RIGHT][X] + src[Y] * m[UP][X] + src[Z] * m[AT][X];
		v[Y] = src[X] * m[RIGHT][Y] + src[Y] * m[UP][Y] + src[Z] * m[AT][Y];
		v[Z] = src[X] * m[RIGHT][Z] + src[Y] * m[UP][Z] + src[Z] * m[AT][Z];

		return v;
	}

	typedef void(__thiscall* const pOrthoNormalizeAbout)(Matrix* pThis, DWORD axis);
	void OrthoNormalizeAbout(DWORD axis)
	{
		pOrthoNormalizeAbout(0x0040F0E0)(this, axis);
	}

	typedef void(__thiscall* const pRotate)(Matrix* pThis, const Matrix& rot);
	void Rotate(const Matrix& rot)
	{
		pRotate(0x0040F7A0)(this, rot);
	}

	inline Matrix& InvertUniform()
	{


		// Only works for orthonormal Matrix

#if 0	// (Mike) this version is buggy!

		Transpose(src);

		row[POS][X] = -DotProduct(src[POS], src[RIGHT]);
		row[POS][Y] = -DotProduct(src[POS], src[UP]);
		row[POS][Z] = -DotProduct(src[POS], src[AT]);

		return *this;
#else	// (Mike) try this one instead...

	// (Dan) actually, don't use the vu0; it returns before the correct values are safely stored in memory; if you access the results too
	// quickly, you get the wrong answers; this is because the compiler may reorder instructions
#if 0
	// (Ken) or try this one, uses vu0 
		Vu0Invert((sceVu0FMATRIX)this);
		return *this;
#else

	// need to copy the old row,
	// or else it will change on us as we're doing
	// our dot products...

		Vertex oldPos = *(Vertex*)m[POS];

		m[POS][X] = -D3DXVec3Dot((D3DXVECTOR3*)&oldPos, (D3DXVECTOR3*)m[RIGHT]);
		m[POS][Y] = -D3DXVec3Dot((D3DXVECTOR3*)&oldPos, (D3DXVECTOR3*)m[UP]);
		m[POS][Z] = -D3DXVec3Dot((D3DXVECTOR3*)&oldPos, (D3DXVECTOR3*)m[AT]);

		std::swap(m[X][Y], m[Y][X]);
		std::swap(m[X][Z], m[Z][X]);
		std::swap(m[Y][Z], m[Z][Y]);

		return *this;

#endif

#endif
	}


	void Rotate(D3DXVECTOR3 axis, float angle)
	{

		Matrix rot_mat;
		D3DXMatrixIdentity(&rot_mat);
		*this *= CreateRotateMatrix(rot_mat, axis, angle);
	}

	inline float			Determinant(void) const
	{


		return m[X][X] * Determinant3(m[Y][Y], m[Z][Y], m[W][Y], m[Y][Z], m[Z][Z], m[W][Z], m[Y][W], m[Z][W], m[W][W])
			- m[X][Y] * Determinant3(m[Y][X], m[Z][X], m[W][X], m[Y][Z], m[Z][Z], m[W][Z], m[Y][W], m[Z][W], m[W][W])
			+ m[X][Z] * Determinant3(m[Y][X], m[Z][X], m[W][X], m[Y][Y], m[Z][Y], m[W][Y], m[Y][W], m[Z][W], m[W][W])
			- m[X][W] * Determinant3(m[Y][X], m[Z][X], m[W][X], m[Y][Y], m[Z][Y], m[W][Y], m[Y][Z], m[Z][Z], m[W][Z]);
	}


	inline Matrix& Adjoint(const Matrix& src)
	{


		m[X][X] = Determinant3(src.m[Y][Y], src.m[Z][Y], src.m[W][Y], src.m[Y][Z], src.m[Z][Z], src.m[W][Z], src.m[Y][W], src.m[Z][W], src.m[W][W]);
		m[Y][X] = -Determinant3(src.m[Y][X], src.m[Z][X], src.m[W][X], src.m[Y][Z], src.m[Z][Z], src.m[W][Z], src.m[Y][W], src.m[Z][W], src.m[W][W]);
		m[Z][X] = Determinant3(src.m[Y][X], src.m[Z][X], src.m[W][X], src.m[Y][Y], src.m[Z][Y], src.m[W][Y], src.m[Y][W], src.m[Z][W], src.m[W][W]);
		m[W][X] = -Determinant3(src.m[Y][X], src.m[Z][X], src.m[W][X], src.m[Y][Y], src.m[Z][Y], src.m[W][Y], src.m[Y][Z], src.m[Z][Z], src.m[W][Z]);

		m[X][Y] = -Determinant3(src.m[X][Y], src.m[Z][Y], src.m[W][Y], src.m[X][Z], src.m[Z][Z], src.m[W][Z], src.m[X][W], src.m[Z][W], src.m[W][W]);
		m[Y][Y] = Determinant3(src.m[X][X], src.m[Z][X], src.m[W][X], src.m[X][Z], src.m[Z][Z], src.m[W][Z], src.m[X][W], src.m[Z][W], src.m[W][W]);
		m[Z][Y] = -Determinant3(src.m[X][X], src.m[Z][X], src.m[W][X], src.m[X][Y], src.m[Z][Y], src.m[W][Y], src.m[X][W], src.m[Z][W], src.m[W][W]);
		m[W][Y] = Determinant3(src.m[X][X], src.m[Z][X], src.m[W][X], src.m[X][Y], src.m[Z][Y], src.m[W][Y], src.m[X][Z], src.m[Z][Z], src.m[W][Z]);

		m[X][Z] = Determinant3(src.m[X][Y], src.m[Y][Y], src.m[W][Y], src.m[X][Z], src.m[Y][Z], src.m[W][Z], src.m[X][W], src.m[Y][W], src.m[W][W]);
		m[Y][Z] = -Determinant3(src.m[X][X], src.m[Y][X], src.m[W][X], src.m[X][Z], src.m[Y][Z], src.m[W][Z], src.m[X][W], src.m[Y][W], src.m[W][W]);
		m[Z][Z] = Determinant3(src.m[X][X], src.m[Y][X], src.m[W][X], src.m[X][Y], src.m[Y][Y], src.m[W][Y], src.m[X][W], src.m[Y][W], src.m[W][W]);
		m[W][Z] = -Determinant3(src.m[X][X], src.m[Y][X], src.m[W][X], src.m[X][Y], src.m[Y][Y], src.m[W][Y], src.m[X][Z], src.m[Y][Z], src.m[W][Z]);

		m[X][W] = -Determinant3(src.m[X][Y], src.m[Y][Y], src.m[Z][Y], src.m[X][Z], src.m[Y][Z], src.m[Z][Z], src.m[X][W], src.m[Y][W], src.m[Z][W]);
		m[Y][W] = Determinant3(src.m[X][X], src.m[Y][X], src.m[Z][X], src.m[X][Z], src.m[Y][Z], src.m[Z][Z], src.m[X][W], src.m[Y][W], src.m[Z][W]);
		m[Z][W] = -Determinant3(src.m[X][X], src.m[Y][X], src.m[Z][X], src.m[X][Y], src.m[Y][Y], src.m[Z][Y], src.m[X][W], src.m[Y][W], src.m[Z][W]);
		m[W][W] = Determinant3(src.m[X][X], src.m[Y][X], src.m[Z][X], src.m[X][Y], src.m[Y][Y], src.m[Z][Y], src.m[X][Z], src.m[Y][Z], src.m[Z][Z]);

		return *this;

	}


	inline Matrix& Invert(const Matrix& src)
	{


		Adjoint(src);

		float det = src.Determinant();

		if (det != 1.0f)
		{
			*this *= (1.0f / det);
		}

		return *this;
	}

	inline Matrix& Invert(void)
	{


		Matrix	d;

		d.Invert(*this);

		*this = d;

		return *this;
	}

	void GetRotationAxisAndAngle(Vertex* pAxis, float* pRadians, D3DXVECTOR3& start, D3DXVECTOR3& end, bool hip_transfer = false)
	{
		float		nTwoSinTheta, nTwoCosTheta;
		Vertex	vTwoSinThetaAxis;
		bool reverse = false;

		nTwoCosTheta = m[RIGHT][X] + m[UP][Y] + m[AT][Z] - 1.0f;

		float xDiff = fabsf(start.x - end.x);
		if (xDiff >= 10.0f)//x diff is big enough, so apply rotation on Z axis
			vTwoSinThetaAxis.z = m[RIGHT][Y] - m[UP][X];
		else
			vTwoSinThetaAxis.z = 0.0f;
		float zDiff = fabsf(start.z - end.z);
		if (zDiff >= 10.0f)//z diff is big enough, so apply rotation on X axis
			vTwoSinThetaAxis.x = -m[UP][Z] - m[AT][Y];
		else
			vTwoSinThetaAxis.x = 0.0f;
		vTwoSinThetaAxis.y = 0.0f;// don't rotate Y since this bugs us..

		//If we have rotation on both X and Z it means we do a hip_transfer, or our velocity made us drift in the QP
		if (vTwoSinThetaAxis.x && vTwoSinThetaAxis.z) [[unlikely]]
		{
			if (hip_transfer) [[unlikely]]//hip_transfer, keeep both rotations
			{
				_printf("An unlikely event..\n");
		        /*vTwoSinThetaAxis.x = m[UP][Z] - m[AT][Y];
                vTwoSinThetaAxis.z = m[UP][X] - m[RIGHT][Y];*/
			}
			else//need to check if we just drifted or if this is a non vertical ramp
			{
				_printf("drifted?");
				if (xDiff > zDiff)
				{
					if ((xDiff - zDiff) > 80.0f)//we just drifted, so rotate ONLY on the longest distance
					{
						vTwoSinThetaAxis.x = 0.0f;
						_printf(" YES\n");
					}
					else
					{
						_printf(" NO 1\n");
						//vTwoSinThetaAxis.x = m[UP][Z] - m[AT][Y];
						//vTwoSinThetaAxis.y = m[AT][X] - m[RIGHT][Z];
						///vTwoSinThetaAxis.z = 0;// m[UP][X] - m[RIGHT][Y];
						//reverse = true;
						vTwoSinThetaAxis.x = m[UP][Z] - m[AT][Y];
						vTwoSinThetaAxis.y = m[AT][X] - m[RIGHT][Z];
						vTwoSinThetaAxis.z = m[RIGHT][Y] - m[UP][X];
					}
				}
				else
				{
					if ((zDiff - xDiff) > 80.0f)//we just drifted, so rotate ONLY on the longest distance
					{
						vTwoSinThetaAxis.z = 0.0f;
						_printf(" YES\n");
					}
					else
					{
						//reverse = true;
						_printf(" NO 2\n");
						vTwoSinThetaAxis.x = m[UP][Z] - m[AT][Y];
						vTwoSinThetaAxis.y = m[AT][X] - m[RIGHT][Z];
						vTwoSinThetaAxis.z = m[RIGHT][Y] - m[UP][X];
						//vTwoSinThetaAxis.z = 0.5f;
						//vTwoSinThetaAxis.z = 0.5f;
						//vTwoSinThetaAxis.z = -m[RIGHT][Y] - m[UP][X];// *= -1.0f;// m[UP][X] - m[RIGHT][Y];
						/*vTwoSinThetaAxis.x = 0;// m[UP][Z] - m[AT][Y];
						//vTwoSinThetaAxis.y =  m[AT][X] - m[RIGHT][Z];
						vTwoSinThetaAxis.z = 0;// m[UP][X] - m[RIGHT][Y];*/
					}
				}
			}

		}
		/*vTwoSinThetaAxis.x = -m[UP][Z] - m[AT][Y];
		vTwoSinThetaAxis.y = 0;// -m[UP][Z] - m[AT][Y];
		vTwoSinThetaAxis.z = 0;// -m[UP][Z] - m[AT][Y];*/
			// 0.0f;// m[RIGHT][Y] - m[UP][X];
		_printf("theta %f %f %f\nstart %f %f\nend %f %f\n", vTwoSinThetaAxis.x, vTwoSinThetaAxis.y, vTwoSinThetaAxis.z, start.x, start.z, end.x, end.z);

		/*

			vTwoSinThetaAxis.x = -m[UP][Z] - m[AT][Y];
			vTwoSinThetaAxis.y = 0.0f;// m[AT][X] - m[RIGHT][Z];


			*/

			//vTwoSinThetaAxis.z = 0.0f;

		/*if (vTwoSinThetaAxis.x >= 0.01f)
			vTwoSinThetaAxis.z = 0;*/
			//if (vTwoSinThetaAxis.z >= 0.1f || vTwoSinThetaAxis.z <= -0.1f)


				/*

			if((vTwoSinThetaAxis.z < -0.005f || vTwoSinThetaAxis.z >= 0.035f) && vTwoSinThetaAxis.x >= -1.003f && vTwoSinThetaAxis.x < 0.0f)
				vTwoSinThetaAxis.x = 0;
			else
				vTwoSinThetaAxis.z = 0;
			*/


			/*if ((vTwoSinThetaAxis.x < -0.75f && vTwoSinThetaAxis.x >= -1.0f))
			{
				_printf("Inverting theta\n");
				vTwoSinThetaAxis.x = 0.0f;// m[UP][Z] - m[AT][Y];
				vTwoSinThetaAxis.y = 0.0f;// m[AT][X] - m[RIGHT][Z];
				vTwoSinThetaAxis.z = m[RIGHT][Y] - m[UP][X];
			}*/

		_printf("theta %f %f %f\n", vTwoSinThetaAxis.x, vTwoSinThetaAxis.y, vTwoSinThetaAxis.z);

		// Gary:  There used to be a magic patch added by Dave
		// (basically negating the axis) which made it work with
		// the RW-based Slerp object.  This doesn't seem to be
		// necessary any more, but I'm going to leave it
		// in the code just in case our Slerp stops working...
		/*vTwoSinThetaAxis.x = m[AT][Y] - m[UP][Z];
		vTwoSinThetaAxis.y = m[RIGHT][Z] - m[AT][X];
		vTwoSinThetaAxis.y = m[UP][X] - m[RIGHT][Y];*/
		//vTwoSinThetaAxis.z = m[UP][X] - m[RIGHT][Y] ;
		//vTwoSinThetaAxis.W = 1.0f;

		nTwoSinTheta = vTwoSinThetaAxis.Length();

		if (nTwoSinTheta > 0.0f)
		{
			float  recipLength = (1.0f / (nTwoSinTheta));

			*pAxis = vTwoSinThetaAxis;
			pAxis->Scale(recipLength);
		}
		else
		{
			*pAxis = Vertex(0.0f, 0.0f, 0.0f);
		}

		(*pRadians) = (float)atan2(nTwoSinTheta, nTwoCosTheta);
		if ((nTwoSinTheta <= 0.01f) && (nTwoCosTheta <= 0.0f))
		{
			_printf("\n\nERROR THETA\n\n");
			/*
			* sin theta is 0; cos theta is -1; theta is 180 degrees
			* vTwoSinThetaAxis was degenerate
			* axis will have to be found another way.
			*/

			//Vector	vTwoSinThetaAxis;

			/*
			* Matrix is:
			* [ [ 2 a_x^2 - 1,  2 a_x a_y,   2 a_x a_z,  0 ]
			*   [  2 a_x a_y,  2 a_y^2 - 1,  2 a_y a_z,  0 ]
			*   [  2 a_x a_z,   2 a_y a_z,  2 a_z^2 - 1, 0 ]
			*   [      0,           0,           0,      1 ] ]
			* Build axis scaled by 4 * component of maximum absolute value
			*/

			if (m[RIGHT][X] > m[UP][Y])
			{
				if (m[RIGHT][X] > m[AT][Z])
				{
					vTwoSinThetaAxis.x = 1.0f + m[RIGHT][X];
					vTwoSinThetaAxis.x = vTwoSinThetaAxis.x + vTwoSinThetaAxis.x;
					vTwoSinThetaAxis.y = m[RIGHT][Y] + m[UP][X];
					vTwoSinThetaAxis.z = m[RIGHT][Z] + m[AT][X];
				}
				else
				{
					vTwoSinThetaAxis.z = 1.0f + m[AT][Z];
					vTwoSinThetaAxis.z = vTwoSinThetaAxis.z + vTwoSinThetaAxis.z;
					vTwoSinThetaAxis.x = m[AT][X] + m[RIGHT][Z];
					vTwoSinThetaAxis.y = m[AT][Y] + m[UP][Z];
				}
			}
			else
			{
				if (m[UP][Y] > m[AT][Z])
				{
					vTwoSinThetaAxis.y = 1.0f + m[UP][Y];
					vTwoSinThetaAxis.y = vTwoSinThetaAxis.y + vTwoSinThetaAxis.y;
					vTwoSinThetaAxis.z = m[UP][Z] + m[AT][Y];
					vTwoSinThetaAxis.x = m[UP][X] + m[RIGHT][Y];
				}
				else
				{
					vTwoSinThetaAxis.z = 1.0f + m[AT][Z];
					vTwoSinThetaAxis.z = vTwoSinThetaAxis.z + vTwoSinThetaAxis.z;
					vTwoSinThetaAxis.x = m[AT][X] + m[RIGHT][Z];
					vTwoSinThetaAxis.y = m[AT][Y] + m[UP][Z];
				}
			}

			/*
			* and normalize the axis
			*/

			*pAxis = vTwoSinThetaAxis;
			pAxis->Normalize();
		}
		if (reverse)
		{
			*pAxis = *(Vertex*)&(-*pAxis);
			(*pRadians) = (2.0f * D3DX_PI) - (*pRadians);
		}
	}

	Matrix& OrthoNormalizeAbout2(int r0)
	{
		int r1, r2;
		r1 = r0 + 1;
		if (r1 == 3)
		{
			r1 = 0;
		}
		r2 = r1 + 1;
		if (r2 == 3)
		{
			r2 = 0;
		}
		// Now regarding Rows r0,r1,r2
		// r0 = r1 x r2	   (implied)
		// r1 = r2 x r0	   (calculate this)
		// r2 = r0 x r1	   (and this)
		//
		// We need to recalculate rows r1 and r2 using the above cross produces
		// however if r0 is close to r2, then the calculation of r1 will be off
		// so it's better to calulate r2 and then r1 
		// the first pair to do will be whichever has the smaller dot product

		if (fabsf(D3DXVec3Dot((D3DXVECTOR3*)(m[r2]), (D3DXVECTOR3*)(m[r0]))) < fabsf(D3DXVec3Dot((D3DXVECTOR3*)(m[r0]), (D3DXVECTOR3*)(m[r1]))))
		{
			*(Vertex*)(m[r1]) = CrossProduct((Vertex*)(m[r2]), (Vertex*)(m[r0]));
			(*(Vertex*)(m[r1])).Normalize();
			*(Vertex*)(m[r2]) = CrossProduct((Vertex*)(m[r0]), (Vertex*)(m[r1]));
			(*(Vertex*)(m[r2])).Normalize();
		}
		else
		{
			*(Vertex*)(m[r2]) = CrossProduct((Vertex*)(m[r0]), (Vertex*)(m[r1]));
			(*(Vertex*)(m[r2])).Normalize();
			*(Vertex*)(m[r1]) = CrossProduct((Vertex*)(m[r2]), (Vertex*)(m[r0]));
			(*(Vertex*)(m[r1])).Normalize();
		}

		return *this;

	}


	Matrix& CreateRotateXYMatrix(Matrix& rotY, float angleX, float angleY)
	{


		float	sX = sinf(angleX);
		float	cX = cosf(angleX);

		float	sY = sinf(angleY);
		float	cY = cosf(angleY);

		rotY.m[RIGHT][X] = cY;
		rotY.m[RIGHT][X] = 0.0f;
		rotY.m[RIGHT][Z] = -sY;
		rotY.m[RIGHT][W] = 0.0f;

		rotY.m[UP][X] = 0.0f;
		rotY.m[UP][Y] = cX;
		rotY.m[UP][Z] = sX;
		rotY.m[UP][Z] = 0.0f;
		rotY.m[UP][W] = 0.0f;

		rotY.m[AT][X] = sY;
		rotY.m[AT][Y] = -sX;
		rotY.m[AT][Z] = cY * cX;
		rotY.m[AT][W] = 0.0f;

		rotY.m[POS][X] = 0.0f;
		rotY.m[POS][Y] = 0.0f;
		rotY.m[POS][Z] = 0.0f;
		rotY.m[POS][W] = 1.0f;

		return rotY;
	}


	Matrix& CreateRotateXMatrix(Matrix& rotX, float angle)
	{


		float	s = sinf(angle);
		float	c = cosf(angle);

		rotX.m[RIGHT][X] = 1.0f;
		rotX.m[RIGHT][Y] = 0.0f;
		rotX.m[RIGHT][Z] = 0.0f;
		rotX.m[RIGHT][W] = 0.0f;

		rotX.m[UP][X] = 0.0f;
		rotX.m[UP][Y] = c;
		rotX.m[UP][Z] = s;
		rotX.m[UP][W] = 0.0f;

		rotX.m[AT][X] = 0.0f;
		rotX.m[AT][Y] = -s;
		rotX.m[AT][Z] = c;
		rotX.m[AT][W] = 0.0f;

		rotX.m[POS][X] = 0.0f;
		rotX.m[POS][Y] = 0.0f;
		rotX.m[POS][Z] = 0.0f;
		rotX.m[POS][W] = 1.0f;

		return rotX;
	}
	static Matrix& CreateRotateYMatrix(Matrix& rotY, float angle)
	{


		float	s = sinf(angle);
		float	c = cosf(angle);

		rotY.m[RIGHT][X] = c;
		rotY.m[RIGHT][Y] = 0.0f;
		rotY.m[RIGHT][Z] = -s;
		rotY.m[RIGHT][W] = 0.0f;

		rotY.m[UP][X] = 0.0f;
		rotY.m[UP][Y] = 1.0f;
		rotY.m[UP][Z] = 0.0f;
		rotY.m[UP][W] = 0.0f;

		rotY.m[AT][X] = s;
		rotY.m[AT][Y] = 0.0f;
		rotY.m[AT][Z] = c;
		rotY.m[AT][W] = 0.0f;

		rotY.m[POS][X] = 0.0f;
		rotY.m[POS][Y] = 0.0f;
		rotY.m[POS][Z] = 0.0f;
		rotY.m[POS][W] = 1.0f;

		return rotY;
	}

	const Vertex& operator[](int index) const
	{
		return *(Vertex*)&m[index];
	}

	Vertex& operator[](int index)
	{
		return *(Vertex*)&m[index];
	}


	inline Matrix& RotateYLocal(const float angle)
	{

		CreateRotateYMatrix(*this, angle)** this;

		return *this;
	}

	inline Matrix& RotateXYLocal(const float angleX, float angleY)
	{

		CreateRotateXYMatrix(*this, angleX, angleY)** this;

		return *this;
	}

	inline Matrix& RotateXLocal(const float angle)
	{

		CreateRotateXMatrix(*this, angle)** this;

		return *this;
	}




};

void InitialRand(int a);
int Rnd(int n);

float AngleY(D3DXMATRIX& orient, D3DXVECTOR3& from, D3DXVECTOR3& to);
float GetAngle(const Matrix& currentOrientation, const Vertex& desiredHeading, int headingAxis, int rotAxis);
float GetAngleAbout(Vertex& v1, Vertex& v2, Vertex& Axis);
float	Sgn(float x);
int ClampValue(int value, int min, int max, int outMin, int outMax);
inline const float GetSpeed(const Vertex* const __restrict vertex);
Vertex GetNormal(Vertex* v);
#endif