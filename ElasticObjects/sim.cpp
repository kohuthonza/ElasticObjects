#include "sim.h"

Sim::Sim() {
	objects.push_back(new Object());
	objects.push_back(new Object());
	objects[0]->InitTestHexa();
	objects[1]->InitTest();
}

void Sim::init()                         // This Method Will Call The init() Method Of Every Mass.
{

}

void Sim::Solve() {
	for (unsigned int a = 0; a < objects.size(); ++a)
		objects[a]->Solve();
}

void Sim::Simulate(float dt) {
	for (unsigned int a = 0; a < objects.size(); ++a)                // We Will Iterate Every Mass.
		objects[a]->Simulate(dt);
}

void Sim::Draw() {
	for (unsigned int a = 0; a < objects.size(); ++a)
		objects[a]->Draw();
}

void Sim::Operate(float dt) {
	init();
	Solve();
	Simulate(dt);
	Draw();
}
