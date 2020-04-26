#include "CParticleSystemSceneNode.h"

void CParticleSystemSceneNode::InitializeSystem()
{
      if (m_particlelist)
      {
             delete [] m_particlelist;
             m_particlelist = NULL;
      }

      m_particlelist = new particle_t [m_maxparticles];
      
      m_numparticles = 0;
      m_accumulatedtime = 0.0f;
}

void CParticleSystemSceneNode::KillSystem()
{
      if (m_particlelist)
      {
             delete [] m_particlelist;
             m_particlelist = NULL;
      }
}

int CParticleSystemSceneNode::Emit(int numparticles)
{
      while (numparticles && (m_numparticles < m_maxparticles))
      {
             InitializeParticle(m_numparticles++);
             --numparticles;
      }

      return numparticles;
}
