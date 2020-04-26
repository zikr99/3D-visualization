#ifndef __CPLANE_H__
#define __CPLANE_H__

#include "CVector3.h"

class CPlane
{
public:
      // Constructors
	  CPlane(): normal(0.0f, 1.0f, 0.0f) { recalculateD(CVector3(0.0f, 0.0f, 0.0f)); }
      CPlane(const CVector3& Normal, const CVector3& MPoint): normal(Normal) { recalculateD(MPoint); }
      CPlane(scalar_t nx, scalar_t ny, scalar_t nz, scalar_t px, scalar_t py, scalar_t pz): 
            normal(nx, ny, nz) { recalculateD(CVector3(px, py, pz)); } 
      CPlane(const CPlane& other): normal(other.normal), D(other.D) {}
      CPlane(const CVector3& point1, const CVector3& point2, const CVector3& point3)	
            { setPlane(point1, point2, point3); }

      void recalculateD(const CVector3& MPoint)
	  {
			D = -MPoint.DotProduct(normal);
	  }
      
	  void setPlane(const CVector3& nvector, const CVector3& point)
	  {
			normal = nvector;
			normal.Normalize();
			recalculateD(point);
	  }

      void setPlane(const CVector3& nvect, scalar_t d)
	  {
			normal = nvect;
			D = d;
	  }

	  void setPlane(const CVector3& point1, const CVector3& point2, const CVector3& point3)
	  {
			normal = (point2 - point1).CrossProduct(point3 - point1);
			normal.Normalize();

			recalculateD(point1);
	  }

      // operators
	  inline bool operator==(const CPlane& other) const 
            { return (D == other.D && normal == other.normal); }
	  inline bool operator!=(const CPlane& other) const 
            { return !(D == other.D && normal == other.normal); }

	  bool isFrontFacing(const CVector3& lookDirection) const
	  {
			return normal.DotProduct(lookDirection) <= 0.0f;
	  }

	  //! works if the normal is Normalized.
	  scalar_t getDistanceTo(const CVector3& point) const
	  {
      		return point.DotProduct(normal) + D;
	  }

      //! works if the normal is Normalized.
      CVector3 getMemberPoint() const
	  {
			return normal*(-D);
	  }
	  
      bool getIntersectionWithLine(const CVector3& linePoint, const CVector3& lineVect,
			CVector3& outIntersection) const
	  {
			scalar_t t2 = normal.DotProduct(lineVect);
			if ((t2 < 0.00001f) && (t2 > -0.00001f))
				return false;

			scalar_t t = - (normal.DotProduct(linePoint) + D) / t2;			
			outIntersection = linePoint + (lineVect * t);
			return true;
	  }

	  bool existsIntersection(const CPlane& other) const
	  {
			CVector3 cross = other.normal.CrossProduct(normal);
			return cross.Length() > 1e-08f;
	  }

	  bool getIntersectionWithPlane(const CPlane& other, CVector3& outLinePoint,
			CVector3& outLineVect) const
	  {
            if (!existsIntersection(other))
                return false;
                
            outLineVect = normal.CrossProduct(other.normal);    
                
            CVector3 mpoint = getMemberPoint(); 
            CVector3 dvect = normal.CrossProduct(outLineVect); 
            
            return other.getIntersectionWithLine(mpoint, dvect, outLinePoint);
      }
      
      bool getIntersectionWithPlanes(const CPlane& o1, 
			const CPlane& o2, CVector3& outPoint) const
	  {
			CVector3 linePoint, lineVect;
			
			if (getIntersectionWithPlane(o1, linePoint, lineVect))
				return o2.getIntersectionWithLine(linePoint, lineVect, outPoint);

			return false;
	  }
	  			
public:
      CVector3 normal;	
	  scalar_t D;	
	  
};

#endif

