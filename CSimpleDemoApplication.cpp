#include "CSimpleDemoApplication.h"

void CSimpleDemoApplication::CreateEnemy()
{
      CVector3 Mxe = BUILDINGNODE->getTransformedBoundingBox().MaxEdge;

      float xrpos = (Mxe.x - BUILDINGCENTER.x)/2.0f*float((rand() % 100) - 50)/float(50);
      float zrpos = (Mxe.z - BUILDINGCENTER.z)/2.0f*float((rand() % 100) - 50)/float(50);

      CVector3 cpos(BUILDINGCENTER.x, 0.0f, BUILDINGCENTER.z);
      CVector3 tpos = cpos + CVector3(xrpos, Mxe.y - 100.0f, zrpos);

      TRISNODE = new CEnemy(ROOTSCENENODE, SCENEMANAGER, TRISMESH, BUILDINGNODE,
            TRISBOUNDINGRADIUS, TRISVISIONDISTANCE, TRISSPEED, BULLETMESH,
            TRISBULLETSPEED);
      TRISNODE->setScale(CVector3(2.0f, 2.0f, 2.0f));
      TRISNODE->setPosition(tpos);
}

bool CSimpleDemoApplication::ProcessEvent(CHolderDevice *dev, UINT umsg, WPARAM wparam, LPARAM lparam)
{
      if (umsg == WM_LBUTTONUP)
      {
            CBullet *bullet = new CBullet((CSceneNode*)ACTIVECAMERA->parentNode, SCENEMANAGER,
                 ACTIVECAMERA, BULLETMESH, ACTIVECAMERA->getPosition(), ACTIVECAMERA->getLookDirection(),
                 MAXBULLETDISTANCE, BULLETSPEED, BULLETBOUNDINGRADIUS, ROOTSCENENODE,
                 CVector3(0.2f, 0.2f, 0.2f));

            return TRUE;
      }
      else if (umsg == WM_KEYDOWN)
      {
            switch (wparam)
            {
                case 0x41:
                    ((CCameraFPSSceneNode*)ACTIVECAMERA)->setActive(false);
                    break;
                case 0x44:
                    ((CCameraFPSSceneNode*)ACTIVECAMERA)->setActive(true);
                    break;
            }
      }

      return FALSE;
}

void CSimpleDemoApplication::initialize()
{
      srand(time(NULL));

      DEVICE = new CMeDevice("Trial", 800, 600, 32, FALSE, this);
      SCENEMANAGER = DEVICE->getSceneManager();
      ACTIVELEVEL = SCENEMANAGER->getActiveLevel();
      ROOTSCENENODE = ACTIVELEVEL->getRootSceneNode();
      ACTIVECAMERA = ACTIVELEVEL->getActiveCamera();

      for (int i = 0; i < 3; i++)
      {
          char s[100];

          sprintf(s, "Camera-%d", i + 2);
          DUMMY[i] = new CDummyDevice(s, 800, 600, 32, FALSE, DEVICE, SCENEMANAGER, NULL);
      }

      BUILDINGMESH = new CBSPMesh(SCENEMANAGER, "maps\\szq2dm1ish.bsp"); //"maps\\20kdm2.bsp");
      BULLETMESH = new CMD2Mesh(SCENEMANAGER, "maps\\rocket.md2", "maps\\rocket.bmp");
      TRISMESH = new CMD2Mesh(SCENEMANAGER, "maps\\tris.md2", "maps\\tris.bmp");
      EXPLOSIONBITMAP = new CBmp24Image("textures\\explosion.bmp");

      BUILDINGNODE = new CBSPSceneNode(NULL, SCENEMANAGER, BUILDINGMESH);
      ROOTSCENENODE->attach(BUILDINGNODE);
      BUILDINGCENTER = BUILDINGNODE->getTransformedBoundingBox().getCenter();

      ((CCameraFPSSceneNode*)ACTIVECAMERA)->setReInit(false);
      ((CCameraFPSSceneNode*)ACTIVECAMERA)->setNoVerticalMovement(false);
      ((CCameraFPSSceneNode*)ACTIVECAMERA)->setNoVerticalRotation(true);

      ACTIVECAMERA->setPosition(BUILDINGCENTER + CVector3(0.0f, 0.0f, 60.0f));
      CAnimator *cameraresponse = new CCollisionResponseAnimator(ACTIVECAMERA, BUILDINGNODE,
            CVector3(20.0f, 45.0f, 20.0f), CVector3(0.0f, -1.0f, 0.0f), CVector3(0.0f, 25.0f, 0.0f),
            true, false);

      CAMERA_1 = new CCameraSceneNode(ACTIVECAMERA, SCENEMANAGER, CVector3(-100.0f, 0.0f, 0.0f));
      CAMERA_2 = new CCameraSceneNode(ACTIVECAMERA, SCENEMANAGER, CVector3(100.0f, 0.0f, 0.0f));
      CAMERA_3 = new CCameraSceneNode(ACTIVECAMERA, SCENEMANAGER, CVector3(0.0f, 0.0f, 100.0f));

      CAPTIONFRAMES = 0;
      NEEDMOREENEMY = false;
}

void CSimpleDemoApplication::finalize()
{
      delete DEVICE;

      for (int i = 0; i < 3; i++)
          delete DUMMY[i];

      delete EXPLOSIONBITMAP;
}

void CSimpleDemoApplication::run()
{
      FILE *fp;
      fp = fopen("e:\\FPS.txt", "w");


      int left, top;

      int width = DEVICE->getWidth()/2;
      int height = DEVICE->getHeight()/2;

      while (DEVICE->run())
      {
            CVector3 upcmr = ACTIVECAMERA->getUpVector();
            CVector3 dircmr = ACTIVECAMERA->getLookDirection();

            CVector3 lfcmr = upcmr.CrossProduct(dircmr);
            CVector3 rgcmr = -lfcmr;

            DEVICE->beginScene();
            SCENEMANAGER->drawAll();
            DEVICE->endScene();

            DUMMY[0]->beginScene();
            CAMERA_1->setLookDirection(lfcmr);
            ACTIVELEVEL->setActiveCamera(CAMERA_1);
            SCENEMANAGER->drawAll();
            DUMMY[0]->endScene();

            DUMMY[1]->beginScene();
            CAMERA_2->setLookDirection(rgcmr);
            ACTIVELEVEL->setActiveCamera(CAMERA_2);
            SCENEMANAGER->drawAll();
            DUMMY[1]->endScene();

            DUMMY[2]->beginScene();
            CAMERA_3->setLookDirection(-dircmr);
            ACTIVELEVEL->setActiveCamera(CAMERA_3);
            SCENEMANAGER->drawAll();
            DUMMY[2]->endScene();

            ACTIVELEVEL->setActiveCamera(ACTIVECAMERA);

            CAPTIONFRAMES++;
            if (CAPTIONFRAMES > 100)
            {
                 sprintf(CAPTION, "(%d, %d) Life Enemies : %d, Dead by Bullet : %d, Dead by Fault : %d (%0.1f FPS)",
                      DEVICE->getWidth(), DEVICE->getHeight(), CEnemy::Number, CEnemy::DeadByBullet, CEnemy::DeadByFault,
                      DEVICE->getTimer()->GetFPS(100));
                 DEVICE->setCaption(CAPTION);

                 char s[100];

                 for (int i = 0; i < 3; i++)
                 {
                    sprintf(s, "(%d, %d)", DUMMY[i]->getWidth(), DUMMY[i]->getHeight());
                    DUMMY[i]->setCaption(s);
                 }

                 CAPTIONFRAMES = 0;
            }

            if (CEnemy::Number < TRISNUMBER/4) NEEDMOREENEMY = true;
            if (CEnemy::Number >= TRISNUMBER) NEEDMOREENEMY = false;
            if (NEEDMOREENEMY == true) CreateEnemy();

            fprintf(fp, "%f, %f\n", DEVICE->getTimer()->GetElapsedSeconds(), DEVICE->getTimer()->GetFPS());
      }


      fclose(fp);
}
