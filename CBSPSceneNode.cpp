#include "CBSPSceneNode.h"
#include "CSceneManager.h"
#include "SViewFrustrum.h"

void CBSPSceneNode::setMesh(CBSPMesh *mesh)
{
      m_mesh = mesh;
      
      for (int i = 0; i < MatGroups.size(); i++) delete MatGroups[i];  
      MatGroups.clear();  
            
      for (int i = 0; i < MatGroupsBezier.size(); i++) delete MatGroupsBezier[i];
      MatGroupsBezier.clear();
      
      int numm = m_mesh->Materials.size();
     
      for (int i = 0; i < numm; i++)
      {
           array<int> *ti = new array<int>;
           MatGroups.push_back(ti);
           array<int> *tib = new array<int>;
           MatGroupsBezier.push_back(tib);
      }
      
      BBox.reset(m_mesh->BBox.MaxEdge);
      BBox.addInternalPoint(m_mesh->BBox.MinEdge);
}

int CBSPSceneNode::FindCamCluster()
{
      matrix4 cmatinv;
      matrix4 cmat = getAbsoluteTransformation();
      cmat.getInverse(cmatinv);
      
      CVector3 campos = m_scenemanager->getActiveLevel()->getActiveCamera()->getAbsolutePosition();
      cmatinv.transformVector3(campos);
      
      tBSPPlane *cplane;
      CVector3 cpnormal;
      double distance;
      
      int index = 0;
      
      while (index >= 0)
      {
           cplane = &(m_mesh->Planes[m_mesh->Nodes[index].plane]);
           
           cpnormal = CVector3(cplane->vNormal[0], cplane->vNormal[1], cplane->vNormal[2]);
           distance = cpnormal.DotProduct(campos) - cplane->d;
           
           if (distance >= 0)
                 index = m_mesh->Nodes[index].front;
           else
                 index = m_mesh->Nodes[index].back;       
      }
      
      int lfidx = -index - 1;
      return m_mesh->Leafs[lfidx].cluster;
}

bool CBSPSceneNode::IsVisibleCluster(int testcluster)
{
      if ((m_mesh->VisData.pBitsets == NULL) || (m_camcluster < 0))
           return TRUE;
      
      int i = (m_camcluster*m_mesh->VisData.bytesPerCluster) + (testcluster >> 3);
      byte visSet = m_mesh->VisData.pBitsets[i];
      
      return (visSet & (1 << (testcluster & 7))) != 0;
}

bool CBSPSceneNode::IntersectsWithViewFrustrum(tBSPLeaf *leaf)
{
      aabbox lfBox(leaf->mins[0], leaf->mins[1], leaf->mins[2], leaf->maxs[0], leaf->maxs[1], leaf->maxs[2]);
      lfBox.repair();
      
      if (lfBox.intersectsWithBox(m_cambbox))
           return TRUE;
      else
           return FALSE;
}

void CBSPSceneNode::AddUnCulledLeafs()
{     
      m_camcluster = FindCamCluster();
     
      matrix4 cmat = getAbsoluteTransformation();
      matrix4 cmatinv;
      cmat.getInverse(cmatinv);
      
      SViewFrustrum *CamFrustrum = m_scenemanager->getActiveLevel()->getActiveCamera()->getViewFrustrum();
      CamFrustrum->transform(cmatinv);
      m_cambbox = CamFrustrum->getBoundingBox();
         
      for (int i = 0; i < m_mesh->NumLeafs; i++)
      {    
           if (IsVisibleCluster(m_mesh->Leafs[i].cluster))
                 if (IntersectsWithViewFrustrum(&(m_mesh->Leafs[i])))
                       UnCulledLeafs.push_back(i);               
      } 
}

void CBSPSceneNode::drawLevel()
{     
      if (!m_mesh)
           return;
      
      int stride = sizeof(tBSPVertex); 
      
      glEnableClientState(GL_VERTEX_ARRAY);
      glEnableClientState(GL_COLOR_ARRAY);
      glEnableClientState(GL_NORMAL_ARRAY);
      glEnableClientState(GL_TEXTURE_COORD_ARRAY);
      
      glVertexPointer(3, GL_FLOAT, stride, m_mesh->Vertices[0].vPosition);
      glColorPointer(3, GL_UNSIGNED_BYTE, stride, m_mesh->Vertices[0].color);
      glNormalPointer(GL_FLOAT, stride, m_mesh->Vertices[0].vNormal);
      
      glClientActiveTextureARB(GL_TEXTURE0_ARB);
      glEnableClientState(GL_TEXTURE_COORD_ARRAY);
      glTexCoordPointer(2, GL_FLOAT, stride, m_mesh->Vertices[0].vTextureCoord);
      
      glClientActiveTextureARB(GL_TEXTURE1_ARB);
      glEnableClientState(GL_TEXTURE_COORD_ARRAY);
      glTexCoordPointer(2, GL_FLOAT, stride, m_mesh->Vertices[0].vLightmapCoord);
      
      AddUnCulledLeafs();   
      FacesFlag = new bool [m_mesh->NumFaces]; 
      for (int i = 0; i < m_mesh->NumFaces; i++) FacesFlag[i] = FALSE;
      
      tBSPLeaf *pleaf;
      int lfcidx; 
         
      for (int i = 0; i < UnCulledLeafs.size(); i++)
      {
           pleaf = &(m_mesh->Leafs[UnCulledLeafs[i]]); 
           
           for (int j = 0; j < pleaf->numOfLeafFaces; j++)
           {    
                lfcidx = pleaf->leafface + j;
                FacesFlag[m_mesh->LeafFaces[lfcidx]] = TRUE;
           }
      }    
      
      tBSPFace *fc;
      int fmidx;
      
      for (int i = 0; i < m_mesh->NumFaces; i++)
      {
           if (FacesFlag[i])
           {
                fc = &(m_mesh->Faces[i]);
                fmidx = (fc->textureID + 1)*(m_mesh->NumLightmaps + 1) + (fc->lightmapID + 1);
           
                if (fc->type == 1)
                     MatGroups[fmidx]->push_back(i);
                else if (fc->type == 2)
                     MatGroupsBezier[fmidx]->push_back(i);
           }
      }
      
      bool cullfenabled = glIsEnabled(GL_CULL_FACE);
      if (cullfenabled) glDisable(GL_CULL_FACE);
            
      int numm = m_mesh->Materials.size();
      int numfi;
      int numfib;
      
      for (int i = 0; i < numm; i++)
      {
           numfi = MatGroups[i]->size();
          
           if (numfi > 0)
           {
                 m_mesh->Materials[i]->activate();
                 for (int j = 0; j < numfi; j++) drawFace((*(MatGroups[i]))[j]);
           }
      }
      
      GLint cullfmode;
      glGetIntegerv(GL_CULL_FACE_MODE, &cullfmode);
      
      glEnable(GL_CULL_FACE);
      glCullFace(GL_FRONT);
            
      for (int i = 0; i < numm; i++)
      {
           numfib = MatGroupsBezier[i]->size();
          
           if (numfib > 0)
           {
                 m_mesh->Materials[i]->activate();
                 for (int j = 0; j < numfib; j++) drawBezier((*(MatGroupsBezier[i]))[j]);
           }
      }
      
      glCullFace(cullfmode);
      if (!cullfenabled) glDisable(GL_CULL_FACE);
      
      UnCulledLeafs.clear();
      delete [] FacesFlag;
      for (int i = 0; i < numm; i++) MatGroups[i]->clear(); 
      for (int i = 0; i < numm; i++) MatGroupsBezier[i]->clear();
}

void CBSPSceneNode::drawFace(int i)
{
      tBSPFace *fc = &(m_mesh->Faces[i]);
      
      glDrawArrays(GL_TRIANGLE_FAN, fc->vertexIndex, fc->numOfVerts);
      
      /*int stride = sizeof(tBSPVertex);
      int offset = fc->vertexIndex;
      
      glVertexPointer(3, GL_FLOAT, stride, m_mesh->Vertices[offset].vPosition);
      
      glClientActiveTextureARB(GL_TEXTURE0_ARB);
      glEnableClientState(GL_TEXTURE_COORD_ARRAY);
      glTexCoordPointer(2, GL_FLOAT, stride, m_mesh->Vertices[offset].vTextureCoord);
      
      glClientActiveTextureARB(GL_TEXTURE1_ARB);
      glEnableClientState(GL_TEXTURE_COORD_ARRAY);
      glTexCoordPointer(2, GL_FLOAT, stride, m_mesh->Vertices[offset].vLightmapCoord);
      
      glDrawElements(GL_TRIANGLES, fc->numMeshVerts,
      GL_UNSIGNED_INT, &(m_mesh->MeshVerts[fc->meshVertIndex]));*/
}

void CBSPSceneNode::drawBezier(int i)
{     
      int bidx;
      CBSPBezier *bzr;
      
      for (int j = 0; j < m_mesh->FaceNumPatches[i]; j++)
      {
           bidx = m_mesh->FaceFirstPatch[i] + j;
           bzr = m_mesh->Beziers[bidx];
           
           glVertexPointer(3, GL_FLOAT, sizeof(tBSPVertex), &(bzr->vertices[0].vPosition));
           
           glClientActiveTextureARB(GL_TEXTURE0_ARB);
           glTexCoordPointer(2, GL_FLOAT, sizeof(tBSPVertex), &(bzr->vertices[0].vTextureCoord));
           
           glClientActiveTextureARB(GL_TEXTURE1_ARB);
           glTexCoordPointer(2, GL_FLOAT, sizeof(tBSPVertex), &(bzr->vertices[0].vLightmapCoord));
           
           glDrawElements(GL_TRIANGLES, bzr->level*bzr->level*6, GL_UNSIGNED_INT, bzr->indices);
      }
}

void CBSPSceneNode::OnPrepareRender()
{ 
      m_scenemanager->registerNodeForRendering(this, ESNRP_SOLID);
}

void CBSPSceneNode::OnRender()
{     
      drawLevel(); 
}
