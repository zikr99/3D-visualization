#ifndef __CCAMERASCENENODE_H__
#define __CCAMERASCENENODE_H__

#include "CSceneNode.h"
#include "CameraAction.h"
#include "SViewFrustrum.h"

class CCameraSceneNode: public CSceneNode
{
public:
      CCameraSceneNode(CSceneNode *parent, CSceneManager *mngr,
            const CVector3& lookdir = CVector3(0.0f, 0.0f, -100.0f),
            const CVector3& position = CVector3(0.0f, 0.0f, 0.0f));	
      virtual ~CCameraSceneNode() {}
      
      virtual void setFOV(float fovy);
      virtual void setAspectRatio(float aspect);
      virtual void setNearValue(float zn);
      virtual void setFarValue(float zf);
      
      virtual float getFOV() { return fovy; }
      virtual float getAspectRatio() { return aspect; }
      virtual float getNearValue() { return zn; }
      virtual float getFarValue() { return zf; }
      virtual const matrix4& getProjectionMatrix() { return Projection; }
      
      virtual void setLookDirection(const CVector3& vect) { LookDirection = vect; }
      virtual void setTarget(const CVector3& target) { LookDirection = target - getPosition(); }
      virtual void setUpVector(const CVector3& vect) { UpVector = vect; }
      virtual void setRotation(const CVector3 &rot) {}
      virtual void setScale(const CVector3 &sc) {}
       
      virtual CVector3 getLookDirection() const { return LookDirection; }
      virtual CVector3 getUpVector() const { return UpVector; }
      virtual const matrix4& getViewMatrix();

      virtual void render();

	  virtual SViewFrustrum* getViewFrustrum();
	  virtual bool ProcessEvent(UINT umsg, WPARAM wparam, LPARAM lparam) {  return FALSE; }

protected:
      float fovy;
	  float aspect;
	  float zn;	
	  float zf;
      matrix4 Projection;

	  CVector3 LookDirection;
      CVector3 UpVector;
      matrix4 View;
	
      SViewFrustrum ViewArea;
      aabbox BBox;
      
      void recalculateProjection();
      void recalculateView();
	  void recalculateViewArea();
	  
	  virtual const aabbox& getBoundingBox() const { return BBox; }
	  virtual void OnPrepareRender();	  
      	
};

#endif
