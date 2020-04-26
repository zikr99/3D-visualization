#ifndef __CBSPSCENENODE_H__
#define __CBSPSCENENODE_H__

#include "CSceneNode.h"
#include "CBSPMesh.h"

class CBSPSceneNode: public CSceneNode
{
protected:
      aabbox BBox;    
      CBSPMesh *m_mesh;
      array<array<int>*> MatGroups;
      array<array<int>*> MatGroupsBezier;
      
      int m_camcluster;
      aabbox m_cambbox;
      array<int> UnCulledLeafs;
      bool *FacesFlag; 
      
      int FindCamCluster();
      bool IsVisibleCluster(int testcluster);
      bool IntersectsWithViewFrustrum(tBSPLeaf *leaf);
      void AddUnCulledLeafs();
              
      virtual const aabbox& getBoundingBox() const { return BBox; }    
      virtual void OnPrepareRender();    
      virtual void OnRender();
      
      virtual void drawFace(int i);
      virtual void drawBezier(int i);
      virtual void drawLevel();
                
public:
      CBSPSceneNode(CSceneNode *parent, CSceneManager* mngr, CBSPMesh *mesh = NULL, 
            const CVector3& position = CVector3(0.0f, 0.0f, 0.0f),
            const CVector3& rotation = CVector3(0.0f, 0.0f, 0.0f),
		    const CVector3& scale = CVector3(1.0f, 1.0f, 1.0f)): 
            CSceneNode(parent, mngr, position, rotation, scale), m_mesh(NULL)
      {
            BBox.reset(0.0f, 0.0f, 0.0f);
            
            if (mesh) setMesh(mesh);
      }
      	  
      virtual ~CBSPSceneNode() 
      {
            for (int i = 0; i < MatGroups.size(); i++) delete MatGroups[i];  
            MatGroups.clear();  
            
            for (int i = 0; i < MatGroupsBezier.size(); i++) delete MatGroupsBezier[i];
            MatGroupsBezier.clear();
      }
      
      void setMesh(CBSPMesh *mesh);
      CBSPMesh* getMesh() { return m_mesh; }
       
};

#endif
