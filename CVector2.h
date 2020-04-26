#ifndef __CVECTOR2_H__
#define __CVECTOR2_H__

#include <math.h>
#include "specMath.h"

template <class T>
class CVector2
{
public:
	 union
	 {
	 	  struct
		  {	
			  T x;
			  T y;
		  };
		 
		  T v[2];
	 };

public:
     CVector2() {}
     CVector2(T a, T b): x(a), y(b) {}
     CVector2(const CVector2 &vec): x(vec.x), y(vec.y) {}
     
     void set(T a, T b)
     {
          x = a;
          y = b;
     }

	 // vector index
	 T &operator[](const long idx)
	 {
		  return *((&x) + idx);
	 }

     // vector assignment
     const CVector2 &operator=(const CVector2 &vec)
     {
          x = vec.x;
          y = vec.y;

          return *this;
     }

     // vecector equality
     const bool operator==(const CVector2 &vec) const
     {
          return ((x == vec.x) && (y == vec.y));
     }

     // vecector inequality
     const bool operator!=(const CVector2 &vec) const
     {
          return !(*this == vec);
     }

     // vector add
     const CVector2 operator+(const CVector2 &vec) const
     {
          return CVector2(x + vec.x, y + vec.y);
     }

     // vector add (opposite of negation)
     const CVector2 operator+() const
     {    
          return CVector2(*this);
     }

     // vector increment
     const CVector2& operator+=(const CVector2& vec)
     {    x += vec.x;
          y += vec.y;
          
          return *this;
     }

     // vector subtraction
     const CVector2 operator-(const CVector2& vec) const
     {    
          return CVector2(x - vec.x, y - vec.y);
     }
     
     // vector negation
     const CVector2 operator-() const
     {    
          return CVector2(-x, -y);
     }

     // vector decrement
     const CVector2 &operator-=(const CVector2& vec)
     {
          x -= vec.x;
          y -= vec.y;

          return *this;
     }

     // scalar self-multiply
     const CVector2 &operator*=(const T &s)
     {
          x *= s;
          y *= s;
          
          return *this;
     }

     // post multiply by scalar
     const CVector2 operator*(const T &s) const
     {
          return CVector2(x*s, y*s);
     }

     // pre multiply by scalar
     friend inline const CVector2 operator*(const T &s, const CVector2 &vec)
     {
          return vec*s;
     }

	 const CVector2 operator*(const CVector2& vec) const
	 {
		  return CVector2(x*vec.x, y*vec.y);
 	 }

	 // post multiply by scalar
     /*friend inline const CVector2 operator*(const CVector2 &vec, const T &s)
     {
          return CVector2(vec.x*s, vec.y*s);
     }*/

     // dot product
     const T DotProduct(const CVector2 &vec) const
     {
          return x*vec.x + y*vec.y;
     }

     // dot product
     const T operator%(const CVector2 &vec) const
     {
          return x*vec.x + y*vec.y;
     }

     // return angle between two vectors
     const float inline Angle(const CVector2& normal) const
     {
          return acosf(*this % normal);
     }
     
};

#endif
