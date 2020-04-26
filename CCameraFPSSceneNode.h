#ifndef __CCAMERAFPSSCENENODE_H__
#define __CCAMERAFPSSCENENODE_H__

#include "CCameraSceneNode.h"
#include "CCursorControl.h"
#include "irrArray.h"

#define MAX_VERTICAL_ANGLE 89.0f

class CCameraFPSSceneNode: public CCameraSceneNode
{
protected:
     struct SCameraKeyMap
     {
          ECAMERA_ACTION action;
          unsigned int keycode;

          SCameraKeyMap() {};
          SCameraKeyMap(ECAMERA_ACTION act, unsigned int kc): action(act), keycode(kc) {}
     };

     CCursorControl *m_cursorcontrol;
     float m_movespeed;
     float m_rotatespeed;
     bool m_noverticalmovement;
     bool m_noverticalrotation;

     array<SCameraKeyMap> KeyMap;
     bool MoveDirKeys[ECA_COUNT];

     void AllKeysUp();
     void CreateKeyMap();

     void OnUpdate(unsigned int timeMs);
     void Animate(unsigned int timeMs);

     bool ReInit;
     bool FirstUpdate;

     bool m_active;

public:

     CCameraFPSSceneNode(CSceneNode *parent, CSceneManager *mngr,
           CCursorControl *cursorcontrol, float movespeed = 200.0f, float rotatespeed = 1000.0f,
           bool novertmovement = false, bool novertrotation = false,
           const CVector3& lookdir = CVector3(0.0f, 0.0f, -100.0f),
           const CVector3& position = CVector3(0.0f, 0.0f, 0.0f),
           bool rinit = true);

     virtual ~CCameraFPSSceneNode() {}
     virtual bool ProcessEvent(UINT umsg, WPARAM wparam, LPARAM lparam);

     void setReInit(bool rinit) { ReInit = rinit; }
     bool isReInit() { return ReInit; }

     void setNoVerticalMovement(bool novertmov) { m_noverticalmovement = novertmov; }
     bool isNoVerticalMovement() { return m_noverticalmovement; }

     void setNoVerticalRotation(bool novertrot) { m_noverticalrotation = novertrot; }
     bool isNoVerticalRotation() { return m_noverticalrotation; }

     void setActive(bool state) { m_active = state; }
     bool isActive() { return m_active; }

};

#endif
