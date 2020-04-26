#ifndef __CVECTOR3_H__
#define __CVECTOR3_H__

#include <math.h>
#include "specMath.h"

typedef float scalar_t;

class CVector3
{
public:
	 union
	 {
	 	  struct
		  {	
			  scalar_t x;
			  scalar_t y;
		 	  scalar_t z;
		  };
		 
		  scalar_t v[3];
	 };

public:
     CVector3(scalar_t a = 0, scalar_t b = 0, scalar_t c = 0) : x(a), y(b), z(c) {}
     CVector3(const CVector3 &vec) : x(vec.x), y(vec.y), z(vec.z) {}
     
     void set(scalar_t a, scalar_t b, scalar_t c)
     {
          x = a;
          y = b;
          z = c;
     }
     
     void set(const CVector3 &vec)
     {
          x = vec.x;
          y = vec.y;
          z = vec.z;
     }

	 // vector index
	 scalar_t &operator[](const long idx)
	 {
		  return *((&x) + idx);
	 }

     // vector assignment
     const CVector3 &operator=(const CVector3 &vec)
     {
          x = vec.x;
          y = vec.y;
          z = vec.z;

          return *this;
     }

     // vecector equality
     const bool operator==(const CVector3 &vec) const
     {
          return ((x == vec.x) && (y == vec.y) && (z == vec.z));
     }

     // vecector inequality
     const bool operator!=(const CVector3 &vec) const
     {
          return !(*this == vec);
     }
     
     const bool operator<=(const CVector3& other) const 
     { 
          return x <= other.x && y <= other.y && z <= other.z;
     }
     
     const bool operator>=(const CVector3& other) const 
     { 
          return x >= other.x && y >= other.y && z >= other.z;
     }

     // vector add
     const CVector3 operator+(const CVector3 &vec) const
     {
          return CVector3(x + vec.x, y + vec.y, z + vec.z);
     }

     // vector add (opposite of negation)
     const CVector3 operator+() const
     {    
          return CVector3(*this);
     }

     // vector increment
     const CVector3& operator+=(const CVector3& vec)
     {    x += vec.x;
          y += vec.y;
          z += vec.z;
          return *this;
     }

     // vector subtraction
     const CVector3 operator-(const CVector3& vec) const
     {    
          return CVector3(x - vec.x, y - vec.y, z - vec.z);
     }
     
     // vector negation
     const CVector3 operator-() const
     {    
          return CVector3(-x, -y, -z);
     }

     // vector decrement
     const CVector3 &operator-=(const CVector3& vec)
     {
          x -= vec.x;
          y -= vec.y;
          z -= vec.z;

          return *this;
     }

     // scalar self-multiply
     const CVector3 &operator*=(const scalar_t &s)
     {
          x *= s;
          y *= s;
          z *= s;
          
          return *this;
     }

     // scalar self-divecide
     const CVector3 &operator/=(const scalar_t &s)
     {
          const float recip = 1/s; 

          x *= recip;
          y *= recip;
          z *= recip;

          return *this;
     }

     // post multiply by scalar
     const CVector3 operator*(const scalar_t &s) const
     {
          return CVector3(x*s, y*s, z*s);
     }

     // pre multiply by scalar
     friend inline const CVector3 operator*(const scalar_t &s, const CVector3 &vec)
     {
          return vec*s;
     }

	 const CVector3 operator*(const CVector3& vec) const
	 {
		  return CVector3(x*vec.x, y*vec.y, z*vec.z);
 	 }

	 // post multiply by scalar
     /*friend inline const CVector3 operator*(const CVector3 &vec, const scalar_t &s)
     {
          return CVector3(vec.x*s, vec.y*s, vec.z*s);
     }*/

     // divide by scalar
     const CVector3 operator/(scalar_t s) const
     {
          s = 1/s;

          return CVector3(s*x, s*y, s*z);
     }

     // cross product
     const CVector3 CrossProduct(const CVector3 &vec) const
     {
          return CVector3(y*vec.z - z*vec.y, z*vec.x - x*vec.z, x*vec.y - y*vec.x);
     }

     // cross product
     const CVector3 operator^(const CVector3 &vec) const
     {
          return CVector3(y*vec.z - z*vec.y, z*vec.x - x*vec.z, x*vec.y - y*vec.x);
     }

     // dot product
     const scalar_t DotProduct(const CVector3 &vec) const
     {
          return x*vec.x + y*vec.y + z*vec.z;
     }

     // dot product
     const scalar_t operator%(const CVector3 &vec) const
     {
          return x*vec.x + y*vec.y + z*vec.z;
     }

     // length of vector
     const scalar_t Length() const
     {
          return (scalar_t)sqrt((double)(x*x + y*y + z*z));
     }

     // return the unit vector
     const CVector3 UnitVector() const
     {
          return (*this) / Length();
     }

     // normalize this vector
     void Normalize()
     {
          (*this) /= Length();
     }

     const scalar_t operator!() const
     {
          return sqrtf(x*x + y*y + z*z);
     }

     // return vector with specified length
     const CVector3 operator | (const scalar_t length) const
     {
          return *this * (length / !(*this));
     }

     // set length of vector equal to length
     const CVector3& operator |= (const float length)
     {
          return *this = *this | length;
     }

     // return angle between two vectors
     const float inline Angle(const CVector3& normal) const
     {
          return acosf(*this % normal);
     }

     // reflect this vector off surface with normal vector
     const CVector3 inline Reflection(const CVector3& normal) const
     {    
          const CVector3 vec(*this | 1);     // normalize this vector
          return (vec - normal * 2.0 * (vec % normal)) * !*this;
     }

	 // rotate angle degrees about a normal
	 const CVector3 inline Rotate(const float angle, const CVector3& normal) const
	 {	
		  const float cosine = cosf(angle);
		  const float sine = sinf(angle);

		  return CVector3(*this * cosine + ((normal * *this) * (1.0f - cosine)) *
		          normal + (*this ^ normal) * sine);
	 }
};

#endif
