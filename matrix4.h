#ifndef __MATRIX4_H__
#define __MATRIX4_H__

#include "aabbox.h"

class matrix4
{
public:
      matrix4();
      
	  //! Simple operator for directly accessing every element of the matrix.
	  float& operator()(int row, int col) { return M[col*4 + row]; }
	  const float& operator()(int row, int col) const {  return M[col*4 + row]; }

      //! Returns true if other matrix is equal/not equal to this matrix.
	  bool operator==(const matrix4 &other) const;
	  bool operator!=(const matrix4 &other) const;

	  //! Multiply by another matrix.
	  matrix4& operator*=(const matrix4& other);
	  matrix4 operator*(const matrix4& other) const;
	  
	  //! Sets this matrix equal to the other matrix.
	  matrix4& operator=(const matrix4 &other);

	  //! Set/test matrix with identity. 
	  void makeIdentity();
	  bool isIdentity();
	  
	  //! Calculates inverse of matrix. Slow. Returns the inversed matrix of this one
	  bool makeInverse();
	  bool getInverse(matrix4& out);
	  
	  //! returns transposed matrix
	  matrix4 getTransposed();
	  void setTranslation(const CVector3& translation);
	  CVector3 getTranslation() const;
	  void setRotation(float alpha, float beta, float gamma);
	  void setRotation(CVector3 rotation);
	  void setScale(const CVector3& scale);
   
      //
	  void buildProjectionMatrix(float fovyrad, float aspect, float znear, float zfar);
	  void buildCameraLookAtMatrixRH(const CVector3& position, const CVector3& target,
           const CVector3& upVector);
           
	  //! Multiplies this matrix by a 1x4 matrix
	  void multiplyWith1x4Matrix(float* matrix) const;
	  void transformVector3(CVector3 &vect) const;
	  void transformVector3(const CVector3 &in, CVector3 &out) const;
	  void transformPlane(CPlane &plane) const;
	  void transformPlane(const CPlane &in, CPlane &out) const;
	  
	  //! Matrix data, stored in column-major order
	  float M[16];
	  
};

inline matrix4::matrix4()
{
      makeIdentity();
}

inline bool matrix4::operator==(const matrix4 &other) const
{
      for (int i = 0; i < 16; ++i)
			if (M[i] != other.M[i])
				return false;

      return true;
}

inline bool matrix4::operator!=(const matrix4 &other) const
{
	  return !(*this == other);
}

//! multiply by another matrix
inline matrix4& matrix4::operator*=(const matrix4& other)
{
      float newMatrix[16];
      const float *m1 = M, *m2 = other.M;

	  newMatrix[0] = m1[0]*m2[0] + m1[4]*m2[1] + m1[8]*m2[2] + m1[12]*m2[3];
	  newMatrix[1] = m1[1]*m2[0] + m1[5]*m2[1] + m1[9]*m2[2] + m1[13]*m2[3];
      newMatrix[2] = m1[2]*m2[0] + m1[6]*m2[1] + m1[10]*m2[2] + m1[14]*m2[3];
	  newMatrix[3] = m1[3]*m2[0] + m1[7]*m2[1] + m1[11]*m2[2] + m1[15]*m2[3];
		
	  newMatrix[4] = m1[0]*m2[4] + m1[4]*m2[5] + m1[8]*m2[6] + m1[12]*m2[7];
	  newMatrix[5] = m1[1]*m2[4] + m1[5]*m2[5] + m1[9]*m2[6] + m1[13]*m2[7];
	  newMatrix[6] = m1[2]*m2[4] + m1[6]*m2[5] + m1[10]*m2[6] + m1[14]*m2[7];
	  newMatrix[7] = m1[3]*m2[4] + m1[7]*m2[5] + m1[11]*m2[6] + m1[15]*m2[7];
		
	  newMatrix[8] = m1[0]*m2[8] + m1[4]*m2[9] + m1[8]*m2[10] + m1[12]*m2[11];
	  newMatrix[9] = m1[1]*m2[8] + m1[5]*m2[9] + m1[9]*m2[10] + m1[13]*m2[11];
	  newMatrix[10] = m1[2]*m2[8] + m1[6]*m2[9] + m1[10]*m2[10] + m1[14]*m2[11];
	  newMatrix[11] = m1[3]*m2[8] + m1[7]*m2[9] + m1[11]*m2[10] + m1[15]*m2[11];
		
	  newMatrix[12] = m1[0]*m2[12] + m1[4]*m2[13] + m1[8]*m2[14] + m1[12]*m2[15];
	  newMatrix[13] = m1[1]*m2[12] + m1[5]*m2[13] + m1[9]*m2[14] + m1[13]*m2[15];
	  newMatrix[14] = m1[2]*m2[12] + m1[6]*m2[13] + m1[10]*m2[14] + m1[14]*m2[15];
	  newMatrix[15] = m1[3]*m2[12] + m1[7]*m2[13] + m1[11]*m2[14] + m1[15]*m2[15];
		
      for (int i=0; i<16; ++i)
	       M[i] = newMatrix[i];

      return *this;
}

inline matrix4 matrix4::operator*(const matrix4& other) const
{
	  matrix4 tmtrx;
	  const float *m1 = M, *m2 = other.M;
	  float *m3 = tmtrx.M;

	  m3[0] = m1[0]*m2[0] + m1[4]*m2[1] + m1[8]*m2[2] + m1[12]*m2[3];
	  m3[1] = m1[1]*m2[0] + m1[5]*m2[1] + m1[9]*m2[2] + m1[13]*m2[3];
	  m3[2] = m1[2]*m2[0] + m1[6]*m2[1] + m1[10]*m2[2] + m1[14]*m2[3];
	  m3[3] = m1[3]*m2[0] + m1[7]*m2[1] + m1[11]*m2[2] + m1[15]*m2[3];
		
	  m3[4] = m1[0]*m2[4] + m1[4]*m2[5] + m1[8]*m2[6] + m1[12]*m2[7];
	  m3[5] = m1[1]*m2[4] + m1[5]*m2[5] + m1[9]*m2[6] + m1[13]*m2[7];
	  m3[6] = m1[2]*m2[4] + m1[6]*m2[5] + m1[10]*m2[6] + m1[14]*m2[7];
	  m3[7] = m1[3]*m2[4] + m1[7]*m2[5] + m1[11]*m2[6] + m1[15]*m2[7];
		
	  m3[8] = m1[0]*m2[8] + m1[4]*m2[9] + m1[8]*m2[10] + m1[12]*m2[11];
	  m3[9] = m1[1]*m2[8] + m1[5]*m2[9] + m1[9]*m2[10] + m1[13]*m2[11];
	  m3[10] = m1[2]*m2[8] + m1[6]*m2[9] + m1[10]*m2[10] + m1[14]*m2[11];
	  m3[11] = m1[3]*m2[8] + m1[7]*m2[9] + m1[11]*m2[10] + m1[15]*m2[11];
		
      m3[12] = m1[0]*m2[12] + m1[4]*m2[13] + m1[8]*m2[14] + m1[12]*m2[15];
	  m3[13] = m1[1]*m2[12] + m1[5]*m2[13] + m1[9]*m2[14] + m1[13]*m2[15];
	  m3[14] = m1[2]*m2[12] + m1[6]*m2[13] + m1[10]*m2[14] + m1[14]*m2[15];
	  m3[15] = m1[3]*m2[12] + m1[7]*m2[13] + m1[11]*m2[14] + m1[15]*m2[15];

	  return tmtrx;
}

inline matrix4& matrix4::operator=(const matrix4 &other)
{
	  for (int i = 0; i < 16; ++i)
			M[i] = other.M[i];

	  return *this;
}

inline void matrix4::makeIdentity()
{
      for (int i = 0; i < 16; ++i)
			M[i] = 0.0f;
			
	  M[0] = M[5] = M[10] = M[15] = 1;
}

inline bool matrix4::isIdentity()
{
	  for (int i = 0; i < 4; ++i)
			for (int j = 0; j < 4; ++j)
			    if (j != i)
			    {
				      if ((*this)(i, j) < -0.0000001f ||
					  (*this)(i, j) >  0.0000001f)
					  return false;
			    }
			    else
			    {
				      if ((*this)(i, j) < 0.9999999f ||
					  (*this)(i, j) > 1.0000001f)
					  return false;
			    }

	  return true;
}

inline bool matrix4::makeInverse()
{
      matrix4 temp;

	  if (getInverse(temp))
	  {
			*this = temp;
			return true;
	  }

	  return false;
}

inline bool matrix4::getInverse(matrix4& out)
{
	  /// Calculates the inverse of this Matrix 
	  /// The inverse is calculated using Cramers rule.
	  /// If no inverse exists then 'false' is returned.

	  const matrix4 &m = *this;

	  float d = (m(0, 0) * m(1, 1) - m(1, 0) * m(0, 1)) * (m(2, 2) * m(3, 3) - m(3, 2) * m(2, 3))	- (m(0, 0) * m(2, 1) - m(2, 0) * m(0, 1)) * (m(1, 2) * m(3, 3) - m(3, 2) * m(1, 3))
	          + (m(0, 0) * m(3, 1) - m(3, 0) * m(0, 1)) * (m(1, 2) * m(2, 3) - m(2, 2) * m(1, 3))	+ (m(1, 0) * m(2, 1) - m(2, 0) * m(1, 1)) * (m(0, 2) * m(3, 3) - m(3, 2) * m(0, 3))
			  - (m(1, 0) * m(3, 1) - m(3, 0) * m(1, 1)) * (m(0, 2) * m(2, 3) - m(2, 2) * m(0, 3))	+ (m(2, 0) * m(3, 1) - m(3, 0) * m(2, 1)) * (m(0, 2) * m(1, 3) - m(1, 2) * m(0, 3));
		
	  if (d == 0.f)
			return false;

	  d = 1.f / d;

	  out(0, 0) = d * (m(1, 1) * (m(2, 2) * m(3, 3) - m(3, 2) * m(2, 3)) + m(2, 1) * (m(3, 2) * m(1, 3) - m(1, 2) * m(3, 3)) + m(3, 1) * (m(1, 2) * m(2, 3) - m(2, 2) * m(1, 3)));
	  out(1, 0) = d * (m(1, 2) * (m(2, 0) * m(3, 3) - m(3, 0) * m(2, 3)) + m(2, 2) * (m(3, 0) * m(1, 3) - m(1, 0) * m(3, 3)) + m(3, 2) * (m(1, 0) * m(2, 3) - m(2, 0) * m(1, 3)));
	  out(2, 0) = d * (m(1, 3) * (m(2, 0) * m(3, 1) - m(3, 0) * m(2, 1)) + m(2, 3) * (m(3, 0) * m(1, 1) - m(1, 0) * m(3, 1)) + m(3, 3) * (m(1, 0) * m(2, 1) - m(2, 0) * m(1, 1)));
	  out(3, 0) = d * (m(1, 0) * (m(3, 1) * m(2, 2) - m(2, 1) * m(3, 2)) + m(2, 0) * (m(1, 1) * m(3, 2) - m(3, 1) * m(1, 2)) + m(3, 0) * (m(2, 1) * m(1, 2) - m(1, 1) * m(2, 2)));
	  out(0, 1) = d * (m(2, 1) * (m(0, 2) * m(3, 3) - m(3, 2) * m(0, 3)) + m(3, 1) * (m(2, 2) * m(0, 3) - m(0, 2) * m(2, 3)) + m(0, 1) * (m(3, 2) * m(2, 3) - m(2, 2) * m(3, 3)));
	  out(1, 1) = d * (m(2, 2) * (m(0, 0) * m(3, 3) - m(3, 0) * m(0, 3)) + m(3, 2) * (m(2, 0) * m(0, 3) - m(0, 0) * m(2, 3)) + m(0, 2) * (m(3, 0) * m(2, 3) - m(2, 0) * m(3, 3)));
	  out(2, 1) = d * (m(2, 3) * (m(0, 0) * m(3, 1) - m(3, 0) * m(0, 1)) + m(3, 3) * (m(2, 0) * m(0, 1) - m(0, 0) * m(2, 1)) + m(0, 3) * (m(3, 0) * m(2, 1) - m(2, 0) * m(3, 1)));
	  out(3, 1) = d * (m(2, 0) * (m(3, 1) * m(0, 2) - m(0, 1) * m(3, 2)) + m(3, 0) * (m(0, 1) * m(2, 2) - m(2, 1) * m(0, 2)) + m(0, 0) * (m(2, 1) * m(3, 2) - m(3, 1) * m(2, 2)));
	  out(0, 2) = d * (m(3, 1) * (m(0, 2) * m(1, 3) - m(1, 2) * m(0, 3)) + m(0, 1) * (m(1, 2) * m(3, 3) - m(3, 2) * m(1, 3)) + m(1, 1) * (m(3, 2) * m(0, 3) - m(0, 2) * m(3, 3)));
	  out(1, 2) = d * (m(3, 2) * (m(0, 0) * m(1, 3) - m(1, 0) * m(0, 3)) + m(0, 2) * (m(1, 0) * m(3, 3) - m(3, 0) * m(1, 3)) + m(1, 2) * (m(3, 0) * m(0, 3) - m(0, 0) * m(3, 3)));
	  out(2, 2) = d * (m(3, 3) * (m(0, 0) * m(1, 1) - m(1, 0) * m(0, 1)) + m(0, 3) * (m(1, 0) * m(3, 1) - m(3, 0) * m(1, 1)) + m(1, 3) * (m(3, 0) * m(0, 1) - m(0, 0) * m(3, 1)));
	  out(3, 2) = d * (m(3, 0) * (m(1, 1) * m(0, 2) - m(0, 1) * m(1, 2)) + m(0, 0) * (m(3, 1) * m(1, 2) - m(1, 1) * m(3, 2)) + m(1, 0) * (m(0, 1) * m(3, 2) - m(3, 1) * m(0, 2)));
	  out(0, 3) = d * (m(0, 1) * (m(2, 2) * m(1, 3) - m(1, 2) * m(2, 3)) + m(1, 1) * (m(0, 2) * m(2, 3) - m(2, 2) * m(0, 3)) + m(2, 1) * (m(1, 2) * m(0, 3) - m(0, 2) * m(1, 3)));
	  out(1, 3) = d * (m(0, 2) * (m(2, 0) * m(1, 3) - m(1, 0) * m(2, 3)) + m(1, 2) * (m(0, 0) * m(2, 3) - m(2, 0) * m(0, 3)) + m(2, 2) * (m(1, 0) * m(0, 3) - m(0, 0) * m(1, 3)));
	  out(2, 3) = d * (m(0, 3) * (m(2, 0) * m(1, 1) - m(1, 0) * m(2, 1)) + m(1, 3) * (m(0, 0) * m(2, 1) - m(2, 0) * m(0, 1)) + m(2, 3) * (m(1, 0) * m(0, 1) - m(0, 0) * m(1, 1)));
	  out(3, 3) = d * (m(0, 0) * (m(1, 1) * m(2, 2) - m(2, 1) * m(1, 2)) + m(1, 0) * (m(2, 1) * m(0, 2) - m(0, 1) * m(2, 2)) + m(2, 0) * (m(0, 1) * m(1, 2) - m(1, 1) * m(0, 2)));

	  return true;
}

//! returns transposed matrix
inline matrix4 matrix4::getTransposed()
{
	  matrix4 t;
	  for (int r = 0; r < 4; ++r)
		    for (int c = 0; c < 4; ++c)
				t(r, c) = (*this)(c, r);
				
	  return t;
}

inline void matrix4::setTranslation(const CVector3& translation)
{
      M[12] = translation.x;
	  M[13] = translation.y;
	  M[14] = translation.z;
}

inline CVector3 matrix4::getTranslation() const
{
      return CVector3(M[12], M[13], M[14]);
}

inline void matrix4::setScale(const CVector3& scale)
{
	  M[0] = scale.x;
	  M[5] = scale.y;
	  M[10] = scale.z;
}

inline void matrix4::setRotation(float gamma, float alpha, float beta)
{
	  double cr = cos(alpha);
	  double sr = sin(alpha);
	  double cp = cos(beta);
	  double sp = sin(beta);
	  double cy = cos(gamma);
	  double sy = sin(gamma);

	  M[0] = (float)(cp*cy);
	  M[1] = (float)(cp*sy);
	  M[2] = (float)(-sp);

	  double srsp = sr*sp;
	  double crsp = cr*sp;

	  M[4] = (float)(srsp*cy - cr*sy);
	  M[5] = (float)(srsp*sy + cr*cy);
	  M[6] = (float)(sr*cp);

	  M[8] = (float)(crsp*cy + sr*sy);
	  M[9] = (float)(crsp*sy - sr*cy);
	  M[10] = (float)(cr*cp);
}

inline void matrix4::setRotation(CVector3 rotation)
{
      setRotation(rotation.x, rotation.y, rotation.z); 
}

inline void matrix4::buildProjectionMatrix(float fovyrad, float aspect, float znear, float zfar)
{      
	  float h = znear*(sin(fovyrad/2)/cos(fovyrad/2));
	  float w = h*aspect;
	
	  float fmn = zfar - znear;
	  float fpn = zfar + znear;
	  float tmb = 2*h;
	  float tpb = 0;
	  float rml = 2*w;
	  float rpl = 0;

	  (*this)(0, 0) = 2*znear/rml;
	  (*this)(1, 0) = 0;
	  (*this)(2, 0) = 0;
	  (*this)(3, 0) = 0;

      (*this)(0, 1) = 0;
	  (*this)(1, 1) = 2*znear/tmb;
	  (*this)(2, 1) = 0;
	  (*this)(3, 1) = 0;

	  (*this)(0, 2) = 0;
	  (*this)(1, 2) = 0;
	  (*this)(2, 2) = -fpn/fmn;
	  (*this)(3, 2) = -1;

	  (*this)(0, 3) = 0;
	  (*this)(1, 3) = 0;
	  (*this)(2, 3) = -2*zfar*znear/fmn;
	  (*this)(3, 3) = 0; 
}

inline void matrix4::buildCameraLookAtMatrixRH(const CVector3& position, const CVector3& target,
       const CVector3& upVector)
{
      //problem if target = up
	  CVector3 up = upVector;
	  up.Normalize();
	
	  CVector3 tgtv = target - position;
	  tgtv.Normalize();

	  float dp = tgtv.DotProduct(up);
	  if ((dp > -1.0001f && dp < -0.9999f) || (dp < 1.0001f && dp > 0.9999f))
		      up.x += 1.0f; 
       
      CVector3 zaxis = position - target;
	  zaxis.Normalize();

	  CVector3 xaxis = up.CrossProduct(zaxis);
	  xaxis.Normalize();

	  CVector3 yaxis = zaxis.CrossProduct(xaxis);

	  (*this)(0, 0) = xaxis.x;
	  (*this)(1, 0) = yaxis.x;
	  (*this)(2, 0) = zaxis.x;
	  (*this)(3, 0) = 0;

	  (*this)(0, 1) = xaxis.y;
	  (*this)(1, 1) = yaxis.y;
	  (*this)(2, 1) = zaxis.y;
	  (*this)(3, 1) = 0;

	  (*this)(0, 2) = xaxis.z;
	  (*this)(1, 2) = yaxis.z;
	  (*this)(2, 2) = zaxis.z;
	  (*this)(3, 2) = 0;

	  (*this)(0, 3) = -xaxis.DotProduct(position);
	  (*this)(1, 3) = -yaxis.DotProduct(position);
	  (*this)(2, 3) = -zaxis.DotProduct(position);
	  (*this)(3, 3) = 1.0f; 
}

//! Multiplies this matrix by a 1x4 matrix
inline void matrix4::multiplyWith1x4Matrix(float* matrix) const
{
      /*
	  0  1  2  3
	  4  5  6  7
	  8  9  10 11
	  12 13 14 15
	  */

	  float mat[4];
	  mat[0] = matrix[0];
	  mat[1] = matrix[1];
	  mat[2] = matrix[2];
	  mat[3] = matrix[3];

	  matrix[0] = M[0]*mat[0] + M[4]*mat[1] + M[8]*mat[2] + M[12]*mat[3];
	  matrix[1] = M[1]*mat[0] + M[5]*mat[1] + M[9]*mat[2] + M[13]*mat[3];
	  matrix[2] = M[2]*mat[0] + M[6]*mat[1] + M[10]*mat[2] + M[14]*mat[3];
	  matrix[3] = M[3]*mat[0] + M[7]*mat[1] + M[11]*mat[2] + M[15]*mat[3];
}

inline void matrix4::transformVector3(CVector3 &vect) const
{
      float vector[3];

	  vector[0] = vect.x*M[0] + vect.y*M[4] + vect.z*M[8] + M[12];
	  vector[1] = vect.x*M[1] + vect.y*M[5] + vect.z*M[9] + M[13];
	  vector[2] = vect.x*M[2] + vect.y*M[6] + vect.z*M[10] + M[14];

	  vect.x = vector[0];
	  vect.y = vector[1];
	  vect.z = vector[2];
}

inline void matrix4::transformVector3(const CVector3 &in, CVector3 &out) const
{
      out = in;
      transformVector3(out);      
}

inline void matrix4::transformPlane(CPlane &plane) const
{
      CVector3 member;
      transformVector3(plane.getMemberPoint(), member);

	  CVector3 origin(0, 0, 0);
	  transformVector3(plane.normal);
	  transformVector3(origin);

	  plane.normal -= origin;
	  plane.D = - member.DotProduct(plane.normal); 
}

inline void matrix4::transformPlane(const CPlane &in, CPlane &out) const
{
      out = in;
      transformPlane(out); 
}

#endif 
 
