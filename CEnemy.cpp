#include "CEnemy.h"
#include "CSceneManager.h"

int CEnemy::Number = 0;
int CEnemy::DeadByBullet = 0;
int CEnemy::DeadByFault = 0;

void CEnemy::OnUpdate(unsigned int timeMs)
{
      CCameraSceneNode *acmr;
      CVector3 cmpos;
      
      if (isDead()) m_state = 1;
      
      if (m_state == 0)
      {
           if (m_lifestate == 0)
           {
                acmr = m_scenemanager->getActiveLevel()->getActiveCamera();
                cmpos = acmr->getAbsolutePosition() - getAbsolutePosition();  
                
                if (cmpos.DotProduct(cmpos) < m_visdistance*m_visdistance)
                {
                     if ((rand() % 100) > 100/4)
                     {                       
                          m_lifestate = 2;
                          m_runningstate = 0;
                          SetNewDirection(false);
                     }
                     else
                     {
                          m_lifestate = 3;
                          m_shotstate = 0;
                     }
                }
                else if (cmpos.DotProduct(cmpos) < 16*m_visdistance*m_visdistance)
                {
                     if ((rand() % 100) < 100/6)
                     {
                          m_lifestate = 3;
                          m_shotstate = 0;
                     }                       
                     else if ((rand() % 100) > 100/4)
                     {
                          m_lifestate = 1;
                          m_idlestate = 0;
                     }
                     else
                     {   
                          m_lifestate = 2;
                          m_runningstate = 0;
                          SetNewDirection();
                     }
                }
                else
                {
                     if ((rand() % 100) > 100/4)
                     {
                          m_lifestate = 1;
                          m_idlestate = 0;
                     }
                     else
                     {   
                          m_lifestate = 2;
                          m_runningstate = 0;
                          SetNewDirection();
                     }     
                }        
           }
           else if (m_lifestate == 1)
           {
                acmr = m_scenemanager->getActiveLevel()->getActiveCamera();
                cmpos = acmr->getAbsolutePosition() - getAbsolutePosition();  
                
                if (cmpos.DotProduct(cmpos) < m_visdistance*m_visdistance)
                {
                     if ((rand() % 100) > 100/4)                       
                     {
                          m_lifestate = 2;
                          m_runningstate = 0;
                          SetNewDirection(false);
                     }
                     else
                     {
                          m_lifestate = 3;
                          m_shotstate = 0;
                     }
                }     
                
                if (m_idlestate == 0)
                {
                     setState(0, 39);           
                     m_idlestate = 1; 
                     m_idletime = 0;          
                }
                else if (m_idlestate == 1)
                {
                     if (m_currentframe >= 38)
                          m_idletime++;
                     
                     if ((m_nextframe >= 1) && (m_idletime > 0))
                          m_lifestate = 0; 
                }
           }
           else if (m_lifestate == 2)
           {
                if (m_runningstate == 0)
                {
                     setState(40, 46);
                     m_runningstate = 1;
                     m_runningtime = 0;
                     m_runningperiod = 1000 + (10000*((rand() % 100) - 50))/50;
                     m_deflecttime = 0;       
                }
                else if (m_runningstate == 1)
                {
                     if (m_runningtime > m_runningperiod)
                     {
                          m_lifestate = 0;             
                     }
                     else
                     {
                          m_runningtime = m_runningtime + timeMs;
                          m_deflecttime = m_deflecttime + timeMs;
                          m_chasedtime = m_chasedtime + timeMs;
                     
                          acmr = m_scenemanager->getActiveLevel()->getActiveCamera();
                          cmpos = acmr->getAbsolutePosition() - getAbsolutePosition();  
                
                          if ((cmpos.DotProduct(cmpos) < m_visdistance*m_visdistance) &&
                                (m_deflecttime > 500))
                                if (DeflectDirection())
                                      m_deflecttime = 0;
                          
                          if ((cmpos.DotProduct(cmpos) < m_visdistance*m_visdistance) &&
                                (m_chasedtime > 15000))
                          {
                                m_chasedtime = 0;
                                m_lifestate = 3;
                                m_shotstate = 0;
                          }            
                      
                          CVector3 pos = getPosition();
                          pos = pos + m_runningdirection*(m_movementspeed*timeMs/1000.0f);
                          setPosition(pos);
                     }     
                }           
           }
           else if (m_lifestate == 3)
           {
                if (m_shotstate == 0)
                {
                     setState(96, 112);
                     m_shotstate = 1;
                     m_shottime = 0;
                     m_hasfired = false;
                     SetLookDirection();
                }
                else if (m_shotstate == 1)
                {
                     if ((m_currentframe >= 103) && (!m_hasfired))
                     {
                          m_hasfired = true;              
                          FireWeapon();
                     }
                     
                     if (m_currentframe >= 111)
                          m_shottime++;
                     
                     if ((m_nextframe >= 97) && (m_shottime > 0))
                          m_lifestate = 0;
                }
           }
      }
      else if (m_state == 1)
      {
           if (m_deadstate == 0)
           {
                setState(178, 184);
                m_deadstate = 1;
           }
           else if (m_deadstate == 1)
           {
                if (m_currentframe >= 183)
                {
                     setFrame(183);
                     m_deadstate = 2;
                }
           }
           else if (m_deadstate == 2)
           {
                if (m_blinknum >= 2)
                {
                     m_disposable = true;
                }
                else
                {     
                     m_blinktime++;
                                
                     if (m_blinktime > 80)
                     {
                          m_blinktime = m_blinktime - 80;
                          m_blinknum++;
                     }
                     else if (m_blinktime > 60)
                          setVisible(false);
                     else 
                          setVisible(true);
                }
           }
      }
            
      CMD2SceneNode::OnUpdate(timeMs);  
}

void CEnemy::SetNewDirection(bool randomdir, bool camerabased, const CVector3 &basedir)
{
      CVector3 bdir;
      CVector3 sdir;
      
      if (randomdir)
      {
           m_runningdirection.x = (rand() % 100) - 50.0f;
           m_runningdirection.y = 0.0f;
           m_runningdirection.z = (rand() % 100) - 50.0f;
           m_runningdirection.Normalize();
      }
      else
      {
           if (camerabased)
           {
                CCameraSceneNode *acmr = m_scenemanager->getActiveLevel()->getActiveCamera();
                CVector3 cmpos = acmr->getAbsolutePosition();
           
                matrix4 pmat = ((CSceneNode*)parentNode)->getAbsoluteTransformation();
                matrix4 pmatinv;
                pmat.getInverse(pmatinv);
                pmatinv.transformVector3(cmpos);
           
                bdir = getPosition() - cmpos;
           }  
           else
           {
                bdir = basedir;
           }     
           
           bdir.y = 0.0f;
           sdir.y = 0.0f;
           sdir.x = -bdir.z;
           sdir.z = bdir.x;
           sdir = sdir*(float((rand() % 100) - 50)/float(50));
           
           m_runningdirection = bdir + sdir;
           m_runningdirection.y = 0.0f;
           m_runningdirection.Normalize();
      }
      
      CVector3 crot = getRotation();
      crot.z = atan2(-m_runningdirection.z, m_runningdirection.x);
      setRotation(crot);             
}

bool CEnemy::DeflectDirection()
{
      CCameraSceneNode *acmr = m_scenemanager->getActiveLevel()->getActiveCamera();
      CVector3 cmpos = acmr->getAbsolutePosition();
                                                 
      matrix4 pmat = ((CSceneNode*)parentNode)->getAbsoluteTransformation();
      matrix4 pmatinv;
      pmat.getInverse(pmatinv);
                           
      pmatinv.transformVector3(cmpos);
      cmpos = cmpos - getPosition();
                           
      float pl = cmpos.DotProduct(m_runningdirection);
                           
      if (pl > 0.0f)
      {
           CVector3 deflect = m_runningdirection*pl - cmpos;
           deflect.Normalize();
           deflect = deflect*1.0f/3.0f;
      
           m_runningdirection = m_runningdirection + deflect;
           m_runningdirection.y = 0.0f;
           m_runningdirection.Normalize(); 
      
           CVector3 crot = getRotation();
           crot.z = atan2(-m_runningdirection.z, m_runningdirection.x);
           setRotation(crot);
           
           return true;  
      }           
      
      return false;
}

void CEnemy::SetLookDirection()
{
      CCameraSceneNode *acmr = m_scenemanager->getActiveLevel()->getActiveCamera();
      CVector3 cmpos = acmr->getAbsolutePosition();
                                                 
      matrix4 pmat = ((CSceneNode*)parentNode)->getAbsoluteTransformation();
      matrix4 pmatinv;
      pmat.getInverse(pmatinv);
                           
      pmatinv.transformVector3(cmpos);
      cmpos = cmpos - getPosition();
                                 
      CVector3 crot = getRotation();
      crot.z = atan2(-cmpos.z, cmpos.x);
      setRotation(crot);
}

void CEnemy::FireWeapon()
{
      CCameraSceneNode *acmr = m_scenemanager->getActiveLevel()->getActiveCamera();
      CVector3 cmpos = acmr->getAbsolutePosition();
                                                 
      matrix4 pmat = ((CSceneNode*)parentNode)->getAbsoluteTransformation();
      matrix4 pmatinv;
      pmat.getInverse(pmatinv);
                           
      pmatinv.transformVector3(cmpos);
      cmpos = cmpos - getPosition();
      
      CSceneNode *rootn = m_scenemanager->getActiveLevel()->getRootSceneNode();
                                 
      CBullet *bullet = new CBullet((CSceneNode*)parentNode, m_scenemanager,
           this, m_weaponmesh, getPosition(), cmpos, 800.0f, m_bulletspeed, 
           CVector3(15.0f, 15.0f, 15.0f), rootn, CVector3(0.4f, 0.4f, 0.4f));
}
