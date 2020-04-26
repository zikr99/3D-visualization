#include "CTestSceneNode.h"
#include "CSceneManager.h"

void CTestSceneNode::OnPrepareRender()
{
      m_scenemanager->registerNodeForRendering(this, ESNRP_SOLID);
}

void CTestSceneNode::OnRender()
{  
      glEnable(GL_POINT_SMOOTH);
      glPointSize(30);
      glBegin(GL_POINTS);
            glVertex3f(0.0f, 0.0f, 0.0f);
      glEnd();  
            
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      glDisable(GL_LIGHTING);
      glBegin(GL_QUADS);
            glColor3f(1.0f, 1.0f, 0.0f);
            glVertex3f(-0.5f, -0.5f, 0.5f);
            glVertex3f(0.5f, -0.5f, 0.5f);
            glVertex3f(0.5f, 0.5f, 0.5f);
            glVertex3f(-0.5f, 0.5f, 0.5f);
            glColor3f(0.0f, 1.0f, 1.0f);
            glVertex3f(0.5f, -0.5f, 0.5f);
            glVertex3f(0.5f, -0.5f, -0.5f);
            glVertex3f(0.5f, 0.5f, -0.5f);
            glVertex3f(0.5f, 0.5f, 0.5f);
            glColor3f(1.0f, 0.0f, 1.0f);
            glVertex3f(-0.5f, -0.5f, -0.5f);
            glVertex3f(-0.5f, 0.5f, -0.5f);
            glVertex3f(0.5f, 0.5f, -0.5f);
            glVertex3f(0.5f, -0.5f, -0.5f);
            glColor3f(0.0f, 1.0f, 1.0f);
            glVertex3f(-0.5f, -0.5f, -0.5f);
            glVertex3f(-0.5f, -0.5f, 0.5f);
            glVertex3f(-0.5f, 0.5f, 0.5f);
            glVertex3f(-0.5f, 0.5f, -0.5f);
      glEnd();
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
} 
