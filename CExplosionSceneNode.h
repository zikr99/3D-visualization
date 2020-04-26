#ifndef __CEXPLOSIONSCENENODE_H__
#define __CEXPLOSIONSCENENODE_H__

#include "CParticleSystemSceneNode.h"
#include "SMaterial.h"
#include <time.h>

#define FRAND (((float)rand() - (float)rand())/RAND_MAX)

const CVector3 PARTICLE_VELOCITY(0.0f, 2.0f, 0.0f);
const CVector3 VELOCITY_VARIATION(4.0f, 4.0f, 4.0f);
const CVector3 PARTICLE_ACCELERATION(0.0f, -5.0f, 0.0f);
const float PARTICLE_SIZE = 8.0f;
const float SIZE_VARIATION = 4.0f;

class CExplosionSceneNode: public CParticleSystemSceneNode
{
protected:
      float m_spread; 
      SMaterial m_material;   
      
      virtual void InitializeSystem() { CParticleSystemSceneNode::InitializeSystem(); }
      virtual void KillSystem() { CParticleSystemSceneNode::KillSystem(); }
      virtual int Emit(int numparticles) { CParticleSystemSceneNode::Emit(numparticles); }
      virtual void InitializeParticle(int idx);
      
      virtual void OnUpdate(unsigned int timeMs);
      virtual void OnPrepareRender();
      virtual void OnRender();
              
public:
      CExplosionSceneNode(CSceneNode *parent, CSceneManager *mngr,
            int maxparticles, float spread, CImage *img = NULL,
            const CVector3& position = CVector3(0.0f, 0.0f, 0.0f),
            const CVector3& rotation = CVector3(0.0f, 0.0f, 0.0f),
		    const CVector3& scale = CVector3(1.0f, 1.0f, 1.0f)): 
            CParticleSystemSceneNode(parent, mngr, maxparticles, position, rotation, scale),
            m_spread(spread) 
      { 
            m_material.MaterialType = EMT_COLOR_TEXTURE;
            m_material.Texture1 = new CTexture();
            m_material.Texture1->setParameters(GL_NEAREST_MIPMAP_LINEAR, 
                  GL_NEAREST_MIPMAP_LINEAR, GL_CLAMP, GL_CLAMP);     
            setImage(img);
            
            srand(time(NULL));
            
            InitializeSystem();
            Emit(maxparticles);
      }
      
      virtual ~CExplosionSceneNode()
      {
            delete m_material.Texture1;  
      }
      
      virtual void setImage(CImage *img);
      virtual void update(unsigned int timeMs);
       
};

#endif
