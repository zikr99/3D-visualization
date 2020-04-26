#include "CMD2SceneNode.h"
#include "CSceneManager.h"

int MD2_START_FRAME[] = {0, 40, 47, 61, 67, 74, 96, 113, 123, 136, 155, 162, 170, 178, 186, 191};
int MD2_END_FRAME[] = {39, 46, 60, 66, 73, 95, 112, 122, 135, 154, 161, 169, 177, 185, 190, 198};

void CMD2SceneNode::setMesh(CMD2Mesh *mesh)
{
      m_mesh = mesh;
      m_numframes = m_mesh->NumFrames;
      
      m_framestart = 0;
      m_frameend = 0;
      m_loop = true;
      
      m_currentframe = 0;
      m_nextframe = 1;
      m_interpol = 0.0f;
      
      m_lastcurrentframe = 0;
      m_lastnextframe = 1;
      m_lastinterpol = 0.0f;
}

void CMD2SceneNode::setState(int framestart, int frameend, bool loop, int speed)
{
      if (framestart <= frameend)
      {
           m_framestart = framestart;
           m_frameend = frameend;
           
           m_currentframe = m_framestart;
           m_nextframe = m_framestart + 1;
           m_interpol = 0.0f;
           
           m_lastcurrentframe = m_framestart;
           m_lastnextframe = m_framestart + 1;
           m_lastinterpol = 0.0f;
      }
      
      m_loop = loop;
     
      if (speed > 0)
           m_speed = speed;
}

void CMD2SceneNode::setState(E_MD2_STATE state, bool loop, int speed)
{
      m_framestart = MD2_START_FRAME[state];
      m_frameend = MD2_END_FRAME[state];
      
      m_currentframe = m_framestart;
      m_nextframe = m_framestart + 1;
      m_interpol = 0.0f;
      
      m_lastcurrentframe = m_framestart;
      m_lastnextframe = m_framestart + 1;
      m_lastinterpol = 0.0f;
     
      m_loop = loop;
     
      if (speed > 0)
           m_speed = speed;   
}

void CMD2SceneNode::setFrame(int framenum)
{
      m_framestart = framenum;
      m_frameend = framenum;
      
      m_currentframe = m_framestart;
      m_nextframe = m_framestart + 1;
      m_interpol = 0.0f;
      
      m_lastcurrentframe = m_framestart;
      m_lastnextframe = m_framestart + 1;
      m_lastinterpol = 0.0f;
}

void CMD2SceneNode::setSpeed(int speed)
{
      if (speed > 0)
           m_speed = speed;
}

void CMD2SceneNode::CalculateBoundingBox()
{ 
      if (m_framestart < m_frameend)
      {
           CVector3 IntMinEdge = m_mesh->BBoxes[m_lastcurrentframe].MinEdge*m_lastinterpol + 
                 m_mesh->BBoxes[m_lastnextframe].MinEdge*(1.0f - m_lastinterpol);
                 
           CVector3 IntMaxEdge = m_mesh->BBoxes[m_lastcurrentframe].MaxEdge*m_lastinterpol + 
                 m_mesh->BBoxes[m_lastnextframe].MaxEdge*(1.0f - m_lastinterpol);
           
           BBox.reset(IntMinEdge);
           BBox.addInternalPoint(IntMaxEdge);
      }
      else
      {
           BBox.reset(m_mesh->BBoxes[m_framestart].MinEdge);
           BBox.addInternalPoint(m_mesh->BBoxes[m_framestart].MaxEdge);
      }
}

void CMD2SceneNode::OnUpdate(unsigned int timeMs)
{
      if (m_framestart < m_frameend)
      {
           m_lastinterpol = m_interpol;            
           m_interpol = m_interpol + 0.008f*timeMs*m_speed/2000.0f;
           
           if (m_interpol > 1.0f)
           {
                 m_interpol = m_interpol - 1.0f;         
                 
                 m_lastcurrentframe = m_currentframe;
                 m_currentframe = m_currentframe + 1;
                 if (m_currentframe >= m_frameend) m_currentframe = m_framestart;         
                 
                 m_lastnextframe = m_nextframe;
                 m_nextframe = m_currentframe + 1;             
                 if (m_nextframe >= m_frameend) m_nextframe = m_framestart;
           }
      }
      
      CalculateBoundingBox();
}

void CMD2SceneNode::OnPrepareRender()
{
      m_scenemanager->registerNodeForRendering(this, ESNRP_SOLID);
}

void CMD2SceneNode::OnRender()
{
      mesh_t *mIdx;
      int numtr;
      texCoord_t *tIdx;
      vector_t *vList;
      vector_t *nextvList;
      
      float x1, y1, z1, x2, y2, z2;
      vector_t vertex[3];
     
      if (m_framestart == m_frameend)
      {
           if ((m_framestart < 0) || (m_framestart > m_mesh->NumFrames - 1))
                 return;
  
           if (!m_mesh) 
                 return;
      
           mIdx = m_mesh->Triangles;              // pointer to the frame
           numtr = m_mesh->NumTriangles;
           tIdx = m_mesh->STs;
           vList = &(m_mesh->Vertices[m_framestart*m_mesh->NumVertices]);
           
           m_mesh->Material.activate();
  
           glBegin(GL_TRIANGLES);
           for(int i = 0; i < numtr; i++)
           {
                 CalculateNormal(vList[mIdx[i].meshIndex[0]].point,
                 vList[mIdx[i].meshIndex[2]].point, vList[mIdx[i].meshIndex[1]].point);

                 glTexCoord2f(tIdx[mIdx[i].stIndex[0]].s, tIdx[mIdx[i].stIndex[0]].t);
                 glVertex3fv(vList[mIdx[i].meshIndex[0]].point);

                 glTexCoord2f(tIdx[mIdx[i].stIndex[2]].s, tIdx[mIdx[i].stIndex[2]].t);
                 glVertex3fv(vList[mIdx[i].meshIndex[2]].point);

                 glTexCoord2f(tIdx[mIdx[i].stIndex[1]].s, tIdx[mIdx[i].stIndex[1]].t);
                 glVertex3fv(vList[mIdx[i].meshIndex[1]].point);                                   
           }
           glEnd();
      }
      else
      {
           if (!m_mesh)
                 return;
           
           mIdx = m_mesh->Triangles;              // pointer to the frame
           numtr = m_mesh->NumTriangles;
           tIdx = m_mesh->STs;
           
           vList = &(m_mesh->Vertices[m_currentframe*m_mesh->NumVertices]);
           nextvList = &(m_mesh->Vertices[m_nextframe*m_mesh->NumVertices]);
           
           m_mesh->Material.activate();
           
           glBegin(GL_TRIANGLES);
           for (int i = 0; i < numtr; i++)
           {
                 // get first points of each frame
                 x1 = vList[mIdx[i].meshIndex[0]].point[0];
                 y1 = vList[mIdx[i].meshIndex[0]].point[1];
                 z1 = vList[mIdx[i].meshIndex[0]].point[2];
                 x2 = nextvList[mIdx[i].meshIndex[0]].point[0];
                 y2 = nextvList[mIdx[i].meshIndex[0]].point[1];
                 z2 = nextvList[mIdx[i].meshIndex[0]].point[2];
                 // store first interpolated vertex of triangle
                 vertex[0].point[0] = x1 + m_interpol*(x2 - x1);
                 vertex[0].point[1] = y1 + m_interpol*(y2 - y1);
                 vertex[0].point[2] = z1 + m_interpol*(z2 - z1);

                 // get second points of each frame
                 x1 = vList[mIdx[i].meshIndex[2]].point[0];
                 y1 = vList[mIdx[i].meshIndex[2]].point[1];
                 z1 = vList[mIdx[i].meshIndex[2]].point[2];
                 x2 = nextvList[mIdx[i].meshIndex[2]].point[0];
                 y2 = nextvList[mIdx[i].meshIndex[2]].point[1];
                 z2 = nextvList[mIdx[i].meshIndex[2]].point[2];
                 // store second interpolated vertex of triangle
                 vertex[2].point[0] = x1 + m_interpol*(x2 - x1);
                 vertex[2].point[1] = y1 + m_interpol*(y2 - y1);
                 vertex[2].point[2] = z1 + m_interpol*(z2 - z1);

                 // get third points of each frame
                 x1 = vList[mIdx[i].meshIndex[1]].point[0];
                 y1 = vList[mIdx[i].meshIndex[1]].point[1];
                 z1 = vList[mIdx[i].meshIndex[1]].point[2];
                 x2 = nextvList[mIdx[i].meshIndex[1]].point[0];
                 y2 = nextvList[mIdx[i].meshIndex[1]].point[1];
                 z2 = nextvList[mIdx[i].meshIndex[1]].point[2];
                 // store third interpolated vertex of triangle
                 vertex[1].point[0] = x1 + m_interpol*(x2 - x1);
                 vertex[1].point[1] = y1 + m_interpol*(y2 - y1);
                 vertex[1].point[2] = z1 + m_interpol*(z2 - z1);
    
                 CalculateNormal(vertex[0].point, vertex[2].point, vertex[1].point);
   
                 glTexCoord2f(tIdx[mIdx[i].stIndex[0]].s, tIdx[mIdx[i].stIndex[0]].t);
                 glVertex3fv(vertex[0].point);
   
                 glTexCoord2f(tIdx[mIdx[i].stIndex[2]].s, tIdx[mIdx[i].stIndex[2]].t);
                 glVertex3fv(vertex[2].point);

                 glTexCoord2f(tIdx[mIdx[i].stIndex[1]].s, tIdx[mIdx[i].stIndex[1]].t);
                 glVertex3fv(vertex[1].point);
           }
           glEnd();
      } 
}

void CMD2SceneNode::CalculateNormal(float *p1, float *p2, float *p3)
{
      CVector3 a;
      CVector3 b;
      CVector3 reslt;
      
      a.set(p1[0] - p2[0], p1[1] - p2[1], p1[2] - p2[2]);
      b.set(p1[0] - p3[0], p1[1] - p3[1], p1[2] - p3[2]);

      reslt = a.CrossProduct(b);
      reslt.Normalize();

      glNormal3f(reslt.x, reslt.y, reslt.z);
}
