#ifndef __SVIEWFRUSTRUM_H__
#define __SVIEWFRUSTRUM_H__

#include "matrix4.h"

struct SViewFrustrum
{
      enum VFPLANES
	  {
            VF_FAR_PLANE = 0,
			VF_NEAR_PLANE,
			VF_LEFT_PLANE,
			VF_RIGHT_PLANE,
			VF_BOTTOM_PLANE,
			VF_TOP_PLANE,
			VF_PLANE_COUNT
	  };
	  
      SViewFrustrum() {}
	  SViewFrustrum(const matrix4& mat);

      CPlane planes[VF_PLANE_COUNT];
      
	  CVector3 getFarLeftUp() const;
	  CVector3 getFarLeftDown() const;
	  CVector3 getFarRightUp() const;
      CVector3 getFarRightDown() const;
      CVector3 getOrigin() const;
	  aabbox getBoundingBox() const;
	  
      void transform(matrix4 &mat);
      
};

inline SViewFrustrum::SViewFrustrum(const matrix4& mat)
{
      #define sw(a,b)		(mat((b),(a)))

      // left clipping plane
	  planes[SViewFrustrum::VF_LEFT_PLANE].normal.x = -(sw(0,3) + sw(0,0));
	  planes[SViewFrustrum::VF_LEFT_PLANE].normal.y = -(sw(1,3) + sw(1,0));
	  planes[SViewFrustrum::VF_LEFT_PLANE].normal.z = -(sw(2,3) + sw(2,0));
	  planes[SViewFrustrum::VF_LEFT_PLANE].D = -(sw(3,3) + sw(3,0));
	                                                 
      // right clipping plane                  
	  planes[SViewFrustrum::VF_RIGHT_PLANE].normal.x = -(sw(0,3) - sw(0,0));
	  planes[SViewFrustrum::VF_RIGHT_PLANE].normal.y = -(sw(1,3) - sw(1,0));
	  planes[SViewFrustrum::VF_RIGHT_PLANE].normal.z = -(sw(2,3) - sw(2,0));
	  planes[SViewFrustrum::VF_RIGHT_PLANE].D = -(sw(3,3) - sw(3,0));
	    
      // top clipping plane                  
	  planes[SViewFrustrum::VF_TOP_PLANE].normal.x = -(sw(0,3) - sw(0,1));
	  planes[SViewFrustrum::VF_TOP_PLANE].normal.y = -(sw(1,3) - sw(1,1));
	  planes[SViewFrustrum::VF_TOP_PLANE].normal.z = -(sw(2,3) - sw(2,1));
	  planes[SViewFrustrum::VF_TOP_PLANE].D = -(sw(3,3) - sw(3,1));

	  // bottom clipping plane                  
	  planes[SViewFrustrum::VF_BOTTOM_PLANE].normal.x = -(sw(0,3) + sw(0,1));
	  planes[SViewFrustrum::VF_BOTTOM_PLANE].normal.y = -(sw(1,3) + sw(1,1));
	  planes[SViewFrustrum::VF_BOTTOM_PLANE].normal.z = -(sw(2,3) + sw(2,1));
	  planes[SViewFrustrum::VF_BOTTOM_PLANE].D = -(sw(3,3) + sw(3,1));
	 
	  // near clipping plane                   
	  planes[SViewFrustrum::VF_NEAR_PLANE].normal.x = -sw(0,2);
	  planes[SViewFrustrum::VF_NEAR_PLANE].normal.y = -sw(1,2);
	  planes[SViewFrustrum::VF_NEAR_PLANE].normal.z = -sw(2,2);
	  planes[SViewFrustrum::VF_NEAR_PLANE].D = -sw(3,2);
	                                                 
	  // far clipping plane                    
	  planes[SViewFrustrum::VF_FAR_PLANE].normal.x = -(sw(0,3) - sw(0,2));
	  planes[SViewFrustrum::VF_FAR_PLANE].normal.y = -(sw(1,3) - sw(1,2));
	  planes[SViewFrustrum::VF_FAR_PLANE].normal.z = -(sw(2,3) - sw(2,2));
	  planes[SViewFrustrum::VF_FAR_PLANE].D = -(sw(3,3) - sw(3,2));

      // normalize normals
	  for (int i = 0; i < 6; ++i)
      {
			float len = (float)(1.0f / planes[i].normal.Length());
			planes[i].normal *= len;
			planes[i].D *= len;
	  }
}

inline CVector3 SViewFrustrum::getFarLeftUp() const
{
	  CVector3 p;
	  planes[SViewFrustrum::VF_FAR_PLANE].getIntersectionWithPlanes(
		   planes[SViewFrustrum::VF_TOP_PLANE],
		   planes[SViewFrustrum::VF_LEFT_PLANE], p);
      
	  return p;
}

inline CVector3 SViewFrustrum::getFarLeftDown() const
{
	  CVector3 p;
	  planes[SViewFrustrum::VF_FAR_PLANE].getIntersectionWithPlanes(
	       planes[SViewFrustrum::VF_BOTTOM_PLANE],
		   planes[SViewFrustrum::VF_LEFT_PLANE], p);

      return p;
}

inline CVector3 SViewFrustrum::getFarRightUp() const
{
      CVector3 p;
	  planes[SViewFrustrum::VF_FAR_PLANE].getIntersectionWithPlanes(
		   planes[SViewFrustrum::VF_TOP_PLANE],
		   planes[SViewFrustrum::VF_RIGHT_PLANE], p);

      return p;
}

inline CVector3 SViewFrustrum::getFarRightDown() const
{
      CVector3 p;
	  planes[SViewFrustrum::VF_FAR_PLANE].getIntersectionWithPlanes(
	       planes[SViewFrustrum::VF_BOTTOM_PLANE],
		   planes[SViewFrustrum::VF_RIGHT_PLANE], p);

      return p;
}

inline CVector3 SViewFrustrum::getOrigin() const
{
      CVector3 p;
	  planes[SViewFrustrum::VF_LEFT_PLANE].getIntersectionWithPlanes(
	       planes[SViewFrustrum::VF_RIGHT_PLANE],
		   planes[SViewFrustrum::VF_TOP_PLANE], p);

      return p;
}

inline aabbox SViewFrustrum::getBoundingBox() const
{     
      aabbox box(getOrigin());
      
	  box.addInternalPoint(getFarLeftUp());
	  box.addInternalPoint(getFarRightUp());
	  box.addInternalPoint(getFarLeftDown());
	  box.addInternalPoint(getFarRightDown());
	  box.repair();
	  
	  return box;
}

inline void SViewFrustrum::transform(matrix4 &mat)
{
      for (int i = 0; i < VF_PLANE_COUNT; ++i)
			mat.transformPlane(planes[i]);
}

#endif
