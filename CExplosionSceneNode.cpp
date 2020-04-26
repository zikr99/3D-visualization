#include "CExplosionSceneNode.h"
#include "CSceneManager.h"

void CExplosionSceneNode::InitializeParticle(int idx)
{
      m_particlelist[idx].m_pos.x = getPosition().x + FRAND*m_spread;
      m_particlelist[idx].m_pos.y = getPosition().y + FRAND*m_spread;
      m_particlelist[idx].m_pos.z = getPosition().z + FRAND*m_spread;

      m_particlelist[idx].m_velocity.x = PARTICLE_VELOCITY.x + FRAND*VELOCITY_VARIATION.x;
      m_particlelist[idx].m_velocity.y = PARTICLE_VELOCITY.y + FRAND*VELOCITY_VARIATION.y;
      m_particlelist[idx].m_velocity.z = PARTICLE_VELOCITY.z + FRAND*VELOCITY_VARIATION.z;
      m_particlelist[idx].m_acceleration = PARTICLE_ACCELERATION;

      m_particlelist[idx].m_energy = 1.5f + FRAND/2.0f;

      m_particlelist[idx].m_size = PARTICLE_SIZE + FRAND*SIZE_VARIATION;
      m_particlelist[idx].m_sizeDelta = -m_particlelist[idx].m_size/m_particlelist[idx].m_energy;

      m_particlelist[idx].m_color.r = 1.0f;
      m_particlelist[idx].m_color.g = 0.5f + FRAND*0.5f;
      m_particlelist[idx].m_color.b = 0.01f;
      m_particlelist[idx].m_color.a = 1.0f;

      m_particlelist[idx].m_colorDelta.r = 0.0f;
      m_particlelist[idx].m_colorDelta.g = -(m_particlelist[idx].m_color.g/2.0f)/m_particlelist[idx].m_energy;
      m_particlelist[idx].m_colorDelta.b = 0.0f;
      m_particlelist[idx].m_colorDelta.a = -1.0f/m_particlelist[idx].m_energy; 
}

void CExplosionSceneNode::update(unsigned int timeMs)
{    
     if (m_numparticles == 0)
           delete this;
     else
           CSceneNode::update(timeMs);      
}
     
void CExplosionSceneNode::OnUpdate(unsigned int timeMs)
{     
      float elpsTime = (float)timeMs/1000;
      int i = 0;
     
      while (i < m_numparticles)
      {
            m_particlelist[i].m_pos = m_particlelist[i].m_pos + m_particlelist[i].m_velocity*elpsTime;
            m_particlelist[i].m_velocity = m_particlelist[i].m_velocity + 
                   m_particlelist[i].m_acceleration*elpsTime;

            m_particlelist[i].m_size += m_particlelist[i].m_sizeDelta*elpsTime;

            m_particlelist[i].m_color.a += m_particlelist[i].m_colorDelta.a*elpsTime;
            m_particlelist[i].m_color.g += m_particlelist[i].m_colorDelta.g*elpsTime;
            
            m_particlelist[i].m_energy -= elpsTime;

            if (m_particlelist[i].m_energy <= 0.0f)
            { 
                  if (m_numparticles > i + 1)
                        m_particlelist[i] = m_particlelist[m_numparticles - 1];
                  
                  m_numparticles--;       
            }
            else
                  i++;
      }
}

void CExplosionSceneNode::OnPrepareRender()
{
      m_scenemanager->registerNodeForRendering(this, ESNRP_TRANSPARENT);
}

void CExplosionSceneNode::OnRender()
{     
      CCameraSceneNode *acmr = m_scenemanager->getActiveLevel()->getActiveCamera();
      matrix4 pmat = ((CSceneNode*)(acmr->parentNode))->getAbsoluteTransformation();
       
      CVector3 xvec = acmr->getUpVector().CrossProduct(acmr->getLookDirection());
      CVector3 yvec = acmr->getLookDirection().CrossProduct(xvec);
       
      CVector3 campos = acmr->getAbsolutePosition();
      CVector3 camhor = acmr->getPosition() + xvec;
      CVector3 camup = acmr->getPosition() + yvec;     
      pmat.transformVector3(camhor);
      pmat.transformVector3(camup);
       
      pmat = ((CSceneNode*)parentNode)->getAbsoluteTransformation();
      matrix4 pmatinv;
      pmat.getInverse(pmatinv);
      
      pmatinv.transformVector3(campos);
      pmatinv.transformVector3(camhor);
      pmatinv.transformVector3(camup);
       
      CVector3 right = campos - camhor;
      CVector3 up = camup - campos;
      right.Normalize();
      up.Normalize();

      GLfloat size;
      CVector3 pos;
      bool blendenabled = glIsEnabled(GL_BLEND);

      if (!blendenabled) glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_DST_ALPHA);

      m_material.activate(GL_MODULATE);

      glBegin(GL_QUADS);
      for (int i = 0; i < m_numparticles; ++i)
      {
            size = m_particlelist[i].m_size/2;
            pos = m_particlelist[i].m_pos;
            
            glColor4fv(m_particlelist[i].m_color.rgba);
            glTexCoord2f(0.0, 0.0); glVertex3fv((pos + (right + up)*(-size)).v);
            glTexCoord2f(1.0, 0.0); glVertex3fv((pos + (right - up)*size).v);
            glTexCoord2f(1.0, 1.0); glVertex3fv((pos + (right + up)*size).v);
            glTexCoord2f(0.0, 1.0); glVertex3fv((pos + (up - right)*size).v);
      }
      glEnd();
      
      if (!blendenabled) glDisable(GL_BLEND);
}

void CExplosionSceneNode::setImage(CImage *img)
{
      if (img == NULL) return;
       
      int width = img->getWidth();
      int height = img->getHeight();
      int nump = width*height;
       
      const byte *imdata; 
      byte *nimdata;
      imdata = img->getImageData();
       
      if (img->getColorFormat() == GL_RGBA)
      {
            m_material.Texture1->buildTexture(imdata, width, height, GL_RGBA);                    
      }
      else if (img->getColorFormat() == GL_RGB)
      {
            nimdata = new byte [nump*4];
                 
            for (int i = 0; i < nump; i++)
            {
                  nimdata[i*4] = imdata[i*3];
                  nimdata[i*4 + 1] = imdata[i*3 + 1];
                  nimdata[i*4 + 2] = imdata[i*3 + 2];
                  
                  if ((imdata[i*3] == 0) && (imdata[i*3 + 1] == 0) && (imdata[i*3 + 2] == 0)) 
                        nimdata[i*4 + 3] = 0;
                  else
                        nimdata[i*4 + 3] = 255;      
            }
             
            m_material.Texture1->buildTexture(nimdata, width, height, GL_RGBA);
            delete [] nimdata;
      }
}
