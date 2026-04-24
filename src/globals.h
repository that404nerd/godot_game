#pragma once

#include "godot_cpp/core/class_db.hpp"
#include <godot_cpp/godot.hpp>
#include <godot_cpp/core/math.hpp>

#include <array>

using namespace godot;

/* 
  Credit to Withaust for macro (defining each one by default is a pain in the ass)
  The getter, setter functions will not match the conventions I'm using for other functions
*/
#define _CONCAT(x, y) #x "" #y
#define _TOKEN_PASTE(x, y) x##y
#define _CAT(x, y) _TOKEN_PASTE(x, y)
#define GD_CLASS(p_class, p_inherits) \
private:                              \
    typedef p_inherits Super;         \
    typedef p_class CurrentClass;     \
    GDCLASS(p_class, p_inherits);     \
    struct _CAT(__semicolon_place, __LINE__) // Forces semicolon use

#define GD_DEFINE_PROPERTY(p_type, p_name, p_default_value) \
private:                                                 \
    p_type p_name = p_default_value;                     \
public:                                                  \
    void set_##p_name(p_type value) {                    \
        p_name = value;                                  \
    }                                                    \
    p_type get_##p_name() {                              \
        return p_name;                                   \
    }                                                    \
    struct _CAT(__semicolon_place, __LINE__)

// For binding a general property
#define GD_BIND_PROPERTY(p_class, p_name, p_type) \
        ClassDB::bind_method(D_METHOD("get_" #p_name), &p_class::get_##p_name); \
        ClassDB::bind_method(D_METHOD("set_" #p_name, "p_" #p_name), &p_class::set_##p_name); \
        ADD_PROPERTY(PropertyInfo(p_type, #p_name), "set_" #p_name, "get_" #p_name);

// For binding a property like an Array, Nodes etc...
#define GD_BIND_CUSTOM_PROPERTY(p_class, p_name, p_type, p_property_type) \
        ClassDB::bind_method(D_METHOD("get_"#p_name), &p_class::get_##p_name); \
        ClassDB::bind_method(D_METHOD("set_"#p_name, "p_"#p_name), &p_class::set_##p_name); \
        ADD_PROPERTY(PropertyInfo(p_type, #p_name, p_property_type), "set_" #p_name, "get_" #p_name);

#define GD_BIND_ENUM(p_class, p_name, p_enum_values) \
        ClassDB::bind_method(D_METHOD("get_" #p_name), &p_class::get_##p_name); \
        ClassDB::bind_method(D_METHOD("set_" #p_name, "p_" #p_name), &p_class::set_##p_name); \
        ADD_PROPERTY(PropertyInfo(Variant::INT, #p_name, PROPERTY_HINT_ENUM, p_enum_values), \
                    "set_"#p_name, "get_"#p_name);

namespace Utils {

    // Exponential decay function
    template <typename T>
    inline T exp_decay(T a, T b, float decay, double dt)
    {
        return b + (a - b) * Math::exp(-decay * dt);
    }
  
	//******************************************************************************
	// Credit: https://www.ryanjuckett.com/damped-springs/
	// Cached set of motion parameters that can be used to efficiently update
	// multiple springs using the same time step, angular frequency and damping
	// ratio.
	//******************************************************************************
	struct tDampedSpringMotionParams
	{
		// newPos = posPosCoef*oldPos + posVelCoef*oldVel
		float m_posPosCoef, m_posVelCoef;
		// newVel = velPosCoef*oldPos + velVelCoef*oldVel
		float m_velPosCoef, m_velVelCoef;
	};

	//******************************************************************************
	// This function will compute the parameters needed to simulate a damped spring
	// over a given period of time.
	// - An angular frequency is given to control how fast the spring oscillates.
	// - A damping ratio is given to control how fast the motion decays.
	//     damping ratio > 1: over damped
	//     damping ratio = 1: critically damped
	//     damping ratio < 1: under damped
	//******************************************************************************
	inline void CalcDampedSpringMotionParams(
		tDampedSpringMotionParams& pOutParams,       // motion parameters result
		float	                   deltaTime,        // time step to advance
		float	                   angularFrequency, // angular frequency of motion
		float	                   dampingRatio)     // damping ratio of motion
	{
		const float epsilon = 0.0001f;

		// force values into legal range
		if (dampingRatio     < 0.0f) dampingRatio     = 0.0f;
		if (angularFrequency < 0.0f) angularFrequency = 0.0f;

		// if there is no angular frequency, the spring will not move and we can
		// return identity
		if ( angularFrequency < epsilon )
		{
			pOutParams.m_posPosCoef = 1.0f; pOutParams.m_posVelCoef = 0.0f;
			pOutParams.m_velPosCoef = 0.0f; pOutParams.m_velVelCoef = 1.0f;
			return;
		}

		if (dampingRatio > 1.0f + epsilon)
		{
			// over-damped
			float za = -angularFrequency * dampingRatio;
			float zb = angularFrequency * sqrtf(dampingRatio*dampingRatio - 1.0f);
			float z1 = za - zb;
			float z2 = za + zb;

			float e1 = expf( z1 * deltaTime );
			float e2 = expf( z2 * deltaTime );

			float invTwoZb = 1.0f / (2.0f*zb); // = 1 / (z2 - z1)
				
			float e1_Over_TwoZb = e1*invTwoZb;
			float e2_Over_TwoZb = e2*invTwoZb;

			float z1e1_Over_TwoZb = z1*e1_Over_TwoZb;
			float z2e2_Over_TwoZb = z2*e2_Over_TwoZb;

			pOutParams.m_posPosCoef =  e1_Over_TwoZb*z2 - z2e2_Over_TwoZb + e2;
			pOutParams.m_posVelCoef = -e1_Over_TwoZb    + e2_Over_TwoZb;

			pOutParams.m_velPosCoef = (z1e1_Over_TwoZb - z2e2_Over_TwoZb + e2)*z2;
			pOutParams.m_velVelCoef = -z1e1_Over_TwoZb + z2e2_Over_TwoZb;
		}
		else if (dampingRatio < 1.0f - epsilon)
		{
			// under-damped
			float omegaZeta = angularFrequency * dampingRatio;
			float alpha     = angularFrequency * sqrtf(1.0f - dampingRatio*dampingRatio);

			float expTerm = expf( -omegaZeta * deltaTime );
			float cosTerm = cosf( alpha * deltaTime );
			float sinTerm = sinf( alpha * deltaTime );
				
			float invAlpha = 1.0f / alpha;

			float expSin = expTerm*sinTerm;
			float expCos = expTerm*cosTerm;
			float expOmegaZetaSin_Over_Alpha = expTerm*omegaZeta*sinTerm*invAlpha;

			pOutParams.m_posPosCoef = expCos + expOmegaZetaSin_Over_Alpha;
			pOutParams.m_posVelCoef = expSin*invAlpha;

			pOutParams.m_velPosCoef = -expSin*alpha - omegaZeta*expOmegaZetaSin_Over_Alpha;
			pOutParams.m_velVelCoef =  expCos - expOmegaZetaSin_Over_Alpha;
		}
		else
		{
			// critically damped
			float expTerm     = expf( -angularFrequency*deltaTime );
			float timeExp     = deltaTime*expTerm;
			float timeExpFreq = timeExp*angularFrequency;

			pOutParams.m_posPosCoef = timeExpFreq + expTerm;
			pOutParams.m_posVelCoef = timeExp;

			pOutParams.m_velPosCoef = -angularFrequency*timeExpFreq;
			pOutParams.m_velVelCoef = -timeExpFreq + expTerm;
		}
	}
	
	//******************************************************************************
	// This function will update the supplied position and velocity values over
	// according to the motion parameters.
	//******************************************************************************
	template <typename T>
	inline void UpdateDampedSpringMotion(
		T&                           pPos,           // position value to update
		T&                           pVel,           // velocity value to update
		const T                      equilibriumPos, // position to approach
		const tDampedSpringMotionParams& params)         // motion parameters to use
	{		
		const T oldPos = pPos - equilibriumPos; // update in equilibrium relative space
		const T oldVel = pVel;

		pPos = oldPos*params.m_posPosCoef + oldVel*params.m_posVelCoef + equilibriumPos;
		pVel = oldPos*params.m_velPosCoef + oldVel*params.m_velVelCoef;
	}

};
