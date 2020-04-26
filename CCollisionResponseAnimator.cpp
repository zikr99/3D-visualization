#include "CCollisionResponseAnimator.h"
#include "CSceneNode.h"
#include "CSceneManager.h"

CCollisionResponseAnimator::CCollisionResponseAnimator(CSceneNode *scn, CBSPSceneNode *bspselector):
      CAnimator(scn), m_bspselector(bspselector), m_mode(ECRM_POINT)
{
}

CCollisionResponseAnimator::CCollisionResponseAnimator(CSceneNode *scn, CBSPSceneNode *bspselector, 
      float radius): CAnimator(scn), m_bspselector(bspselector), m_mode(ECRM_SPHERE), m_radius(radius)
{
}

CCollisionResponseAnimator::CCollisionResponseAnimator(CSceneNode *scn, CBSPSceneNode *bspselector, 
      CVector3 ellipseradius, CVector3 gravitation, CVector3 translation, bool fpscamera,
      bool alwaysapplygravity): CAnimator(scn), m_bspselector(bspselector), m_mode(ECRM_ELLIPSE)
{
      m_ellipseradius = ellipseradius;
      
      m_gravitation = gravitation;
      m_translation = translation;
      
      m_fpscamera = fpscamera;
      m_always = alwaysapplygravity;
      
      m_vl.set(0.0f, 0.0f, 0.0f);
      m_hasstirrup = FALSE;
}

void CCollisionResponseAnimator::OnAnimateNode(unsigned int timeMs)
{
      if (m_mode == ECRM_POINT)
            animateNodeCollPoint(timeMs);
      else if (m_mode == ECRM_SPHERE)
            animateNodeCollMovement(timeMs, m_radius);
      else if (m_mode == ECRM_ELLIPSE)
            animateNodeCollMovement(timeMs, m_ellipseradius);
}

void CCollisionResponseAnimator::animateNodeCollPoint(unsigned int timeMs)
{
      CVector3 newposition = m_scenenode->getPosition();
      
      CVector3 alastposition = m_scenenode->getAbsolutePosition();
      CVector3 anewposition = newposition; 
      
      matrix4 pmat = ((CSceneNode*)m_scenenode->parentNode)->getAbsoluteTransformation();
      pmat.transformVector3(anewposition);
      
      if (anewposition != alastposition)
      {          
            CVector3 collpoint;
            CCollisionManager *collmngr = m_scenemanager->getCollisionManager();
            
            if (collmngr->getCollisionPoint(alastposition, anewposition, m_bspselector, collpoint))
            {     
                  matrix4 pmatinv;
                  pmat.getInverse(pmatinv); 
                  pmatinv.transformVector3(collpoint);
      
                  m_scenenode->setPosition(collpoint);
            }
      }
}

void CCollisionResponseAnimator::animateNodeCollMovement(unsigned int timeMs, float radius)
{
      CVector3 newposition = m_scenenode->getPosition();
      
      CVector3 alastposition = m_scenenode->getAbsolutePosition();
      CVector3 anewposition = newposition;
      
      matrix4 pmat = ((CSceneNode*)m_scenenode->parentNode)->getAbsoluteTransformation();
      pmat.transformVector3(anewposition);
      
      if (anewposition != alastposition)
      {          
            CVector3 amove = anewposition - alastposition;
      
            CVector3 collmove;
            CCollisionManager *collmngr = m_scenemanager->getCollisionManager();
            
            if (collmngr->getCollisionMovement(alastposition, amove, radius, m_bspselector, collmove))
            {     
                  CVector3 anposition = alastposition + collmove;
                  
                  matrix4 pmatinv;
                  pmat.getInverse(pmatinv); 
                  pmatinv.transformVector3(anposition);
      
                  m_scenenode->setPosition(anposition);
            }
      }
}

void CCollisionResponseAnimator::animateNodeCollMovement(unsigned int timeMs, CVector3 eradius)
{     
      CVector3 newposition = m_scenenode->getPosition();
      
      CVector3 alastposition = m_scenenode->getAbsolutePosition();
      CVector3 anewposition = newposition;
      
      matrix4 pmat = ((CSceneNode*)m_scenenode->parentNode)->getAbsoluteTransformation();
      pmat.transformVector3(anewposition);
      
      CVector3 vt = m_vl + m_gravitation*timeMs;
      CVector3 gravmovement = (m_vl + vt)*(timeMs*0.5f*0.005f);
      m_vl = vt;
      
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
      CVector3 collmove;
      CVector3 newmove;
      
      CVector3 odmove, sv, tv, uv;
      float cofrac, nds;
      
      int inum = 0;
      if (movelen > 0.0f) m_hasstirrup = FALSE;    
            
      while ((movelen > 0.0f) && collmngr->getCollisionMovement(alastposition - m_translation, amove, 
            eradius, m_bspselector, collmove, newmove, sv, tv, uv, cofrac, nds))
      {
            alastposition = alastposition + collmove;
            amove = newmove;
            movelen = amove.Length(); 
            
            m_hasstirrup = TRUE;
            m_vl.set(0.0f, 0.0f, 0.0f);
            
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
}
