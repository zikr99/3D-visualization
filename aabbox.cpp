#include "aabbox.h"
#include "matrix4.h"

void aabbox::getTransformed(const matrix4& mat, aabbox& box)
{
       CVector3 pa(MaxEdge.x, MaxEdge.y, MaxEdge.z);
       CVector3 pb(MaxEdge.x, MinEdge.y, MaxEdge.z);
       CVector3 pc(MinEdge.x, MaxEdge.y, MaxEdge.z);
       CVector3 pd(MinEdge.x, MinEdge.y, MaxEdge.z);
       
       CVector3 pe(MaxEdge.x, MaxEdge.y, MinEdge.z);
       CVector3 pf(MaxEdge.x, MinEdge.y, MinEdge.z);
       CVector3 pg(MinEdge.x, MaxEdge.y, MinEdge.z);
       CVector3 ph(MinEdge.x, MinEdge.y, MinEdge.z);

       mat.transformVector3(pa);
       mat.transformVector3(pb);
       mat.transformVector3(pc);
       mat.transformVector3(pd);
       mat.transformVector3(pe);
       mat.transformVector3(pf);
       mat.transformVector3(pg);
       mat.transformVector3(ph);
       
       box.reset(pa.x, pa.y, pa.z);
       box.addInternalPoint(pb);
       box.addInternalPoint(pc);
       box.addInternalPoint(pd);
       box.addInternalPoint(pe);
       box.addInternalPoint(pf);
       box.addInternalPoint(pg);
       box.addInternalPoint(ph);
}
