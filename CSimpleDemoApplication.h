#ifndef __CSIMPLEDEMOAPPLICATION_H__
#define __CSIMPLEDEMOAPPLICATION_H__

#include "CMeDevice.h"
#include "CDummyDevice.h"
#include "CEnemy.h"
#include "CBullet.h"
#include "CBSPSceneNode.h"
#include "CCollisionResponseAnimator.h"
#include "CBmp24Image.h"
#include "CEventProcessor.h"
#include <stdio.h>

#define MAXBULLETDISTANCE 4000.0f
#define BULLETSPEED 600.0f
#define BULLETBOUNDINGRADIUS CVector3(10.0f, 10.0f, 10.0f)

#define TRISBOUNDINGRADIUS CVector3(20.0f, 45.0f, 20.0f)
#define TRISVISIONDISTANCE 300.0f
#define TRISSPEED 220.0f
#define TRISBULLETSPEED 280.0f
#define TRISNUMBER 12

class CSimpleDemoApplication: public CEventProcessor
{
protected:
      CMeDevice *DEVICE;
      CDummyDevice *DUMMY[3];
      CSceneManager *SCENEMANAGER;
      CLevel *ACTIVELEVEL;
      CSceneNode *ROOTSCENENODE;
      CCameraSceneNode *ACTIVECAMERA;
      CCameraSceneNode *CAMERA_1;
      CCameraSceneNode *CAMERA_2;
      CCameraSceneNode *CAMERA_3;

      CBSPMesh *BUILDINGMESH;
      CMD2Mesh *BULLETMESH;
      CMD2Mesh *TRISMESH;
      CBmp24Image *EXPLOSIONBITMAP;

      CBSPSceneNode *BUILDINGNODE;
      CVector3 BUILDINGCENTER;
      CMD2SceneNode *TRISNODE;

      char CAPTION[40];
      int CAPTIONFRAMES;

      bool NEEDMOREENEMY;
      void CreateEnemy();

public:
      CSimpleDemoApplication() {}
      virtual ~CSimpleDemoApplication() {}

      virtual bool ProcessEvent(CHolderDevice *dev, UINT umsg, WPARAM wparam, LPARAM lparam);

      void initialize();
      void finalize();
      void run();

};

#endif
