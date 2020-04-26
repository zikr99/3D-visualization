#ifndef __CCOLLISIONMANAGER_H__
#define __CCOLLISIONMANAGER_H__
#define EPSILON 0.03125f

#include "CBSPSceneNode.h"

class CCollisionManager
{
protected:
     CSceneManager *m_scenemanager;     
     
     CBSPMesh *gcpmesh;
     CVector3 gcpinputstart;
     CVector3 gcpinputend;
     float gcpoutputfraction;
     CVector3 gcpoutputpoint;
     bool gcpstartout;
     bool gcpallinsolid;
     
     void CheckNodeCollPoint(int nodeIndex, float startfraction, float endfraction, 
          CVector3 startfractpoint, CVector3 endfractpoint);
     void CheckBrushCollPoint(tBSPBrush *gcpbrush);
     
     CBSPMesh *gcmmesh;
     CVector3 gcmstartposition;
     CVector3 gcmmovvector;
     float gcmradius;
     float gcmoutputfraction;
     
     void CheckNodeCollMov(int nodeIndex, float startfraction, float endfraction, 
          CVector3 startfractpoint, CVector3 endfractpoint);
     void CheckBrushCollMov(tBSPBrush *gcmbrush);
     
     CBSPMesh *gcemesh;
     CVector3 gcestartposition;
     CVector3 gcemovvector;
     CVector3 gceradius;
     float gcemaxradius;
     float gceavgradius;
     float gcescalex;
     float gcescaley;
     float gcescalez;
     float gceoutputfraction;
     CVector3 gcecollpoint;
     matrix4 gcecmat;
     
     void CheckNodeCollMovE(int nodeIndex, float startfraction, float endfraction, 
          CVector3 startfractpoint, CVector3 endfractpoint);
     void CheckTriangleCollMovE(const CVector3 &pa, const CVector3 &pb, const CVector3 &pc);
     bool IsInsideTriangle(const CVector3 &pp, const CVector3 &pa, const CVector3 &pb, const CVector3 &pc);
     CVector3 ClosestPointOnTriangle(const CVector3 &pp, const CVector3 &pa, const CVector3 &pb, 
          const CVector3 &pc);
     CVector3 ClosestPointOnLine(const CVector3 &pp, const CVector3 &pa, const CVector3 &pb);
     float GetSmallerRoot(float a, float b, float c);
          
public:
     CCollisionManager(CSceneManager *mngr): m_scenemanager(mngr) {}
     virtual ~CCollisionManager() {} 
     
     virtual bool getCollisionPoint(const CVector3 &startray, const CVector3 &endray,
          CBSPSceneNode* selector, CVector3 &outCollisionPoint);
     
     virtual bool getCollisionMovement(const CVector3 &startposition, const CVector3 &movement,
          float radius, CBSPSceneNode* selector, CVector3 &outCollisionMovement);
     
     virtual bool getCollisionMovement(const CVector3 &startposition, const CVector3 &movement,
          CVector3 ellipseradius, CBSPSceneNode* selector, CVector3 &outCollisionMovement,
          CVector3 &outNewMovement, CVector3 &collRelPosition, CVector3 &tv, CVector3 &uv, float &ofrac, 
          float &nd);
             
};

#endif
