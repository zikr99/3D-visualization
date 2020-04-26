#ifndef __CENEMY_H__
#define __CENEMY_H__

#include <time.h>
#include "CMD2SceneNode.h"
#include "CBounceAnimator.h"
#include "CBullet.h"

class CEnemy: public CMD2SceneNode
{
protected:  
      bool m_disposable;
      
      float m_visdistance;
      float m_movementspeed;  
      CVector3 m_runningdirection;
      
      CMD2Mesh *m_weaponmesh;
      float m_bulletspeed;    
            
      int m_state;
      int m_lifestate;
      int m_deadstate;
      
      int m_runningstate;
      int m_idlestate;
      int m_shotstate; 
      
      int m_idletime;
      int m_shottime;
      bool m_hasfired;
      
      unsigned int m_runningtime;
      unsigned int m_runningperiod;
      unsigned int m_deflecttime;
      unsigned int m_chasedtime;
      
      int m_blinktime;
      int m_blinknum;
      
      virtual void OnUpdate(unsigned int timeMs); 
      virtual void SetNewDirection(bool randomdir = true, bool camerabased = true, 
            const CVector3 &basedir = CVector3(0.0f, 0.0f, 0.0f));
      virtual bool DeflectDirection();
      virtual void SetLookDirection();
      virtual void FireWeapon();
      
      friend class CBounceAnimator;
          
public:
      CEnemy(CSceneNode *parent, CSceneManager* mngr, CMD2Mesh *mesh,
            CBSPSceneNode *bspselector, CVector3 ellipseradius, 
            float visdistance, float movementspeed, CMD2Mesh *weaponmesh,
            float bulletspeed, CVector3 gravitation = CVector3(0.0f, -1.0f, 0.0f), 
            const CVector3& position = CVector3(0.0f, 0.0f, 0.0f),
            const CVector3& rotation = CVector3(0.0f, 0.0f, 0.0f),
		    const CVector3& scale = CVector3(1.0f, 1.0f, 1.0f)):
            CMD2SceneNode(parent, mngr, mesh, position, rotation, scale)      
      { 
            srand(time(NULL));
            
            Number++;
            m_disposable = false;
            
            m_visdistance = visdistance;
            m_movementspeed = movementspeed;
            SetNewDirection();
            
            m_weaponmesh = weaponmesh;
            m_bulletspeed = bulletspeed;
            
            m_state = 0;
            m_lifestate = 0;
            m_deadstate = 0;
            
            m_runningstate = 0;
            m_idlestate = 0;
            m_shotstate = 0;
            
            m_idletime = 0;
            m_shottime = 0;
            m_hasfired = false;
            setState(0, 39);
            
            m_chasedtime = 0;
            
            m_blinktime = 0;
            m_blinknum = 0;
            
            CBounceAnimator *animator = new CBounceAnimator(this, bspselector,
                 ellipseradius, gravitation);
      }
      
      virtual ~CEnemy() 
      {
            Number--;  
      }
      
      virtual void update(unsigned int timeMs)
      {
            if (m_disposable)
            {
                 delete this;
                 return;            
            }                      
              
            CMD2SceneNode::update(timeMs);
      }
      
      static int Number;
      static int DeadByBullet;
      static int DeadByFault;
       
};

#endif
