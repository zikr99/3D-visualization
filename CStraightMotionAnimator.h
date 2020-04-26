#ifndef __CSTRAIGHTMOTIONANIMATOR_H__
#define __CSTRAIGHTMOTIONANIMATOR_H__

#include "CAnimator.h"
#include "CVector3.h"

class CSceneNode;

class CStraightMotionAnimator: public CAnimator
{
protected:    
      CVector3 m_targetvector;
      float m_length;
      float m_speed;
      bool m_makedead;
      
      float m_clength;
      
      virtual void OnAnimateNode(unsigned int timeMs);
          
public:
      CStraightMotionAnimator(CSceneNode *scn, CVector3 targetvector,
            float length, float speed = 20.0f, bool makedead = false);
      virtual ~CStraightMotionAnimator() {}
       
};

#endif
