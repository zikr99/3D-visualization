#ifndef __CAMERAACTION_H__
#define __CAMERAACTION_H__

enum ECAMERA_ACTION
{
     ECA_MOVE_FORWARD = 0,
	 ECA_MOVE_BACKWARD,
	 ECA_STRAFE_LEFT,
	 ECA_STRAFE_RIGHT,
	 ECA_COUNT,

	 //! Not used. It only forces this enumeration to compile in 32 bit. 
	 ECA_FORCE_32BIT = 0x7fffffff	
};

#endif
