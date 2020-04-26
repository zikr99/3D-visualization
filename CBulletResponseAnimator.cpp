#include "CBulletResponseAnimator.h"
#include "CSceneManager.h"
#include "CExplosionSceneNode.h"
#include "CEnemy.h"

CBulletResponseAnimator::CBulletResponseAnimator(CSceneNode *scn, CSceneNode *owner,
      CSceneNode *wroot, CVector3 eradius): CAnimator(scn), m_owner(owner), 
      m_root(wroot), m_hitnode(NULL)
{
      CBmp24Image *bmp = new CBmp24Image("textures\\explosion.bmp");
      m_explosion = new CExplosionSceneNode(NULL, m_scenemanager, 400, 30.0f, bmp);
      delete bmp;
      
      m_radius = eradius;
}

CBulletResponseAnimator::~CBulletResponseAnimator()
{
      if (m_explosion) delete m_explosion;                                             
}

void CBulletResponseAnimator::OnAnimateNode(unsigned int timeMs)
{     
      CVector3 newposition = m_scenenode->getPosition();
      
      m_alastposition = m_scenenode->getAbsolutePosition();
      CVector3 anewposition = newposition;
      
      matrix4 pmat = ((CSceneNode*)m_scenenode->parentNode)->getAbsoluteTransformation();
      pmat.transformVector3(anewposition);
      
      if (anewposition != m_alastposition)
      {       
            m_amove = anewposition - m_alastposition;
            
            m_box = m_scenenode->getTransformedBoundingBox();
            m_bx0 = m_alastposition.x - m_box.MinEdge.x;
            m_bx1 = m_box.MaxEdge.x - m_alastposition.x;
            m_by0 = m_alastposition.y - m_box.MinEdge.y;
            m_by1 = m_box.MaxEdge.y - m_alastposition.y;;
            m_bz0 = m_alastposition.z - m_box.MinEdge.z;
            m_bz1 = m_box.MaxEdge.z - m_alastposition.z;;
                            
            m_outputfraction = 2.0f;
      
            CheckNodes(m_root);    
            
            if (m_outputfraction < 2.0f) 
            {                                 
                  float mlength = m_amove.Length();               
                  float ofraction = m_outputfraction - EPSILON/mlength;
                  if (ofraction < 0.0f) ofraction = 0.0f;      
                                 
                  CVector3 updposition = m_alastposition + m_amove*ofraction;
                  
                  matrix4 pmatinv;
                  pmat.getInverse(pmatinv);
                  pmatinv.transformVector3(updposition);               
                  
                  m_scenenode->setPosition(updposition);
                  
                  if (!m_scenenode->isDead()) 
                  {     
                        m_hitnode->setDead();
                        m_scenenode->setDead();
                        
                        if (dynamic_cast<CEnemy*>(m_hitnode)) CEnemy::DeadByBullet++; 
                  }
                  
                  if (m_explosion)
                  {
                        m_explosion->setPosition(updposition);
                        m_scenenode->parentNode->attach(m_explosion);
                        m_explosion = NULL;
                  }
            }
      } 
}

void CBulletResponseAnimator::CheckNodes(CSceneNode *rnode)
{
      if (rnode == m_scenenode) {}
      else if (rnode == m_owner) {}
      else if (dynamic_cast<CRootSceneNode*>(rnode)) {}
      else if (dynamic_cast<CBSPSceneNode*>(rnode))
      {
            CheckBSP((CBSPSceneNode*)rnode);                                 
      }
      else
      {
            CheckBB(rnode);
      }
 
      CNode *cn;
      
      if (rnode->childNode)
	  {
            cn = rnode->childNode->prevNode;
           
            do
            {
                  cn = cn->nextNode;
                  CheckNodes((CSceneNode*)cn);
            } while (!cn->isLastChild());      
      }
}

void CBulletResponseAnimator::CheckBB(CSceneNode *rnode)
{    
      aabbox abox = rnode->getTransformedBoundingBox();
      CVector3 apos = rnode->getAbsolutePosition();
 
      if (abox.MinEdge == abox.MaxEdge) return;
      
      if (m_box.intersectsWithBox(abox))
      {
            if (m_outputfraction > 0.0f) 
            {
                  m_outputfraction = 0.0f;
                  m_hitnode = rnode;
            }
            
            return;     
      }
      
      float tmin = 2.0f;
      
      float t;
      float t1 = (abox.MinEdge.x - m_bx1 - m_alastposition.x)/m_amove.x;
      float t2 = (abox.MaxEdge.x + m_bx0 - m_alastposition.x)/m_amove.x;
      
      float maxa, maxb;
      float mina, minb;
      float tmaxa, tmaxb;
      float tmina, tminb;
      
      if ((t1 >= 0) && (t2 >= 0))
      {
            t = (t1 < t2)?t1:t2;
            
            if (t <= 1)
            {
                  maxa = m_alastposition.y + m_amove.y*t + m_by1;
                  maxb = m_alastposition.z + m_amove.z*t + m_bz1;
                  mina = m_alastposition.y + m_amove.y*t - m_by0;
                  minb = m_alastposition.z + m_amove.z*t - m_bz0;
                  
                  tmaxa = abox.MaxEdge.y; 
                  tmaxb = abox.MaxEdge.z;
                  tmina = abox.MinEdge.y; 
                  tminb = abox.MinEdge.z;
                  
                  if (IsRectIntersect(maxa, maxb, mina, minb, tmaxa, tmaxb,
                        tmina, tminb))
                        if (t < tmin)
                              tmin = t;
            }
      }
      
      t1 = (abox.MinEdge.y - m_by1 - m_alastposition.y)/m_amove.y;
      t2 = (abox.MaxEdge.y + m_by0 - m_alastposition.y)/m_amove.y;
            
      if ((t1 >= 0) && (t2 >= 0))
      {
            t = (t1 < t2)?t1:t2;
            
            if (t <= 1)
            {
                  maxa = m_alastposition.x + m_amove.x*t + m_bx1;
                  maxb = m_alastposition.z + m_amove.z*t + m_bz1;
                  mina = m_alastposition.x + m_amove.x*t - m_bx0;
                  minb = m_alastposition.z + m_amove.z*t - m_bz0;
                  
                  tmaxa = abox.MaxEdge.x; 
                  tmaxb = abox.MaxEdge.z;
                  tmina = abox.MinEdge.x; 
                  tminb = abox.MinEdge.z;
                  
                  if (IsRectIntersect(maxa, maxb, mina, minb, tmaxa, tmaxb,
                        tmina, tminb))
                        if (t < tmin)
                              tmin = t;
            }
      }
      
      t1 = (abox.MinEdge.z - m_bz1 - m_alastposition.z)/m_amove.z;
      t2 = (abox.MaxEdge.z + m_bz0 - m_alastposition.z)/m_amove.z;
            
      if ((t1 >= 0) && (t2 >= 0))
      {
            t = (t1 < t2)?t1:t2;
            
            if (t <= 1)
            {
                  maxa = m_alastposition.x + m_amove.x*t + m_bx1;
                  maxb = m_alastposition.y + m_amove.y*t + m_by1;
                  mina = m_alastposition.x + m_amove.x*t - m_bx0;
                  minb = m_alastposition.y + m_amove.y*t - m_by0;
                  
                  tmaxa = abox.MaxEdge.x; 
                  tmaxb = abox.MaxEdge.y;
                  tmina = abox.MinEdge.x; 
                  tminb = abox.MinEdge.y;
                  
                  if (IsRectIntersect(maxa, maxb, mina, minb, tmaxa, tmaxb,
                        tmina, tminb))
                        if (t < tmin)
                              tmin = t;
            }
      }
      
      if (tmin < m_outputfraction)
      {
            m_outputfraction = tmin;
            m_hitnode = rnode;
      }
}

void CBulletResponseAnimator::CheckBSP(CBSPSceneNode *rnode)
{
      CCollisionManager *collmgr = m_scenemanager->getCollisionManager();
      
      CVector3 collmove;
      CVector3 newmove;
      CVector3 sv, tv, uv;
      float cofrac, nds;
      
      if (collmgr->getCollisionMovement(m_alastposition, m_amove, m_radius, 
            rnode, collmove, newmove, sv, tv, uv, cofrac, nds))
      {
            if (cofrac < m_outputfraction)
            {
                   m_outputfraction = cofrac;
                   m_hitnode = rnode;
            }
      }       
}

bool CBulletResponseAnimator::IsRectIntersect(float r1maxa, float r1maxb, float r1mina, 
      float r1minb, float r2maxa, float r2maxb, float r2mina, float r2minb)
{
      return (r1maxa >= r2mina) && (r1maxb >= r2minb) && (r2maxa >= r1mina) && 
           (r2maxb >= r1minb);     
}
