#include "CCollisionManager.h"

bool CCollisionManager::getCollisionPoint(const CVector3 &startray, const CVector3 &endray,
      CBSPSceneNode* selector, CVector3 &outCollisionPoint)
{
      gcpmesh = selector->getMesh();
      gcpinputstart = startray;
      gcpinputend = endray;
      
      matrix4 cmat = selector->getAbsoluteTransformation();
      matrix4 cmatinv;
      cmat.getInverse(cmatinv);
      
      cmatinv.transformVector3(gcpinputstart);
      cmatinv.transformVector3(gcpinputend);
      
      gcpoutputfraction = 2.0f;
      gcpstartout = TRUE;
      gcpallinsolid = FALSE;
      
      CheckNodeCollPoint(0, 0.0f, 1.0f, gcpinputstart, gcpinputend);
      
      if (gcpoutputfraction > 1.0f)
            return FALSE; 
      
      CVector3 movvector = gcpinputend - gcpinputstart;
      
      float offsetlength = movvector.Length();
      float offsetfraction = gcpoutputfraction - EPSILON/offsetlength;
      //if (offsetfraction < 0.0f) offsetfraction = 0.0f;
      
      gcpoutputpoint = gcpinputstart + movvector*offsetfraction;
      
      cmat.transformVector3(gcpoutputpoint);
      outCollisionPoint = gcpoutputpoint;
      
      return TRUE;
}

void CCollisionManager::CheckNodeCollPoint(int nodeIndex, float startfraction, float endfraction, 
      CVector3 startfractpoint, CVector3 endfractpoint)
{
      tBSPLeaf *leaf;
      tBSPBrush *brush;
      
      if (nodeIndex < 0)
      {
            leaf = &(gcpmesh->Leafs[-(nodeIndex + 1)]);
            
            for (int i = 0; i < leaf->numOfLeafBrushes; i++)
            {
                  brush = &(gcpmesh->Brushes[gcpmesh->LeafBrushes[leaf->leafBrush + i]]);
                  
                  if ((brush->numOfBrushSides > 0) && (gcpmesh->Textures[brush->textureID].contents & 1))
                        CheckBrushCollPoint(brush);   
            }
            
            return;        
      }
      
      CVector3 planenormal;
      float fraction1, fraction2, invfrac;
      int side;
      float middlefraction;
      CVector3 middlefractpoint;
      
      tBSPNode *node = &(gcpmesh->Nodes[nodeIndex]);
      tBSPPlane *plane = &(gcpmesh->Planes[node->plane]);
      planenormal.set(plane->vNormal[0], plane->vNormal[1], plane->vNormal[2]);
      planenormal.Normalize();
      
      float startdistance = startfractpoint.DotProduct(planenormal) - plane->d;
      float enddistance = endfractpoint.DotProduct(planenormal) - plane->d;
      
      if ((startdistance > 0) && (enddistance > 0))
            CheckNodeCollPoint(node->front, startfraction, endfraction, startfractpoint, endfractpoint);             
      else if ((startdistance < 0) && (enddistance < 0))
            CheckNodeCollPoint(node->back, startfraction, endfraction, startfractpoint, endfractpoint);
      else
      {
            if (startdistance > enddistance)
            {
                  invfrac = 1.0f/(startdistance - enddistance);            
                  fraction1 = (startdistance + EPSILON)*invfrac;
                  fraction2 = (startdistance - EPSILON)*invfrac;
                  side = 0;           
            }
            else if (startdistance < enddistance)
            {
                  invfrac = 1.0f/(startdistance - enddistance);
                  fraction1 = (startdistance - EPSILON)*invfrac;
                  fraction2 = (startdistance + EPSILON)*invfrac;
                  side = 1;
            }
            else
            {
                  fraction1 = 0.0f;
                  fraction2 = 0.0f;
                  side = 0;
            }
            
            if (fraction1 < 0.0f) fraction1 = 0.0f;
            else if (fraction1 > 1.0f) fraction1 = 1.0f;
            
            if (fraction2 < 0.0f) fraction2 = 0.0f;
            else if (fraction2 > 1.0f) fraction2 = 1.0f;
            
            middlefraction = startfraction + (endfraction - startfraction)*fraction1;
            middlefractpoint = startfractpoint + (endfractpoint - startfractpoint)*fraction1;
            
            CheckNodeCollPoint((side == 0)?node->front:node->back, startfraction, middlefraction, 
                  startfractpoint, middlefractpoint);
            
            middlefraction = startfraction + (endfraction - startfraction)*fraction2;
            middlefractpoint = startfractpoint + (endfractpoint - startfractpoint)*fraction2;
            
            CheckNodeCollPoint((side == 0)?node->back:node->front, middlefraction, endfraction, 
                  middlefractpoint, endfractpoint);
      }
}

void CCollisionManager::CheckBrushCollPoint(tBSPBrush *gcpbrush)
{
      float stfract = -1.0f;
      float enfract = 1.0f;
      bool stout = FALSE;
      bool enout = FALSE;
      
      tBSPBrushSide *brushside;
      tBSPPlane *plane;
      CVector3 planenormal;
      float stdist;
      float endist;
      float fract;
      
      for (int i = 0; i < gcpbrush->numOfBrushSides; i++)
      {
            brushside = &(gcpmesh->BrushSides[gcpbrush->brushSide + i]);
            plane = &(gcpmesh->Planes[brushside->plane]);
            planenormal.set(plane->vNormal[0], plane->vNormal[1], plane->vNormal[2]);
            planenormal.Normalize();
            
            stdist = gcpinputstart.DotProduct(planenormal) - plane->d;
            endist = gcpinputend.DotProduct(planenormal) - plane->d;
             
            if (stdist >= 0) stout = TRUE;
            if (endist >= 0) enout = TRUE;
            
            if ((stdist > 0) && (endist > 0)) return;
            if ((stdist < 0) && (endist < 0)) continue;
            
            if (stdist > endist)
            {
                  fract = stdist/(stdist - endist);
                  if (fract > stfract) stfract = fract;     
            }
            else
            {
                  fract = stdist/(stdist - endist);
                  if (fract < enfract) enfract = fract;
            }
            
      }
      
      if (stout == FALSE)
      { 
            gcpstartout = FALSE;
            if (enout == FALSE) gcpallinsolid = TRUE;
            return;    
      }
      
      if (stfract <= enfract)
      {
            if ((stfract > -1.0f) && (stfract < gcpoutputfraction))
            {
                  if (stfract < 0.0f) stfract = 0.0f;
                  gcpoutputfraction = stfract;      
            }      
      }
}

bool CCollisionManager::getCollisionMovement(const CVector3 &startposition, const CVector3 &movement,
      float radius, CBSPSceneNode* selector, CVector3 &outCollisionMovement)
{
      gcmmesh = selector->getMesh();
      gcmstartposition = startposition;
      CVector3 endposition = startposition + movement;
      
      matrix4 cmat = selector->getAbsoluteTransformation();
      matrix4 cmatinv;
      cmat.getInverse(cmatinv);
      
      cmatinv.transformVector3(gcmstartposition);
      cmatinv.transformVector3(endposition);
      gcmmovvector = endposition - gcmstartposition;
      
      CVector3 vorg(0.0f, 0.0f, 0.0f);
      CVector3 vradius(radius, 0.0f, 0.0f);
      cmatinv.transformVector3(vorg);
      cmatinv.transformVector3(vradius);
      vradius = vradius - vorg; 
      gcmradius = vradius.Length();
      
      gcmoutputfraction = 2.0f;
      
      CheckNodeCollMov(0, 0.0f, 1.0f, gcmstartposition, endposition);
      
      if (gcmoutputfraction > 1.0f)
            return FALSE;
      
      float offsetlength = gcmmovvector.Length();
      float offsetfraction = gcmoutputfraction - EPSILON/offsetlength;
      //if (offsetfraction < 0.0f) offsetfraction = 0.0f;
       
      CVector3 nendposition = gcmstartposition + gcmmovvector*offsetfraction;
      cmat.transformVector3(gcmstartposition);
      cmat.transformVector3(nendposition);
      
      outCollisionMovement = nendposition - gcmstartposition;
      
      return TRUE; 
}

void CCollisionManager::CheckNodeCollMov(int nodeIndex, float startfraction, float endfraction, 
      CVector3 startfractpoint, CVector3 endfractpoint)
{
      tBSPLeaf *leaf;
      tBSPBrush *brush;
      
      if (nodeIndex < 0)
      {
            leaf = &(gcmmesh->Leafs[-(nodeIndex + 1)]);
            
            for (int i = 0; i < leaf->numOfLeafBrushes; i++)
            {
                  brush = &(gcmmesh->Brushes[gcmmesh->LeafBrushes[leaf->leafBrush + i]]);
                  
                  if ((brush->numOfBrushSides > 0) && (gcmmesh->Textures[brush->textureID].contents & 1))
                        CheckBrushCollMov(brush);   
            }
            
            return;        
      }
      
      CVector3 planenormal;
      float fraction1, fraction2, invfrac;
      int side;
      float middlefraction;
      CVector3 middlefractpoint;
      
      tBSPNode *node = &(gcmmesh->Nodes[nodeIndex]);
      tBSPPlane *plane = &(gcmmesh->Planes[node->plane]);
      planenormal.set(plane->vNormal[0], plane->vNormal[1], plane->vNormal[2]);
      planenormal.Normalize();
      
      float startdistance = startfractpoint.DotProduct(planenormal) - plane->d;
      float enddistance = endfractpoint.DotProduct(planenormal) - plane->d;
      
      if ((startdistance > gcmradius) && (enddistance > gcmradius))
            CheckNodeCollMov(node->front, startfraction, endfraction, startfractpoint, endfractpoint);             
      else if ((startdistance < -gcmradius) && (enddistance < -gcmradius))
            CheckNodeCollMov(node->back, startfraction, endfraction, startfractpoint, endfractpoint);
      else
      {
            if (startdistance > enddistance)
            {
                  invfrac = 1.0f/(startdistance - enddistance);            
                  fraction1 = (startdistance + gcmradius + EPSILON)*invfrac;
                  fraction2 = (startdistance - gcmradius - EPSILON)*invfrac;
                  side = 0;           
            }
            else if (startdistance < enddistance)
            {
                  invfrac = 1.0f/(startdistance - enddistance);
                  fraction1 = (startdistance - gcmradius - EPSILON)*invfrac;
                  fraction2 = (startdistance + gcmradius + EPSILON)*invfrac;
                  side = 1;
            }
            else
            {
                  fraction1 = 0.0f;
                  fraction2 = 0.0f;
                  side = 0;
            }
            
            if (fraction1 < 0.0f) fraction1 = 0.0f;
            else if (fraction1 > 1.0f) fraction1 = 1.0f;
            
            if (fraction2 < 0.0f) fraction2 = 0.0f;
            else if (fraction2 > 1.0f) fraction2 = 1.0f;
            
            middlefraction = startfraction + (endfraction - startfraction)*fraction1;
            middlefractpoint = startfractpoint + (endfractpoint - startfractpoint)*fraction1;
            
            CheckNodeCollMov((side == 0)?node->front:node->back, startfraction, middlefraction, 
                  startfractpoint, middlefractpoint);
            
            middlefraction = startfraction + (endfraction - startfraction)*fraction2;
            middlefractpoint = startfractpoint + (endfractpoint - startfractpoint)*fraction2;
            
            CheckNodeCollMov((side == 0)?node->back:node->front, middlefraction, endfraction, 
                  middlefractpoint, endfractpoint);
      }
}

void CCollisionManager::CheckBrushCollMov(tBSPBrush *gcmbrush)
{     
      float stfract = -1.0f;
      float enfract = 1.0f;
      
      tBSPBrushSide *brushside;
      tBSPPlane *plane;
      CVector3 planenormal;
      CVector3 ipstart;
      CVector3 ipend;
      float stdist;
      float endist;
      float fract;
      
      for (int i = 0; i < gcmbrush->numOfBrushSides; i++)
      {
            brushside = &(gcmmesh->BrushSides[gcmbrush->brushSide + i]);
            plane = &(gcmmesh->Planes[brushside->plane]);
            planenormal.set(plane->vNormal[0], plane->vNormal[1], plane->vNormal[2]);
            planenormal.Normalize();
            
            ipstart = gcmstartposition - planenormal*gcmradius;
            ipend = ipstart + gcmmovvector; 
            
            stdist = ipstart.DotProduct(planenormal) - plane->d;
            endist = ipend.DotProduct(planenormal) - plane->d;
            
            if ((stdist > 0) && (endist > 0)) return;
            if ((stdist < 0) && (endist < 0)) continue;
            
            if (stdist > endist)
            {
                  fract = stdist/(stdist - endist);
                  if (fract > stfract) stfract = fract;     
            }
            else
            {
                  fract = stdist/(stdist - endist);
                  if (fract < enfract) enfract = fract;
            }
            
      }
            
      if (stfract <= enfract)
      {
            if ((stfract > -1.0f) && (stfract < gcmoutputfraction))
            {
                  if (stfract < 0.0f) stfract = 0.0f;
                  gcmoutputfraction = stfract;      
            }      
      }
}

bool CCollisionManager::getCollisionMovement(const CVector3 &startposition, const CVector3 &movement,
      CVector3 ellipseradius, CBSPSceneNode* selector, CVector3 &outCollisionMovement,
      CVector3 &outNewMovement, CVector3 &collRelPosition, CVector3 &tv, CVector3 &uv, float &ofrac, 
      float &nd)
{
      gcemesh = selector->getMesh();
      gcestartposition = startposition;
      gcemovvector = movement;
      float offsetlength = gcemovvector.Length();
      
      matrix4 gcecmat = selector->getAbsoluteTransformation();
      matrix4 cmatinv; 
      gcecmat.getInverse(cmatinv);
      
      CVector3 ostartposition = startposition;
      CVector3 endposition = startposition + movement;
      cmatinv.transformVector3(ostartposition); 
      cmatinv.transformVector3(endposition); 
      
      gceradius = ellipseradius;
      
      gcemaxradius = gceradius.x;
      if (gceradius.y > gcemaxradius) gcemaxradius = gceradius.y;
      if (gceradius.z > gcemaxradius) gcemaxradius = gceradius.z;
      
      float minradius = gceradius.x;
      if (gceradius.y < minradius) minradius = gceradius.y;
      if (gceradius.z < minradius) minradius = gceradius.z;
      gceavgradius = (gcemaxradius + minradius)/2;
      
      gcescalex = gceavgradius/gceradius.x;
      gcescaley = gceavgradius/gceradius.y;
      gcescalez = gceavgradius/gceradius.z;
      
      gcestartposition.x = gcescalex*gcestartposition.x;
      gcestartposition.y = gcescaley*gcestartposition.y;
      gcestartposition.z = gcescalez*gcestartposition.z;
      
      gcemovvector.x = gcescalex*gcemovvector.x;
      gcemovvector.y = gcescaley*gcemovvector.y;
      gcemovvector.z = gcescalez*gcemovvector.z;
      
      gceoutputfraction = 2.0f;
      
      CheckNodeCollMovE(0, 0.0f, 1.0f, ostartposition, endposition);
      
      if (gceoutputfraction > 1.0f)
            return FALSE;
      
      float offsetfraction = gceoutputfraction - EPSILON/offsetlength;
      //if (offsetfraction < 0.0f) offsetfraction = 0.0f;  
      
      CVector3 ncolmovement = gcemovvector*offsetfraction; 
      
      ncolmovement.x = ncolmovement.x/gcescalex;
      ncolmovement.y = ncolmovement.y/gcescaley;
      ncolmovement.z = ncolmovement.z/gcescalez;
      
      outCollisionMovement = ncolmovement;
      
      CVector3 slidepnorm = gcestartposition - gcecollpoint;
      slidepnorm.Normalize();
      
      CVector3 rmmovvector = gcemovvector*(1.0f - gceoutputfraction); 
      float newdist = rmmovvector.DotProduct(slidepnorm);
      CVector3 newmovvector = rmmovvector - slidepnorm*newdist;
      
      newmovvector.x = newmovvector.x/gcescalex;
      newmovvector.y = newmovvector.y/gcescaley;
      newmovvector.z = newmovvector.z/gcescalez;
      
      outNewMovement = newmovvector;
      ofrac = gceoutputfraction; 
      
      collRelPosition = gcecollpoint - gcestartposition;
      collRelPosition.x = collRelPosition.x/gcescalex;
      collRelPosition.y = collRelPosition.y/gcescaley;
      collRelPosition.z = collRelPosition.z/gcescalez;
      
      return TRUE;     
}

void CCollisionManager::CheckNodeCollMovE(int nodeIndex, float startfraction, float endfraction, 
      CVector3 startfractpoint, CVector3 endfractpoint)
{     
      tBSPLeaf *leaf;
      tBSPFace *face;
      int fcidx, cidx, ptidx;
      tBSPVertex *pv;
      CVector3 pa, pb, pc;
      CBSPBezier *ptc;
      int numvts;
      
      if (nodeIndex < 0)
      {     
            leaf = &(gcemesh->Leafs[-(nodeIndex + 1)]);
            
            for (int i = 0; i < leaf->numOfLeafFaces; i++)
            {    
                  fcidx = gcemesh->LeafFaces[leaf->leafface + i];    
                  face = &(gcemesh->Faces[fcidx]);
                  
                  if (face->type == 1)
                        for (int j = 0; j < face->numMeshVerts; j+=3)
                        {
                               cidx = gcemesh->MeshVerts[face->meshVertIndex + j] + face->vertexIndex;
                               pv = &(gcemesh->Vertices[cidx]);
                               pa.set(pv->vPosition[0], pv->vPosition[1], pv->vPosition[2]);
                               gcecmat.transformVector3(pa);
                               pa.x = pa.x*gcescalex;
                               pa.y = pa.y*gcescaley;
                               pa.z = pa.z*gcescalez;
                               
                               cidx = gcemesh->MeshVerts[face->meshVertIndex + j + 1] + face->vertexIndex;
                               pv = &(gcemesh->Vertices[cidx]);
                               pb.set(pv->vPosition[0], pv->vPosition[1], pv->vPosition[2]);
                               gcecmat.transformVector3(pb);
                               pb.x = pb.x*gcescalex;
                               pb.y = pb.y*gcescaley;
                               pb.z = pb.z*gcescalez;
                               
                               cidx = gcemesh->MeshVerts[face->meshVertIndex + j + 2] + face->vertexIndex;
                               pv = &(gcemesh->Vertices[cidx]);
                               pc.set(pv->vPosition[0], pv->vPosition[1], pv->vPosition[2]);
                               gcecmat.transformVector3(pc);
                               pc.x = pc.x*gcescalex;
                               pc.y = pc.y*gcescaley;
                               pc.z = pc.z*gcescalez;
                               
                               CheckTriangleCollMovE(pa, pc, pb); // BSP meshverts use clockwise order
                        }
                  else if (face->type == 2)
                        for (int j = 0; j < gcemesh->FaceNumPatches[fcidx]; j++)
                        {
                               ptidx = gcemesh->FaceFirstPatch[fcidx] + j;
                               ptc = gcemesh->Beziers[ptidx];
                               
                               numvts = ptc->level*ptc->level*6; 
                               
                               for (int k = 0; k < numvts; k+=3)
                               {
                                     cidx = ptc->indices[k];
                                     pv = &(ptc->vertices[cidx]);
                                     pa.set(pv->vPosition[0], pv->vPosition[1], pv->vPosition[2]);
                                     gcecmat.transformVector3(pa);
                                     pa.x = pa.x*gcescalex;
                                     pa.y = pa.y*gcescaley;
                                     pa.z = pa.z*gcescalez;
                               
                                     cidx = ptc->indices[k + 1];
                                     pv = &(ptc->vertices[cidx]);
                                     pb.set(pv->vPosition[0], pv->vPosition[1], pv->vPosition[2]);
                                     gcecmat.transformVector3(pb);
                                     pb.x = pb.x*gcescalex;
                                     pb.y = pb.y*gcescaley;
                                     pb.z = pb.z*gcescalez;
                               
                                     cidx = ptc->indices[k + 2];
                                     pv = &(ptc->vertices[cidx]);
                                     pc.set(pv->vPosition[0], pv->vPosition[1], pv->vPosition[2]);
                                     gcecmat.transformVector3(pc);
                                     pc.x = pc.x*gcescalex;
                                     pc.y = pc.y*gcescaley;
                                     pc.z = pc.z*gcescalez;
                               
                                     CheckTriangleCollMovE(pa, pc, pb); // BSP meshverts use clockwise order
                               }   
                        }    
            }
            
            return;        
      }
      
      CVector3 planenormal;
      float fraction1, fraction2, invfrac;
      int side;
      float middlefraction;
      CVector3 middlefractpoint;
      
      tBSPNode *node = &(gcemesh->Nodes[nodeIndex]);
      tBSPPlane *plane = &(gcemesh->Planes[node->plane]);
      planenormal.set(plane->vNormal[0], plane->vNormal[1], plane->vNormal[2]);
      planenormal.Normalize();
      
      float startdistance = startfractpoint.DotProduct(planenormal) - plane->d;
      float enddistance = endfractpoint.DotProduct(planenormal) - plane->d;
      
      if ((startdistance > gcemaxradius) && (enddistance > gcemaxradius))
            CheckNodeCollMovE(node->front, startfraction, endfraction, startfractpoint, endfractpoint);             
      else if ((startdistance < -gcemaxradius) && (enddistance < -gcemaxradius))
            CheckNodeCollMovE(node->back, startfraction, endfraction, startfractpoint, endfractpoint);
      else
      {
            if (startdistance > enddistance)
            {
                  invfrac = 1.0f/(startdistance - enddistance);            
                  fraction1 = (startdistance + gcemaxradius + EPSILON)*invfrac;
                  fraction2 = (startdistance - gcemaxradius - EPSILON)*invfrac;
                  side = 0;           
            }
            else if (startdistance < enddistance)
            {
                  invfrac = 1.0f/(startdistance - enddistance);
                  fraction1 = (startdistance - gcemaxradius - EPSILON)*invfrac;
                  fraction2 = (startdistance + gcemaxradius + EPSILON)*invfrac;
                  side = 1;
            }
            else
            {
                  fraction1 = 0.0f;
                  fraction2 = 0.0f;
                  side = 0;
            }
            
            if (fraction1 < 0.0f) fraction1 = 0.0f;
            else if (fraction1 > 1.0f) fraction1 = 1.0f;
            
            if (fraction2 < 0.0f) fraction2 = 0.0f;
            else if (fraction2 > 1.0f) fraction2 = 1.0f;
            
            middlefraction = startfraction + (endfraction - startfraction)*fraction1;
            middlefractpoint = startfractpoint + (endfractpoint - startfractpoint)*fraction1;
            
            CheckNodeCollMovE((side == 0)?node->front:node->back, startfraction, middlefraction, 
                  startfractpoint, middlefractpoint);
            
            middlefraction = startfraction + (endfraction - startfraction)*fraction2;
            middlefractpoint = startfractpoint + (endfractpoint - startfractpoint)*fraction2;
            
            CheckNodeCollMovE((side == 0)?node->back:node->front, middlefraction, endfraction, 
                  middlefractpoint, endfractpoint);
      }
}

void CCollisionManager::CheckTriangleCollMovE(const CVector3 &pa, const CVector3 &pb, const CVector3 &pc)
{     
      CVector3 ab = pb - pa;
      CVector3 ac = pc - pa;
      CVector3 tnorm = ab.CrossProduct(ac);   
      tnorm.Normalize();
      float td = pa.DotProduct(tnorm);
      
      CVector3 ipstart = gcestartposition - tnorm*gceavgradius;
      CVector3 ipend = ipstart + gcemovvector;
      
      float stdist = ipstart.DotProduct(tnorm) - td;
      float endist = ipend.DotProduct(tnorm) - td;
      float ordist = gcestartposition.DotProduct(tnorm) - td;
      
      if (endist > stdist) return;
      if (ordist <= 0) return;
      if ((stdist > 0) && (endist > 0)) return;
                 
      CVector3 plipoint;
      CVector3 pgonpoint;
      float fracifinside;

      if (ordist <= gceavgradius)
      {     
            plipoint = gcestartposition - tnorm*ordist;
                 
            if (IsInsideTriangle(plipoint, pa, pb, pc))
            {
                  if (endist == stdist)
                        return;
                  
                  if (gceoutputfraction > 0.0f) 
                  {
                        gceoutputfraction = 0.0f;
                        gcecollpoint = plipoint;                                  
                  }
                  
                  return;
            }            
            else 
            {     
                  pgonpoint = ClosestPointOnTriangle(plipoint, pa, pb, pc);
                  
                  if ((pgonpoint - gcestartposition).Length() <= gceavgradius)
                  {
                        if (endist == stdist)
                               return;
                                  
                        if (gceoutputfraction > 0.0f)
                        {
                               gceoutputfraction = 0.0f;
                               gcecollpoint = pgonpoint;               
                        }
                                 
                        return;         
                  } 
            }                     
      }
      else
      {
            fracifinside = stdist/(stdist - endist);
            plipoint = ipstart + gcemovvector*fracifinside;
            
            if (IsInsideTriangle(plipoint, pa, pb, pc))
            {                            
                  if (fracifinside < gceoutputfraction)
                  {
                        gceoutputfraction = fracifinside;
                        gcecollpoint = ipstart;
                  }
            
                  return;          
            }
      }
      
      int numoutfrac = 0;
      float outfrac[6];
      CVector3 clpoint[6];
      
      float fr = gceavgradius*gceavgradius;
      CVector3 dsp = gcestartposition - pa;
      float fa = gcemovvector.DotProduct(gcemovvector);
      float fb = dsp.DotProduct(gcemovvector)*2.0f;
      float fc = dsp.DotProduct(dsp) - fr;
      
      float root = GetSmallerRoot(fa, fb, fc);
      if ((root >= 0.0f) && (root <= 1.0f))
      {
            numoutfrac++;
            outfrac[numoutfrac - 1] = root;
            clpoint[numoutfrac - 1] = pa;   
      }
      
      dsp = gcestartposition - pb;
      fa = gcemovvector.DotProduct(gcemovvector);
      fb = dsp.DotProduct(gcemovvector)*2.0f;
      fc = dsp.DotProduct(dsp) - fr;
      
      root = GetSmallerRoot(fa, fb, fc);
      if ((root >= 0.0f) && (root <= 1.0f))
      {
            numoutfrac++;
            outfrac[numoutfrac - 1] = root;
            clpoint[numoutfrac - 1] = pb;   
      }
      
      dsp = gcestartposition - pc;
      fa = gcemovvector.DotProduct(gcemovvector);
      fb = dsp.DotProduct(gcemovvector)*2.0f;
      fc = dsp.DotProduct(dsp) - fr;
      
      root = GetSmallerRoot(fa, fb, fc);
      if ((root >= 0.0f) && (root <= 1.0f))
      {
            numoutfrac++;
            outfrac[numoutfrac - 1] = root;
            clpoint[numoutfrac - 1] = pc;   
      }
      
      CVector3 pp;
      float f0;
      
      CVector3 pe = pb - pa;
      pe.Normalize();
      CVector3 pv = pa - gcestartposition;
      dsp = pv - pe*(pe.DotProduct(pv));
      CVector3 pm = pe*(pe.DotProduct(gcemovvector)) - gcemovvector;

      fa = pm.DotProduct(pm);
      fb = dsp.DotProduct(pm)*2.0f;
      fc = dsp.DotProduct(dsp) - fr;
      
      root = GetSmallerRoot(fa, fb, fc);
      if ((root >= 0.0f) && (root <= 1.0f))
      {
            pp = gcestartposition + gcemovvector*root;    
            f0 = (pp - pa).DotProduct(pe)/(pb - pa).DotProduct(pe);
               
            if ((f0 >= 0.0f) && (f0 <= 1.0f))
            {   
                  numoutfrac++;
                  outfrac[numoutfrac - 1] = root;
                  clpoint[numoutfrac - 1] = pa + (pb - pa)*f0;
            }   
      }
      
      pe = pc - pb;
      pe.Normalize();
      pv = pb - gcestartposition;
      dsp = pv - pe*(pe.DotProduct(pv));
      pm = pe*(pe.DotProduct(gcemovvector)) - gcemovvector;

      fa = pm.DotProduct(pm);
      fb = dsp.DotProduct(pm)*2.0f;
      fc = dsp.DotProduct(dsp) - fr;
      
      root = GetSmallerRoot(fa, fb, fc);
      if ((root >= 0.0f) && (root <= 1.0f))
      {
            pp = gcestartposition + gcemovvector*root;    
            f0 = (pp - pb).DotProduct(pe)/(pc - pb).DotProduct(pe);
               
            if ((f0 >= 0.0f) && (f0 <= 1.0f))
            {   
                  numoutfrac++;
                  outfrac[numoutfrac - 1] = root;
                  clpoint[numoutfrac - 1] = pb + (pc - pb)*f0;
            }   
      }
      
      pe = pa - pc;
      pe.Normalize();
      pv = pc - gcestartposition;
      dsp = pv - pe*(pe.DotProduct(pv));
      pm = pe*(pe.DotProduct(gcemovvector)) - gcemovvector;

      fa = pm.DotProduct(pm);
      fb = dsp.DotProduct(pm)*2.0f;
      fc = dsp.DotProduct(dsp) - fr;
      
      root = GetSmallerRoot(fa, fb, fc);
      if ((root >= 0.0f) && (root <= 1.0f))
      {
            pp = gcestartposition + gcemovvector*root;    
            f0 = (pp - pc).DotProduct(pe)/(pa - pc).DotProduct(pe);
               
            if ((f0 >= 0.0f) && (f0 <= 1.0f))
            {   
                  numoutfrac++;
                  outfrac[numoutfrac - 1] = root;
                  clpoint[numoutfrac - 1] = pc + (pa - pc)*f0;
            }   
      }
      
      if (numoutfrac < 1)
            return;
      
      float minfrac = outfrac[0];
      int minfracidx = 0;
      
      for (int i = 1; i < numoutfrac; i++)
            if (outfrac[i] < minfrac)
            {
                  minfrac = outfrac[i];         
                  minfracidx = i;         
            }
      
      if (minfrac < gceoutputfraction)
      {
            gceoutputfraction = minfrac;
            gcecollpoint = clpoint[minfracidx] - gcemovvector*minfrac;      
      }     
}

bool CCollisionManager::IsInsideTriangle(const CVector3 &pp, const CVector3 &pa, const CVector3 &pb, 
      const CVector3 &pc)
{     
      CVector3 ab = pb - pa;
      CVector3 ac = pc - pa;
      CVector3 poa = pa - pp;
      CVector3 pob = pb - pp;
      CVector3 poc = pc - pp;
      
      CVector3 tnorm = ab.CrossProduct(ac);
      
      float tpab = (poa.CrossProduct(pob)).DotProduct(tnorm);
      if (tpab < 0) return FALSE;
      
      float tpbc = (pob.CrossProduct(poc)).DotProduct(tnorm);
      if (tpbc < 0) return FALSE;
      
      float tpca = (poc.CrossProduct(poa)).DotProduct(tnorm);
      if (tpca < 0) return FALSE;
      
      return TRUE;
}

CVector3 CCollisionManager::ClosestPointOnTriangle(const CVector3 &pp, const CVector3 &pa, const CVector3 &pb, 
      const CVector3 &pc)
{
      CVector3 cpab = ClosestPointOnLine(pp, pa, pb);
      CVector3 cpbc = ClosestPointOnLine(pp, pb, pc);
      CVector3 cpca = ClosestPointOnLine(pp, pc, pa);
      
      float dab = (cpab - pp).Length();
      float dbc = (cpbc - pp).Length();
      float dca = (cpca - pp).Length();
      
      float lmin = dab;
      int minidx = 0;
      
      if (dbc < lmin)
      {
            lmin = dbc;
            minidx = 1;  
      }
      
      if (dca < lmin)
      {
            lmin = dca;
            minidx = 2;  
      }
      
      if (minidx == 0)
            return cpab;
      else if (minidx == 1)
            return cpbc;
      else
            return cpca;
}

CVector3 CCollisionManager::ClosestPointOnLine(const CVector3 &pp, const CVector3 &pa, const CVector3 &pb)
{
      CVector3 vap = pp - pa;
      
      CVector3 vab = pb - pa;
      float dab = vab.Length();
      vab.Normalize();
      
      float t = vap.DotProduct(vab);
      
      if (t < 0) return pa;
      if (t > dab) return pb;
      
      vab = vab*t;
      CVector3 vres = pa + vab;
      
      return vres;
}

float CCollisionManager::GetSmallerRoot(float a, float b, float c)
{
      float D = b*b - 4*a*c;
      float rD;
      
      if (D >= 0.0f)
      {
            rD = sqrt(D);
            
            if (b <= 0.0f)
            {
                  if (-b < rD) return -1.0f;
                  return (-b - rD)/(2.0f*a);
            }
            else
                  return -1.0f;
      }
      else
            return -1.0f; 
}
