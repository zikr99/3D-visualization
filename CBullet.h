#ifndef __CBULLET_H__
#define __CBULLET_H__

#include "CMD2SceneNode.h"
#include "CStraightMotionAnimator.h"
#include "CBulletResponseAnimator.h"

class CBullet: public CMD2SceneNode
{              
protected:
      CSceneNode *m_owner;
          
public:
      CBullet(CSceneNode *parent, CSceneManager* mngr, CSceneNode *owner, 
            CMD2Mesh *mesh, const CVector3 position, const CVector3 direction, 
            float distance, float speed, const CVector3 eradius, CSceneNode *wroot,
	   	    const CVector3& scale = CVector3(1.0f, 1.0f, 1.0f)):
            CMD2SceneNode(parent, mngr, mesh, position, CVector3(0.0f, 0.0f, 0.0f), 
            scale)
      { 
            m_owner = owner;
            setState(0, getNumFrames());
            
            if (direction != CVector3(0.0f, 0.0f, 0.0f))
            {
                  float roty = 0.0f;
                  float rotz = 0.0f;
                  float rxy, rx, dx;
                  
                  rxy = sqrt(direction.x*direction.x + direction.y*direction.y);
                  if (direction.x >= 0) rx = rxy;
                  else rx = -rxy;
                          
                  if ((direction.z != 0.0f) || (rx != 0.0f))        
                        roty = atan2(-direction.z, rx);
                  
                  if (direction.x >= 0) dx = direction.x;
                  else dx = -direction.x;      
                        
                  if ((direction.y != 0.0f) || (dx != 0.0f))
                        rotz = atan2(direction.y, dx);
            
                  if (direction.x < 0) rotz = -rotz;      
            
                  setRotation(CVector3(rotz, 0.0f, roty));
            }
            
            CAnimator *motion = new CStraightMotionAnimator(this, direction, distance, speed, true);
            CAnimator *response = new CBulletResponseAnimator(this, owner, wroot, eradius);
      }
      
      virtual ~CBullet() {}
      
      virtual void update(unsigned int timeMs)
      {
            if (m_isvisible)
                  if (isDead())
                  {
                        delete this;
                        return;
                  }  
            
            CMD2SceneNode::update(timeMs);
      }
      
      virtual CSceneNode* getOwner() { return m_owner; }
      
};

#endif
