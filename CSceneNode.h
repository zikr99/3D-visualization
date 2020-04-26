#ifndef __CSCENENODE_H__
#define __CSCENENODE_H__

#include <gl\glu.h>
#include "irrList.h"
#include "CNode.h"
#include "matrix4.h"
#include "CAnimator.h"

class CSceneManager;

class CSceneNode: public CNode
{
protected:      
     CSceneManager *m_scenemanager;   
     bool m_isvisible;
     bool m_dead;
           
	 CVector3 m_relativetranslation;
	 CVector3 m_relativerotation;
     CVector3 m_relativescale;
     matrix4 m_absolutetransformation;
     
     list<CAnimator*> Animators;   
     
     void UpdateAbsoluteTransformation();
     virtual const aabbox& getBoundingBox() const = 0;
     
     virtual void OnUpdate(unsigned int timeMs) {}
     virtual void OnPrepareRender() {}
     virtual void OnRender() {}
     
     void createGLMatrix(GLfloat gl_matrix[16], matrix4& m)
	 {
		   int i = 0;
		   for (int r = 0; r < 4; ++r)
				for (int c = 0; c < 4; ++c)
					gl_matrix[i++] = m(c, r);
     }
     
     void addAnimator(CAnimator *anm);
     friend class CAnimator;
     
public:
     CSceneNode(CSceneNode* parent, CSceneManager* mngr, 
          const CVector3& position = CVector3(0.0f, 0.0f, 0.0f),
          const CVector3& rotation = CVector3(0.0f, 0.0f, 0.0f),
		  const CVector3& scale = CVector3(1.0f, 1.0f, 1.0f)): CNode(parent), 
          m_scenemanager(mngr), m_isvisible(true), m_dead(false),
          m_relativetranslation(position), m_relativerotation(rotation),
          m_relativescale(scale)
     {
            UpdateAbsoluteTransformation();			
     }
     	
     virtual ~CSceneNode() { clearAnimators(); }
    
     matrix4 getRelativeTransformation();
     const matrix4& getAbsoluteTransformation() { return m_absolutetransformation; }
    
     virtual void update(unsigned int timeMs); 
	 virtual void preparerender();
	 virtual void render();
	 
	 virtual void setVisible(bool vsb) { m_isvisible = vsb; }
	 virtual bool isVisible() { return m_isvisible; }
	 virtual void setDead(bool dead = true) { m_dead = dead; }
	 virtual bool isDead() { return m_dead; }
	 virtual void setPosition(const CVector3 &pos) { m_relativetranslation = pos; }
	 virtual CVector3 getPosition() const { return m_relativetranslation; }
	 virtual void setRotation(const CVector3 &rot) { m_relativerotation = rot; }
	 virtual CVector3 getRotation() const { return m_relativerotation; }
	 virtual void setScale(const CVector3 &sc) { m_relativescale = sc; }
	 virtual CVector3 getScale() const { return m_relativescale; }
	 virtual CVector3 getAbsolutePosition() const { return m_absolutetransformation.getTranslation(); }
	 aabbox getTransformedBoundingBox();
            
     void removeAnimator(CAnimator *anm);
	 void clearAnimators();
            
};

#endif
