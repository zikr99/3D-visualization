#include "CBillboardSceneNode.h"
#include "CSceneManager.h"

void CBillboardSceneNode::OnUpdate(unsigned int timeMs)
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
       
       CVector3 horizontal = campos - camhor;
       CVector3 vertical = camup - campos;
       horizontal.Normalize();
       vertical.Normalize();
       
       m_rd = horizontal*(m_size.x/2) + vertical*(-m_size.y/2);
       m_ru = horizontal*(m_size.x/2) + vertical*(m_size.y/2);
       m_lu = horizontal*(-m_size.x/2) + vertical*(m_size.y/2);
       m_ld = horizontal*(-m_size.x/2) + vertical*(-m_size.y/2);
       
       BBox.reset(m_rd.x, m_rd.y, m_rd.z);
       BBox.addInternalPoint(m_ru);
       BBox.addInternalPoint(m_lu);
       BBox.addInternalPoint(m_ld);
}

void CBillboardSceneNode::OnPrepareRender()
{
       m_scenemanager->registerNodeForRendering(this, ESNRP_TRANSPARENT);
}

void CBillboardSceneNode::OnRender()
{
       m_material.activate(GL_REPLACE);
       
       bool alphaenabled = glIsEnabled(GL_ALPHA_TEST);
       bool blendenabled = glIsEnabled(GL_BLEND);
       
       if (!alphaenabled) glEnable(GL_ALPHA_TEST);
       if (!blendenabled) glEnable(GL_BLEND);
       
       glAlphaFunc(GL_GREATER, 0.0f);
       glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
       
       glBegin(GL_QUADS);
             glTexCoord2f(1.0f, 0.0f); glVertex3f(m_rd.x, m_rd.y, m_rd.z);
             glTexCoord2f(1.0f, 1.0f); glVertex3f(m_ru.x, m_ru.y, m_ru.z);
             glTexCoord2f(0.0f, 1.0f); glVertex3f(m_lu.x, m_lu.y, m_lu.z);
             glTexCoord2f(0.0f, 0.0f); glVertex3f(m_ld.x, m_ld.y, m_ld.z);
       glEnd();
       
       if (!alphaenabled) glDisable(GL_ALPHA_TEST);
       if (!blendenabled) glDisable(GL_BLEND);
}

void CBillboardSceneNode::setImage(CImage *img)
{
       if (img == NULL) return;
       
       int width = img->getWidth();
       int height = img->getHeight();
       int nump = width*height;
       
       const byte *imdata; 
       byte *nimdata;
       imdata = img->getImageData();
       int nimalpha;
       
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
                   
                   nimalpha = int(floor(sqrt((imdata[i*3]*imdata[i*3] + 
                         imdata[i*3 + 1]*imdata[i*3 + 1] + imdata[i*3 + 2]*imdata[i*3] + 2)/3)));
                          
                   if (nimalpha > m_alphathreshold)
                         nimdata[i*4 + 3] = nimalpha;
                   else
                         nimdata[i*4 + 3] = 0;      
             }
             
             m_material.Texture1->buildTexture(nimdata, width, height, GL_RGBA);
             delete [] nimdata;
       }
}
