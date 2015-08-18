#pragma once
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include <math.h>
#include <vector>
#include <random>


#define GRAVITATION_CONSTANT 10.0f
#define SINGLE_FORCE_MULTIPLIER 100.0f
class Integrable {
public:
	virtual void integrate(float dt) = 0;
};

