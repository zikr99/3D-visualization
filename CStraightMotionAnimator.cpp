#include "CStraightMotionAnimator.h"
#include "CSceneManager.h"
#include "CSceneNode.h"

CStraightMotionAnimator::CStraightMotionAnimator(CSceneNode *scn,
      CVector3 targetvector, float length, float speed, bool makedead): 
      CAnimator(scn), m_length(length), m_speed(speed), m_makedead(makedead), 
      m_clength(0.0f)
{
      m_targetvector = targetvector;
      m_targetvector.Normalize();
}

void CStraightMotionAnimator::OnAnimateNode(unsigned int timeMs)
{     
      if (m_clength >= m_length) 
      {
            if (m_makedead) m_scenenode->setDead();        
            m_scenenode->removeAnimator(this);        
            return;
      }
      
      CVector3 pos = m_scenenode->getPosition();
      float lmov = m_speed*(float)timeMs/1000;
      
      if (m_clength + lmov > m_length)
      {
            lmov = m_length - m_clength;        
            m_clength = m_length;
      } 
      else
      {
            m_clength = m_clength + lmov;
      }
      
      CVector3 mov = m_targetvector*lmov;
      m_scenenode->setPosition(pos + mov);
}
