#ifndef __CSKYBOXSCENENODE_H__
#define __CSKYBOXSCENENODE_H__

#include "CSceneNode.h"

enum E_SKY_BOX_SIDE
{
      ESBS_FRONT = 0,
      ESBS_BACK,
      ESBS_LEFT,
      ESBS_RIGHT,
      ESBS_TOP,
      ESBS_BOTTOM,
      ESBS_COUNT
};

class CSkyBoxSceneNode: public CSceneNode
{
protected:
      aabbox BBox;
      SMaterial m_sides[ESBS_COUNT];
      
      virtual const aabbox& getBoundingBox() const { return BBox; }
      
      virtual void OnPrepareRender()
      {
            m_scenemanager->registerNodeForRendering(this, ESNRP_SOLID);  
      }
          
      virtual void OnRender()
      {     
            m_sides[ESBS_TOP].activate();  
            glBegin(GL_QUADS);
                  glTexCoord2f(1.0f, 1.0f); glVertex3f(BBox.MaxEdge.x, BBox.MaxEdge.y, BBox.MaxEdge.z);
                  glTexCoord2f(0.0f, 1.0f); glVertex3f(BBox.MinEdge.x, BBox.MaxEdge.y, BBox.MaxEdge.z);
                  glTexCoord2f(0.0f, 0.0f); glVertex3f(BBox.MinEdge.x, BBox.MaxEdge.y, BBox.MinEdge.z);
                  glTexCoord2f(1.0f, 0.0f); glVertex3f(BBox.MaxEdge.x, BBox.MaxEdge.y, BBox.MinEdge.z);
            glEnd();
            m_sides[ESBS_BOTTOM].activate();
            glBegin(GL_QUADS);
                  glTexCoord2f(0.0f, 0.0f); glVertex3f(BBox.MinEdge.x, BBox.MinEdge.y, BBox.MinEdge.z);
                  glTexCoord2f(0.0f, 1.0f); glVertex3f(BBox.MinEdge.x, BBox.MinEdge.y, BBox.MaxEdge.z);
                  glTexCoord2f(1.0f, 1.0f); glVertex3f(BBox.MaxEdge.x, BBox.MinEdge.y, BBox.MaxEdge.z);
                  glTexCoord2f(1.0f, 0.0f); glVertex3f(BBox.MaxEdge.x, BBox.MinEdge.y, BBox.MinEdge.z);            
            glEnd();
            m_sides[ESBS_LEFT].activate();
            glBegin(GL_QUADS);
                  glTexCoord2f(0.0f, 1.0f); glVertex3f(BBox.MinEdge.x, BBox.MaxEdge.y, BBox.MaxEdge.z);
                  glTexCoord2f(0.0f, 0.0f); glVertex3f(BBox.MinEdge.x, BBox.MinEdge.y, BBox.MaxEdge.z);
                  glTexCoord2f(1.0f, 0.0f); glVertex3f(BBox.MinEdge.x, BBox.MinEdge.y, BBox.MinEdge.z);
                  glTexCoord2f(1.0f, 1.0f); glVertex3f(BBox.MinEdge.x, BBox.MaxEdge.y, BBox.MinEdge.z);
            glEnd();
            m_sides[ESBS_RIGHT].activate();
            glBegin(GL_QUADS);
                  glTexCoord2f(1.0f, 1.0f); glVertex3f(BBox.MaxEdge.x, BBox.MaxEdge.y, BBox.MaxEdge.z);
                  glTexCoord2f(0.0f, 1.0f); glVertex3f(BBox.MaxEdge.x, BBox.MaxEdge.y, BBox.MinEdge.z);
                  glTexCoord2f(0.0f, 0.0f); glVertex3f(BBox.MaxEdge.x, BBox.MinEdge.y, BBox.MinEdge.z);
                  glTexCoord2f(1.0f, 0.0f); glVertex3f(BBox.MaxEdge.x, BBox.MinEdge.y, BBox.MaxEdge.z);
            glEnd();
            m_sides[ESBS_FRONT].activate();
            glBegin(GL_QUADS);
                  glTexCoord2f(1.0f, 1.0f); glVertex3f(BBox.MinEdge.x, BBox.MaxEdge.y, BBox.MaxEdge.z);
                  glTexCoord2f(0.0f, 1.0f); glVertex3f(BBox.MaxEdge.x, BBox.MaxEdge.y, BBox.MaxEdge.z);
                  glTexCoord2f(0.0f, 0.0f); glVertex3f(BBox.MaxEdge.x, BBox.MinEdge.y, BBox.MaxEdge.z);
                  glTexCoord2f(1.0f, 0.0f); glVertex3f(BBox.MinEdge.x, BBox.MinEdge.y, BBox.MaxEdge.z);
            glEnd();
            m_sides[ESBS_BACK].activate();
            glBegin(GL_QUADS);
                  glTexCoord2f(1.0f, 1.0f); glVertex3f(BBox.MaxEdge.x, BBox.MaxEdge.y, BBox.MinEdge.z);
                  glTexCoord2f(0.0f, 1.0f); glVertex3f(BBox.MinEdge.x, BBox.MaxEdge.y, BBox.MinEdge.z);
                  glTexCoord2f(0.0f, 0.0f); glVertex3f(BBox.MinEdge.x, BBox.MinEdge.y, BBox.MinEdge.z);
                  glTexCoord2f(1.0f, 0.0f); glVertex3f(BBox.MaxEdge.x, BBox.MinEdge.y, BBox.MinEdge.z);
            glEnd(); 
      }   
          
public:
      CSkyBoxSceneNode(CSceneNode *parent, CSceneManager *mngr, float sizex, float sizey,
            float sizez, const CVector3& position = CVector3(0.0f, 0.0f, 0.0f),
            const CVector3& rotation = CVector3(0.0f, 0.0f, 0.0f),
            const CVector3& scale = CVector3(1.0f, 1.0f, 1.0f)): 
            CSceneNode(parent, mngr, position, rotation, scale)
      {
            setSize(sizex, sizey, sizez);
            for (int i = 0; i < ESBS_COUNT; i++) m_sides[i].MaterialType = EMT_TEXTURE;
            
            CTexture *tx = new CTexture();
            setTexture(ESBS_FRONT, tx);
            tx = new CTexture();
            setTexture(ESBS_BACK, tx);
            tx = new CTexture();
            setTexture(ESBS_LEFT, tx);
            tx = new CTexture();
            setTexture(ESBS_RIGHT, tx);
            tx = new CTexture();
            setTexture(ESBS_TOP, tx);
            tx = new CTexture();
            setTexture(ESBS_BOTTOM, tx);
      }
       
      CSkyBoxSceneNode(CSceneNode *parent, CSceneManager *mngr, float sizex, float sizey,
            float sizez, CTexture *front, CTexture *back, CTexture *left,
            CTexture *right, CTexture *top, CTexture *bottom,  
            const CVector3& position = CVector3(0.0f, 0.0f, 0.0f),
            const CVector3& rotation = CVector3(0.0f, 0.0f, 0.0f),
            const CVector3& scale = CVector3(1.0f, 1.0f, 1.0f)): 
            CSceneNode(parent, mngr, position, rotation, scale)
      {
            setSize(sizex, sizey, sizez);
            for (int i = 0; i < ESBS_COUNT; i++) m_sides[i].MaterialType = EMT_TEXTURE;
            
            CTexture *tx;
            
            if (front)
                  setTexture(ESBS_FRONT, front);
            else
            {
                  tx = new CTexture();
                  setTexture(ESBS_FRONT, tx);
            }
            
            if (back)
                  setTexture(ESBS_BACK, back);
            else
            {
                  tx = new CTexture();
                  setTexture(ESBS_FRONT, tx);
            }
            
            if (left)
                  setTexture(ESBS_LEFT, left);
            else
            {
                  tx = new CTexture();
                  setTexture(ESBS_FRONT, tx);
            }
            
            if (right)
                  setTexture(ESBS_RIGHT, right);
            else
            {
                  tx = new CTexture();
                  setTexture(ESBS_FRONT, tx);
            }
            
            if (top)
                  setTexture(ESBS_TOP, top);
            else
            {
                  tx = new CTexture();
                  setTexture(ESBS_FRONT, tx);
            }
            
            if (bottom)
                  setTexture(ESBS_BOTTOM, bottom);
            else
            {
                  tx = new CTexture();
                  setTexture(ESBS_FRONT, tx);
            }
      }
      
      CSkyBoxSceneNode(CSceneNode *parent, CSceneManager *mngr, float sizex, float sizey, 
            float sizez, char *frontfile, char *backfile, char *leftfile,
            char *rightfile, char *topfile, char *bottomfile,  
            const CVector3& position = CVector3(0.0f, 0.0f, 0.0f),
            const CVector3& rotation = CVector3(0.0f, 0.0f, 0.0f),
            const CVector3& scale = CVector3(1.0f, 1.0f, 1.0f)): 
            CSceneNode(parent, mngr, position, rotation, scale)
      {
            setSize(sizex, sizey, sizez);
            for (int i = 0; i < ESBS_COUNT; i++) m_sides[i].MaterialType = EMT_TEXTURE;
            
            CTexture *tx;
            
            if (frontfile)
            {
                  tx = new CTexture();
                  tx->buildTextureBMP24(frontfile);
                  setTexture(ESBS_FRONT, tx);
            }
            
            if (backfile)
            {
                  tx = new CTexture();
                  tx->buildTextureBMP24(backfile);
                  setTexture(ESBS_BACK, tx); 
            }
            
            if (leftfile)
            {
                  tx = new CTexture();
                  tx->buildTextureBMP24(leftfile);
                  setTexture(ESBS_LEFT, tx);
            }
            
            if (rightfile)
            {
                  tx = new CTexture();
                  tx->buildTextureBMP24(rightfile);
                  setTexture(ESBS_RIGHT, tx);
            }
            
            if (topfile)
            {
                  tx = new CTexture();
                  tx->buildTextureBMP24(topfile);
                  setTexture(ESBS_TOP, tx);
            }
            
            if (bottomfile)
            {
                  tx = new CTexture();
                  tx->buildTextureBMP24(bottomfile);
                  setTexture(ESBS_BOTTOM, tx);
            }
      }
      
      virtual ~CSkyBoxSceneNode()
      {
            if (m_sides[0].Texture1) delete m_sides[0].Texture1;
            if (m_sides[1].Texture1) delete m_sides[1].Texture1;
            if (m_sides[2].Texture1) delete m_sides[2].Texture1;
            if (m_sides[3].Texture1) delete m_sides[3].Texture1;
            if (m_sides[4].Texture1) delete m_sides[4].Texture1;
            if (m_sides[5].Texture1) delete m_sides[5].Texture1;
      } 
      
      virtual void setTexture(E_SKY_BOX_SIDE side, CTexture *tex)
      {
            if (m_sides[side].Texture1) delete m_sides[side].Texture1;
            m_sides[side].Texture1 = tex;  
      }
      
      virtual void setSize(float sizex, float sizey, float sizez)
      {
            BBox.reset(0.0f, 0.0f, 0.0f);
            BBox.addInternalPoint(sizex/2, sizey/2, sizez/2);
            BBox.addInternalPoint(-sizex/2, -sizey/2, -sizez/2); 
      }
      
	  virtual void setRotation(const CVector3 &rot) {}
      
};

#endif
