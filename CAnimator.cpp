#include "CAnimator.h"
#include "CSceneNode.h"

CAnimator::CAnimator(CSceneNode *scn)
{
     if (scn) 
     {
           scn->addAnimator(this);
           m_scenenode = scn;
           m_scenemanager = scn->m_scenemanager;
           m_firstanimation = true;
     }
     else      
           delete this;
}

