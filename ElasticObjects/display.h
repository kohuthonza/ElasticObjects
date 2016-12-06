#pragma once
#include <string>

#include <SDL.h>

#include "camera.h"
#include "lightPosition.h"
#include "sim.h"

class Display {
public:
	Display(int w, int h, const std::string &name);

	void Update(Camera &camera, LightPosition &light, Sim &sim);

	bool IsClosed();

	void Clear(float r, float g, float b, float a);

	virtual ~Display();

private:
	SDL_Window *window;
	SDL_GLContext context;

	bool isClosed;

	void SetGLAttributes();

};