#include "CCameraSceneNode.h"
#include "CSceneManager.h"

CCameraSceneNode::CCameraSceneNode(CSceneNode *parent,
    CSceneManager *mngr, const CVector3& lookdir, const CVector3& position):
    CSceneNode(parent, mngr, position)
{
    // set default projection
	fovy = PI/2.5f;
	aspect = 4.0f/3.0f;
	zn = 1.0f;
	zf = 3000.0f;
    recalculateProjection();

	// set default view
	LookDirection.set(lookdir);
	UpVector.set(0.0f, 1.0f, 0.0f);

	BBox.reset(0.0f, 0.0f, 0.0f);
}

void CCameraSceneNode::setFOV(float f)
{
	fovy = f;
	recalculateProjection();
}

void CCameraSceneNode::setAspectRatio(float f)
{
	aspect = f;
	recalculateProjection();
}

void CCameraSceneNode::setNearValue(float f)
{
	zn = f;
	recalculateProjection();
}

void CCameraSceneNode::setFarValue(float f)
{
	zf = f;
	recalculateProjection();
}

const matrix4& CCameraSceneNode::getViewMatrix()
{
    recalculateView();
    return View;
}

void CCameraSceneNode::render()
{
    GLfloat glmat[16];

    recalculateView();
    recalculateViewArea();

    if (m_scenemanager->getActiveLevel()->getActiveCamera() == this)
    {
         createGLMatrix(glmat, Projection);
	     glMatrixMode(GL_PROJECTION);
	     glLoadMatrixf(glmat);

	     createGLMatrix(glmat, View);
	     glMatrixMode(GL_MODELVIEW);
	     glLoadMatrixf(glmat);
    }
}

SViewFrustrum* CCameraSceneNode::getViewFrustrum()
{
    recalculateView();
    recalculateViewArea();
    return &ViewArea;
}

void CCameraSceneNode::recalculateProjection()
{
	Projection.buildProjectionMatrix(fovy, aspect, zn, zf);
}

void CCameraSceneNode::recalculateView()
{
    CVector3 position = getAbsolutePosition();

    matrix4 pmat = ((CSceneNode*)parentNode)->getAbsoluteTransformation();
    CVector3 target = getPosition() + LookDirection;
    pmat.transformVector3(target);

    CVector3 uporg(0.0f, 0.0f, 0.0f);
    CVector3 upvect = UpVector;
    pmat.transformVector3(uporg);
    pmat.transformVector3(upvect);

    View.buildCameraLookAtMatrixRH(position, target, upvect - uporg);
}

void CCameraSceneNode::recalculateViewArea()
{
	matrix4 mat = Projection*View;
	ViewArea = SViewFrustrum(mat);
}

void CCameraSceneNode::OnPrepareRender()
{
    m_scenemanager->registerNodeForRendering(this, ESNRP_CAMERA);
}
