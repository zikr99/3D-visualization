#include "CSceneNode.h"

void CSceneNode::UpdateAbsoluteTransformation()
{
     if (parentNode)
	       m_absolutetransformation = 
		   ((CSceneNode*)parentNode)->getAbsoluteTransformation()*getRelativeTransformation();
     else
	       m_absolutetransformation = getRelativeTransformation();
}

matrix4 CSceneNode::getRelativeTransformation()
{
     matrix4 mat;
	 mat.setRotation(m_relativerotation);
	 mat.setTranslation(m_relativetranslation);

	 if (m_relativescale != CVector3(1.0f, 1.0f, 1.0f))
	 {
	       matrix4 smat;
	       smat.setScale(m_relativescale);
	       mat *= smat;
	 }

	 return mat; 
}

void CSceneNode::update(unsigned int timeMs)
{
     int pf;
     bool stop;
     list<CAnimator*>::Iterator it;
     list<CAnimator*>::Iterator itp;
     list<CAnimator*>::Iterator itn;
     CNode *cn, *pn, *nn;
	 
     OnUpdate(timeMs);
     
     if (!Animators.empty())
     {
           stop = false;
           it = Animators.begin();
                 
           do
           {
                  if (it == Animators.begin())  
                        pf = 1; 
                  else
                  {
                        itp = it;
                        --itp;
                               
                        pf = 0;
                  }
                        
                  (*it)->animateNode(timeMs);
                        
                  if (pf == 1)
                        itn = Animators.begin();
                  else  
                  {
                        itn = itp;
                        ++itn;
                  }                          
                        
                  if (itn == it)
                  {
                        ++it;
                        if (it == Animators.end()) stop = true;
                  }      
                  else
                  {
                        it = itn;
                        if (it == Animators.end()) stop = true; 
                  }
           } while (!stop);                 
     }
           
     UpdateAbsoluteTransformation();
           
     if (childNode)
     {
           stop = false;        
           cn = childNode;
           
           do
           {
                  if (cn->isFirstChild()) 
                  {
                        pn = cn->parentNode;
                        pf = 1;
                  }
                  else 
                  {
                        pn = cn->prevNode;
                        pf = 0;
                  }
                        
                  ((CSceneNode*)cn)->update(timeMs);
                        
                  if (pf == 1) nn = pn->childNode;
                  else nn = pn->nextNode;                            
                        
                  if (nn == cn)
                  {
                        if (cn->isLastChild()) stop = true;
                        else cn = cn->nextNode;
                  }      
                  else
                  {
                        cn = nn;
                              
                        if (cn == NULL) stop = true;
                        else if (cn->isFirstChild()) stop = true; 
                  }
           } while (!stop);      
     }       
}

void CSceneNode::preparerender() 
{
     CNode* cn;
     
     OnPrepareRender();
	 
	 if (childNode)
	 {
            cn = childNode->prevNode;
           
            do
            {
                  cn = cn->nextNode;
                  ((CSceneNode*)cn)->preparerender();
            } while (!cn->isLastChild());      
     }
}

void CSceneNode::render()
{
     if (!m_isvisible) return;
     
     GLfloat glmat[16];
     createGLMatrix(glmat, m_absolutetransformation);
     
	 glPushMatrix();
	       glMultMatrixf(glmat);
	       OnRender();
     glPopMatrix();          
}

aabbox CSceneNode::getTransformedBoundingBox()
{
     aabbox mbox;  
	 
     aabbox box = getBoundingBox();
	 box.getTransformed(m_absolutetransformation, mbox);
	 
     return mbox;
}

void CSceneNode::addAnimator(CAnimator *anm)
{
     if (anm) Animators.push_back(anm);
}

void CSceneNode::removeAnimator(CAnimator *anm)
{
     list<CAnimator*>::Iterator it = Animators.begin();
     for (; it != Animators.end(); ++it)
           if ((*it) == anm)
           {
				  delete (*it);
				  Animators.erase(it);
				  return;
		   }
}

void CSceneNode::clearAnimators()
{
     list<CAnimator*>::Iterator it = Animators.begin();
	 for (; it != Animators.end(); ++it)
	       delete (*it);

	 Animators.clear();
}
