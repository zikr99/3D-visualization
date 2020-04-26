#ifndef __CPARTICLESYSTEMSCENENODE_H__
#define __CPARTICLESYSTEMSCENENODE_H__

#include "CSceneNode.h"
#include "SColor4f.h"

struct particle_t
{
      CVector3 m_pos;
      CVector3 m_prevPos;
      CVector3 m_velocity;
      CVector3 m_acceleration;

      float m_size;   
      float m_sizeDelta;
      
      float m_weight;  
      float m_weightDelta;
      
      SColor4f m_color; 
      SColor4f m_colorDelta; 
      
      float m_energy;
};

class CParticleSystemSceneNode: public CSceneNode
{
protected:
      particle_t *m_particlelist;
      int m_maxparticles;
      
      int m_numparticles;
      float m_accumulatedtime;       
          
      aabbox BBox;        
      virtual const aabbox& getBoundingBox() const { return BBox; }
      
      virtual void InitializeParticle(int idx) = 0;
              
public:
      CParticleSystemSceneNode(CSceneNode *parent, CSceneManager *mngr,
            int maxparticles, const CVector3& position = CVector3(0.0f, 0.0f, 0.0f),
            const CVector3& rotation = CVector3(0.0f, 0.0f, 0.0f),
		    const CVector3& scale = CVector3(1.0f, 1.0f, 1.0f)): 
            CSceneNode(parent, mngr, position, rotation, scale), m_particlelist(NULL),
            m_maxparticles(maxparticles) 
      {
            BBox.reset(0.0f, 0.0f, 0.0f);
      }
      
      virtual ~CParticleSystemSceneNode()
      {
            KillSystem();  
      }
      
      virtual void InitializeSystem();
      virtual void KillSystem();
      virtual int Emit(int numparticles);
      
      virtual void setRotation(const CVector3 &rot) {}
	  virtual void setScale(const CVector3 &sc) {}
       
};

#endif
