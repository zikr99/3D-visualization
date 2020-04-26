#ifndef __CBILLBOARDSCENENODE_H__
#define __CBILLBOARDSCENENODE_H__

#include "CSceneNode.h"
#include "SMaterial.h"

class CBillboardSceneNode: public CSceneNode
{
protected:    
      SMaterial m_material;
      CVector2<float> m_size;
      int m_alphathreshold; 
      CVector3 m_rd;   
      CVector3 m_ru;
      CVector3 m_lu;
      CVector3 m_ld;
          
      aabbox BBox;        
      virtual const aabbox& getBoundingBox() const { return BBox; }
      
      virtual void OnUpdate(unsigned int timeMs);
      virtual void OnPrepareRender();
      virtual void OnRender();
              
public:
      CBillboardSceneNode(CSceneNode *parent, CSceneManager *mngr, CImage *img = NULL,   
            const CVector2<float>& size = CVector2<float>(40.0f, 40.0f), float alphathreshold = 0.2f,
            const CVector3& position = CVector3(0.0f, 0.0f, 0.0f)):
            CSceneNode(parent, mngr, position, CVector3(0.0f, 0.0f, 0.0f),
            CVector3(1.0f, 1.0f, 1.0f))       
      { 
            m_material.MaterialType = EMT_TEXTURE;
            m_material.Texture1 = new CTexture();
            m_material.Texture1->setParameters(GL_NEAREST_MIPMAP_LINEAR, 
                  GL_NEAREST_MIPMAP_LINEAR, GL_CLAMP, GL_CLAMP);
            m_alphathreshold = int(floor(alphathreshold*255));      
            setImage(img);
            
            m_size = size;
            m_rd.set(m_size.x/2, -m_size.y/2, 0.0f);
            m_ru.set(m_size.x/2, m_size.y/2, 0.0f);
            m_lu.set(-m_size.x/2, m_size.y/2, 0.0f);
            m_ld.set(-m_size.x/2, -m_size.y/2, 0.0f);
            
            BBox.reset(m_rd.x, m_rd.y, m_rd.z);
            BBox.addInternalPoint(m_ru);
            BBox.addInternalPoint(m_lu);
            BBox.addInternalPoint(m_ld);
      }
      
      virtual ~CBillboardSceneNode()
      {
            delete m_material.Texture1;  
      }
      
      virtual void setRotation(const CVector3 &rot) {}
	  virtual void setScale(const CVector3 &sc) {}
      
      void setSize(const CVector2<float>& size)
      {
            m_size = size;
      }
      
      void setImage(CImage *img);
      
      void setAlphaThreshold(float alphathreshold) 
      { 
            m_alphathreshold = int(floor(alphathreshold*255));
      }
      
      float getAlphaThreshold() { return (float)m_alphathreshold/255; }
       
};

#endif
