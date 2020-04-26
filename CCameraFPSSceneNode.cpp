#include "CCameraFPSSceneNode.h"
#include "CSceneManager.h"

CCameraFPSSceneNode::CCameraFPSSceneNode(CSceneNode *parent, CSceneManager *mngr,
     CCursorControl *cursorcontrol, float movespeed, float rotatespeed,
     bool novertmovement, bool novertrotation, const CVector3& lookdir, const CVector3& position, bool rinit):
     CCameraSceneNode(parent, mngr, lookdir, position), m_cursorcontrol(cursorcontrol),
     m_movespeed(movespeed), m_rotatespeed(rotatespeed), m_noverticalmovement(novertmovement),
     m_noverticalrotation(novertrotation), ReInit(rinit), FirstUpdate(true), m_active(true)
{
     m_movespeed /=  1000.0f;

     AllKeysUp();
     CreateKeyMap();

     BBox.reset(1.0f, 1.0f, 1.0f);
     BBox.addInternalPoint(-1.0f, -1.0f, -1.0f);
}

void CCameraFPSSceneNode::AllKeysUp()
{
     for (int i = 0; i < ECA_COUNT; i++)
          MoveDirKeys[i] = FALSE;
}

void CCameraFPSSceneNode::CreateKeyMap()
{
     KeyMap.push_back(SCameraKeyMap(ECA_MOVE_FORWARD, VK_UP));
     KeyMap.push_back(SCameraKeyMap(ECA_MOVE_BACKWARD, VK_DOWN));
     KeyMap.push_back(SCameraKeyMap(ECA_STRAFE_LEFT, VK_LEFT));
     KeyMap.push_back(SCameraKeyMap(ECA_STRAFE_RIGHT, VK_RIGHT));
}

void CCameraFPSSceneNode::OnUpdate(unsigned int timeMs)
{
     Animate(timeMs);
}

void CCameraFPSSceneNode::Animate(unsigned int timeMs)
{
     if (!isActive())
        return;

     if (m_scenemanager->getActiveLevel()->getActiveCamera() != this)
     {
          if (ReInit)
               FirstUpdate = TRUE;

          return;
     }

     if (FirstUpdate)
     {
          m_cursorcontrol->setPosition(0.5f, 0.5f);
          FirstUpdate = FALSE;

          return;
     }

     CVector2<float> cursorpos;
     float rotx = 0, roty = 0;
     CVector3 MTarget;
     matrix4 mat;
     CVector3 XDir, YDir, ZDir;

     m_cursorcontrol->getRelativePosition(cursorpos);

     if ((cursorpos.x > 0.503f) || (cursorpos.x < 0.497f) ||
         (cursorpos.y > 0.503f) || (cursorpos.y < 0.497f))
     {
          roty = (0.5f - cursorpos.x)*m_rotatespeed;
          rotx = (0.5f - cursorpos.y)*m_rotatespeed;

          if (rotx > MAX_VERTICAL_ANGLE) rotx = MAX_VERTICAL_ANGLE;
          if (rotx < -MAX_VERTICAL_ANGLE) rotx = -MAX_VERTICAL_ANGLE;

          if (m_noverticalrotation)
               rotx = 0.0f;

          m_cursorcontrol->setPosition(0.5f, 0.5f);
     }

     MTarget.set(0.0f, 0.0f, -1.0f);
     mat.setRotation(0.0f, DEG2RAD(rotx), DEG2RAD(roty));
     mat.transformVector3(MTarget);

     XDir = LookDirection.CrossProduct(UpVector);
     YDir = XDir.CrossProduct(LookDirection);
     ZDir = -LookDirection;
     XDir.Normalize();
     YDir.Normalize();
     ZDir.Normalize();

     LookDirection = XDir*MTarget.x+ YDir*MTarget.y + ZDir*MTarget.z;
     LookDirection.Normalize();

     CVector3 FwMove = LookDirection;
     CVector3 StMove = LookDirection.CrossProduct(UpVector);

     FwMove.Normalize();
     StMove.Normalize();

     if (m_noverticalmovement)
     {
          if (parentNode)
          {
               matrix4 pmat = ((CSceneNode*)parentNode)->getAbsoluteTransformation();
               matrix4 pmatinv;
               pmat.getInverse(pmatinv);

               CVector3 ROrg(0.0f, 0.0f, 0.0f);
               pmat.transformVector3(FwMove);
               pmat.transformVector3(StMove);
               pmat.transformVector3(ROrg);

               FwMove = FwMove - ROrg;
               StMove = StMove - ROrg;

               FwMove.y = 0;
               StMove.y = 0;

               CVector3 AOrg(0.0f, 0.0f, 0.0f);
               pmatinv.transformVector3(FwMove);
               pmatinv.transformVector3(StMove);
               pmatinv.transformVector3(AOrg);

               FwMove = FwMove - AOrg;
               StMove = StMove - AOrg;
          }
          else
          {
               FwMove.y = 0;
               StMove.y = 0;
          }
     }

     CVector3 crpos = getPosition();

     if (MoveDirKeys[ECA_MOVE_FORWARD]) crpos = crpos + FwMove*m_movespeed*timeMs;
     if (MoveDirKeys[ECA_MOVE_BACKWARD]) crpos = crpos - FwMove*m_movespeed*timeMs;
     if (MoveDirKeys[ECA_STRAFE_LEFT]) crpos = crpos - StMove*m_movespeed*timeMs;
     if (MoveDirKeys[ECA_STRAFE_RIGHT]) crpos = crpos + StMove*m_movespeed*timeMs;

     setPosition(crpos);
}

bool CCameraFPSSceneNode::ProcessEvent(UINT umsg, WPARAM wparam, LPARAM lparam)
{
	 int num = KeyMap.size();
	 unsigned int kc = (unsigned int)wparam;

	 if (umsg == WM_KEYDOWN)
	 {
	      for (int i = 0; i < num; i++)
	           if (KeyMap[i].keycode == kc)
               {
                    MoveDirKeys[KeyMap[i].action] = TRUE;
                    return TRUE;
               }
     }
     else if (umsg == WM_KEYUP)
     {
	      for (int i = 0; i < num; i++)
	           if (KeyMap[i].keycode == kc)
               {
                    MoveDirKeys[KeyMap[i].action] = FALSE;
                    return TRUE;
               }
     }

	 return FALSE;
}
