#pragma once

#include <gl/glut.h>

#include "../../!!adGlobals/vector_math.h"


class StereoCamera
{
private:
public:
	float mConvergence;
	float mEyeSeparation;
	float mAspectRatio;
	float mFOV;
	float mNearClippingDistance;
	float mFarClippingDistance;

StereoCamera(   
			float Convergence, 
			float EyeSeparation, 
			float AspectRatio, 
			float FOV, 
			float NearClippingDistance, 
			float FarClippingDistance
			)
	{
		mConvergence            = Convergence; 
		mEyeSeparation          = EyeSeparation; 
		mAspectRatio            = AspectRatio; 
		mFOV                    = FOV * PI / 180.0f; 
		mNearClippingDistance   = NearClippingDistance;
		mFarClippingDistance    = FarClippingDistance;
	}

	void ApplyCentralMonoFrustum()
	{
		float top, bottom, left, right;

		top     = mNearClippingDistance * tan(mFOV/2);
		bottom  = -top;

		left    = -mAspectRatio * tan(mFOV/2) * mNearClippingDistance;
		right   =  mAspectRatio * tan(mFOV/2) * mNearClippingDistance;

		// Set the Projection Matrix
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
			glFrustum(left,right, bottom,top, mNearClippingDistance,mFarClippingDistance);

		// Displace the world to right
		glMatrixMode(GL_MODELVIEW);                     
		glLoadIdentity();
	}

	float ConvergenceRight()
	{	return mAspectRatio * tan(mFOV/2) * mConvergence;
	}

	float ConvergenceTop()
	{	return tan(mFOV/2) * mConvergence;
	}

	void ApplyLeftFrustum()
	{
		float top, bottom, left, right;

		top     = mNearClippingDistance * tan(mFOV/2);
		bottom  = -top;

		float a = mAspectRatio * tan(mFOV/2) * mConvergence;

		float b = a - mEyeSeparation/2;
		float c = a + mEyeSeparation/2;

		left    = -b * mNearClippingDistance/mConvergence;
		right   =  c * mNearClippingDistance/mConvergence;

		// Set the Projection Matrix
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();   
			glFrustum(left,right, bottom,top, mNearClippingDistance,mFarClippingDistance);

		// Displace the world to right
		glMatrixMode(GL_MODELVIEW);                     
		glLoadIdentity();   
			glTranslatef(mEyeSeparation/2, 0.0f, 0.0f);
	}

	void ApplyRightFrustum()
	{
		float top, bottom, left, right;

		top     = mNearClippingDistance * tan(mFOV/2);
		bottom  = -top;

		float a = mAspectRatio * tan(mFOV/2) * mConvergence;

		float b = a - mEyeSeparation/2;
		float c = a + mEyeSeparation/2;

		left    =  -c * mNearClippingDistance/mConvergence;
		right   =   b * mNearClippingDistance/mConvergence;

		// Set the Projection Matrix
		glMatrixMode(GL_PROJECTION);                        
		glLoadIdentity();   
			glFrustum(left,right, bottom,top, mNearClippingDistance,mFarClippingDistance);

		// Displace the world to left
		glMatrixMode(GL_MODELVIEW);                     
		glLoadIdentity();   
			glTranslatef(-mEyeSeparation/2, 0.0f, 0.0f);
	}
};