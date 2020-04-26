#include "CBounceAnimator.h"
#include "CSceneManager.h"
#include "CEnemy.h"

void CBounceAnimator::animateNodeCollMovement(unsigned int timeMs, CVector3 eradius)
{     
      CVector3 newposition = m_scenenode->getPosition();
      
      CVector3 alastposition = m_scenenode->getAbsolutePosition();
      CVector3 anewposition = newposition;
      
      matrix4 pmat = ((CSceneNode*)m_scenenode->parentNode)->getAbsoluteTransformation();
      pmat.transformVector3(anewposition);
      
      CVector3 vt = m_vl + m_gravitation*timeMs;
      CVector3 gravmovement = (m_vl + vt)*(timeMs*0.5f*0.005f);
      m_vl = vt;
      
      m_tfall = m_tfall + timeMs;
      if ((m_tfall > 10000) && (!m_scenenode->isDead())) 
      {
            m_scenenode->setDead();
            CEnemy::DeadByFault++;
      }      
      
      CVector3 pmovement = anewposition - alastposition; 
      CVector3 amove;
      
      if (m_fpscamera)
      {
            if (m_hasstirrup)
            {
                  if (m_always) amove = pmovement + gravmovement; 
                  else amove = pmovement; 
            }
            else 
                  amove = gravmovement;
      } 
      else
            amove = pmovement + gravmovement;
                 
      float movelen = amove.Length();
      
      CCollisionManager *collmngr = m_scenemanager->getCollisionManager();
      bool newdirection = false;
      CVector3 collmove;
      CVector3 newmove;
      
      CVector3 collrelpoint, tv, uv;
      float cofrac, nds;
      
      int inum = 0;
      if (movelen > 0.0f) m_hasstirrup = FALSE;    
            
      while ((movelen > 0.0f) && collmngr->getCollisionMovement(alastposition - m_translation, amove, 
            eradius, m_bspselector, collmove, newmove, collrelpoint, tv, uv, cofrac, nds))
      {
            alastposition = alastposition + collmove;
            amove = newmove;
            movelen = amove.Length();
            
            newdirection = TRUE; 
            m_hasstirrup = TRUE;
            m_vl.set(0.0f, 0.0f, 0.0f);
            m_tfall = 0;
            
            inum++;
            if (inum > MAX_COLLISION_NUMBER)
            {
                  amove.set(0.0f, 0.0f, 0.0f);  
                  break;
            }
      }
                    
      alastposition = alastposition + amove;
                  
      matrix4 pmatinv;
      pmat.getInverse(pmatinv); 
      pmatinv.transformVector3(alastposition);
      
      m_scenenode->setPosition(alastposition);
      
      if (newdirection && (collrelpoint.y > -eradius.y/4))
      {
            CVector3 basedir;
      
            if (newmove == CVector3(0.0f, 0.0f, 0.0f))
                  basedir = -collmove;
            else
                  basedir = newmove;
                       
            ((CEnemy*)m_scenenode)->SetNewDirection(false, false, basedir);
      }      
}
