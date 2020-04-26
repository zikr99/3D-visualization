#ifndef __CMD2SCENENODE_H__
#define __CMD2SCENENODE_H__

#include "CSceneNode.h"
#include "CMD2Mesh.h"

enum E_MD2_STATE
{
      EMD2S_IDLE_1,
      EMD2S_RUNNING,
      EMD2S_SHOT_NOT_FALLING_DOWN,
      EMD2S_SHOT_IN_SHOULDER,
      EMD2S_JUMP,
      EMD2S_IDLE_2,
      EMD2S_SHOT_FALLING_DOWN,
      EMD2S_IDLE_3,
      EMD2S_IDLE_4,
      EMD2S_CROUCHING,
      EMD2S_CROUCHING_CRAWL,
      EMD2S_IDLE_CROUCHING,
      EMD2S_KNEELING_DYING,
      EMD2S_FALLING_BACK_DYING,
      EMD2S_FALLING_FORWARD_DYING,
      EMD2S_FALLING_BACK_SLOWLY_DYING,
      EMD2S_UNDEFINED
};

class CMD2SceneNode: public CSceneNode
{
protected:
      aabbox BBox;   
      CMD2Mesh *m_mesh;
      int m_numframes;
      
      int m_framestart;
      int m_frameend;
      int m_speed;
      bool m_loop;
      
      int m_currentframe;
      int m_nextframe;
      float m_interpol;
      
      int m_lastcurrentframe;
      int m_lastnextframe;
      float m_lastinterpol;
      
      virtual const aabbox& getBoundingBox() const { return BBox; }
      virtual void CalculateBoundingBox();
      virtual void CalculateNormal(float *p1, float *p2, float *p3);
      
      virtual void OnUpdate(unsigned int timeMs);   
      virtual void OnPrepareRender();    
      virtual void OnRender();  
          
public:
      CMD2SceneNode(CSceneNode *parent, CSceneManager* mngr, CMD2Mesh *mesh = NULL, 
            const CVector3& position = CVector3(0.0f, 0.0f, 0.0f),
            const CVector3& rotation = CVector3(0.0f, 0.0f, 0.0f),
		    const CVector3& scale = CVector3(1.0f, 1.0f, 1.0f)): 
            CSceneNode(parent, mngr, position, rotation, scale), m_mesh(NULL),
            m_numframes(0), m_framestart(0), m_frameend(0), m_speed(2000), m_loop(true),
            m_currentframe(0), m_nextframe(1), m_interpol(0.0f)
      {
            BBox.reset(0.0f, 0.0f, 0.0f);
            setState(0, 0);
            
            if (mesh) setMesh(mesh);
      }
      
      virtual ~CMD2SceneNode() {} 
      
      virtual void setMesh(CMD2Mesh *mesh);
      virtual void setState(int framestart, int frameend, bool loop = true, int speed = 0);
      virtual void setState(E_MD2_STATE state, bool loop = true, int speed = 0);
      virtual void setFrame(int framenum = 0);
      virtual void setSpeed(int speed);
      
      virtual int getNumFrames() { return m_numframes; }
      
};

#endif
