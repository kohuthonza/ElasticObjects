#pragma once

#include <vector>

#include "object.h"

class Sim {
public:
	Sim();

	virtual void init();

	virtual void Solve();

	virtual void Simulate(float dt);

	virtual void Draw();

	virtual void Operate(float dt);

private:
	std::vector<Object *> objects;
};