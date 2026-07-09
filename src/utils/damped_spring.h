#pragma once

#include <godot_cpp/godot.hpp>
#include <godot_cpp/core/object.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/vector3.hpp>

using namespace godot;

//******************************************************************************
// Credit: https://www.ryanjuckett.com/damped-springs/
// Cached set of motion parameters that can be used to efficiently update
// multiple springs using the same time step, angular frequency and damping
// ratio.
//******************************************************************************

class DampedSpring
{
public:
  DampedSpring();

	//******************************************************************************
	// This function will compute the parameters needed to simulate a damped spring
	// over a given period of time.
	// - An angular frequency is given to control how fast the spring oscillates.
	// - A damping ratio is given to control how fast the motion decays.
	//     damping ratio > 1: over damped
	//     damping ratio = 1: critically damped
	//     damping ratio < 1: under damped
	//******************************************************************************
	void CalcDampedSpringMotionParams(
		float deltaTime,        // time step to advance
		float angularFrequency, // angular frequency of motion
		float dampingRatio);     // damping ratio of motion
	
	//******************************************************************************
	// This function will update the supplied position and velocity values over
	// according to the motion parameters. 
	//******************************************************************************
	void UpdateDampedSpringMotion(Vector3& pos, Vector3& vel, const Vector3& eqPos);

private:
	float m_posPosCoef, m_posVelCoef;
	float m_velPosCoef, m_velVelCoef;
};
