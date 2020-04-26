#ifndef __AABBOX_H__
#define __AABBOX_H__

#include "CPlane.h"

class matrix4;
struct SViewFrustrum;

class aabbox
{
public:
      // Constructors
      aabbox(): MinEdge(-1.0f, -1.0f, -1.0f), MaxEdge(1.0f, 1.0f, 1.0f) {}
      aabbox(const CVector3& init): MinEdge(init), MaxEdge(init) {}
	  aabbox(const CVector3& min, const CVector3& max): MinEdge(min), MaxEdge(max) { repair (); }
	  aabbox(scalar_t minx, scalar_t miny, scalar_t minz, scalar_t maxx, scalar_t maxy, scalar_t maxz): 
             MinEdge(minx, miny, minz), MaxEdge(maxx, maxy, maxz) { repair(); }
   
      // operators
	  inline bool operator==(const aabbox& other) const 
             { return (MinEdge == other.MinEdge && other.MaxEdge == MaxEdge); }
      inline bool operator!=(const aabbox& other) const 
             { return !(MinEdge == other.MinEdge && other.MaxEdge == MaxEdge); }
      
      void repair()
	  {
			scalar_t t;

			if (MinEdge.x > MaxEdge.x)
				{ t = MinEdge.x; MinEdge.x = MaxEdge.x; MaxEdge.x = t; }
			if (MinEdge.y > MaxEdge.y)
				{ t = MinEdge.y; MinEdge.y = MaxEdge.y; MaxEdge.y = t; }
			if (MinEdge.z > MaxEdge.z)
				{ t = MinEdge.z; MinEdge.z = MaxEdge.z; MaxEdge.z = t; }
	  }

      void reset(scalar_t x, scalar_t y, scalar_t z)
	  {
			MaxEdge.set(x, y, z);
			MinEdge = MaxEdge;
	  }

	  void reset(const aabbox& initValue)
	  {
	       	*this = initValue;
	  }

      void reset(const CVector3& initValue)
	  {
			MaxEdge = initValue;
			MinEdge = initValue;
	  }
	  
	  void addInternalPoint(scalar_t x, scalar_t y, scalar_t z)
	  {
			if (x > MaxEdge.x) MaxEdge.x = x;
			if (y > MaxEdge.y) MaxEdge.y = y;
			if (z > MaxEdge.z) MaxEdge.z = z;

			if (x < MinEdge.x) MinEdge.x = x;
			if (y < MinEdge.y) MinEdge.y = y;
			if (z < MinEdge.z) MinEdge.z = z;
	  }

	  void addInternalPoint(const CVector3& p)
	  {
			addInternalPoint(p.x, p.y, p.z);
	  }

	  void addInternalBox(const aabbox& b)
	  {
	       	addInternalPoint(b.MaxEdge);
		    addInternalPoint(b.MinEdge);
      }  
      
	  CVector3 getCenter() const
	  {
			return (MinEdge + MaxEdge) / 2;
	  }

      CVector3 getExtent() const
	  {
			return MaxEdge - MinEdge;
	  }

      void getEdges(CVector3 *edges) const
	  {
			CVector3 middle = (MinEdge + MaxEdge) / 2;
			CVector3 diag = middle - MaxEdge;

			/*
			Edges are stored in this way:
                  /1--------/3
                 /  |      / |
                /   |     /  |
                5---------7  |
                |   0- - -| -2
                |  /      |  /
                |/        | /
                4---------6/ 
			*/

			edges[0].set(middle.x + diag.x, middle.y + diag.y, middle.z + diag.z);
			edges[1].set(middle.x + diag.x, middle.y - diag.y, middle.z + diag.z);
			edges[2].set(middle.x + diag.x, middle.y + diag.y, middle.z - diag.z);
			edges[3].set(middle.x + diag.x, middle.y - diag.y, middle.z - diag.z);
			edges[4].set(middle.x - diag.x, middle.y + diag.y, middle.z + diag.z);
			edges[5].set(middle.x - diag.x, middle.y - diag.y, middle.z + diag.z);
			edges[6].set(middle.x - diag.x, middle.y + diag.y, middle.z - diag.z);
			edges[7].set(middle.x - diag.x, middle.y - diag.y, middle.z - diag.z);
	  }
	  
	  bool isEmpty() const
	  {
			CVector3 d = MinEdge - MaxEdge;
			if (d.x < 0) d.x = -d.x;
			if (d.y < 0) d.y = -d.y;
			if (d.z < 0) d.z = -d.z;

			return (d.x < ROUNDING_ERROR && 
					d.y < ROUNDING_ERROR && 
					d.z < ROUNDING_ERROR);
	  }

      bool isPointInside(const CVector3& p) const
	  {
			return (p.x >= MinEdge.x && p.x <= MaxEdge.x &&
					p.y >= MinEdge.y && p.y <= MaxEdge.y &&
					p.z >= MinEdge.z && p.z <= MaxEdge.z);
      }

      bool isPointTotalInside(const CVector3& p) const
	  {
			return (p.x > MinEdge.x && p.x < MaxEdge.x &&
					p.y > MinEdge.y && p.y < MaxEdge.y &&
					p.z > MinEdge.z && p.z < MaxEdge.z);
	  }

      bool intersectsWithBox(const aabbox& other) const
	  {
			return (MinEdge <= other.MaxEdge && MaxEdge >= other.MinEdge);
	  }
	  	  
      void getTransformed(const matrix4& mat, aabbox& box);
	  
public:
      CVector3 MinEdge;
	  CVector3 MaxEdge;
	  
};

#endif
