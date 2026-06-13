#include "damped_spring.h"

DampedSpring::DampedSpring()
  : m_posPosCoef(0.0f), m_posVelCoef(0.0f), m_velPosCoef(0.0f), m_velVelCoef(0.0f)
{
}

void DampedSpring::CalcDampedSpringMotionParams(
	float deltaTime,        // time step to advance
	float angularFrequency, // angular frequency of motion
	float dampingRatio)     // damping ratio of motion
{
	const float epsilon = 0.0001f;

	// force values into legal range
	if (dampingRatio     < 0.0f) dampingRatio     = 0.0f;
	if (angularFrequency < 0.0f) angularFrequency = 0.0f;

	// if there is no angular frequency, the spring will not move and we can
	// return identity
	if ( angularFrequency < epsilon )
	{
		m_posPosCoef = 1.0f; m_posVelCoef = 0.0f;
		m_velPosCoef = 0.0f; m_velVelCoef = 1.0f;
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

		m_posPosCoef =  e1_Over_TwoZb*z2 - z2e2_Over_TwoZb + e2;
		m_posVelCoef = -e1_Over_TwoZb    + e2_Over_TwoZb;

		m_velPosCoef = (z1e1_Over_TwoZb - z2e2_Over_TwoZb + e2)*z2;
		m_velVelCoef = -z1e1_Over_TwoZb + z2e2_Over_TwoZb;
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

		m_posPosCoef = expCos + expOmegaZetaSin_Over_Alpha;
		m_posVelCoef = expSin*invAlpha;

		m_velPosCoef = -expSin*alpha - omegaZeta*expOmegaZetaSin_Over_Alpha;
		m_velVelCoef =  expCos - expOmegaZetaSin_Over_Alpha;
	}
	else
	{
		// critically damped
		float expTerm     = expf( -angularFrequency*deltaTime );
		float timeExp     = deltaTime*expTerm;
		float timeExpFreq = timeExp*angularFrequency;

		m_posPosCoef = timeExpFreq + expTerm;
		m_posVelCoef = timeExp;

		m_velPosCoef = -angularFrequency*timeExpFreq;
		m_velVelCoef = -timeExpFreq + expTerm;
	}
}

void DampedSpring::UpdateDampedSpringMotion(Vector3 pos, Vector3 vel, const Vector3& eqPos)
{	
	m_pPos = pos;
	m_pVel = vel;
	m_EquilibriumPos = eqPos;

	const Vector3 oldPos = m_pPos - m_EquilibriumPos; // update in equilibrium relative space
	const Vector3 oldVel = m_pVel;
	
	m_pPos = oldPos * m_posPosCoef + oldVel * m_posVelCoef + m_EquilibriumPos;
	m_pVel = oldPos * m_velPosCoef + oldVel * m_velVelCoef;
}
