#include "display.h"

#include <glm\gtx\transform.hpp>
#include <GL\glew.h>

#include <iostream>



Display::Display(int w, int h, const std::string &name){
	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);	

	window = SDL_CreateWindow(name.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, SDL_WINDOW_OPENGL);

	context = SDL_GL_CreateContext(window);

	GLenum status = glewInit();

	if (status != GLEW_OK) {
		std::cerr << "Glew failed to initialize" << std::endl;
	}
	
	isClosed = false;

	SetGLAttributes();
}

static int oldX = 0, oldY = 0;

void Display::Update(Camera &camera, Sim &sim){
	SDL_GL_SwapWindow(window);

	SDL_Event e;

	while (SDL_PollEvent(&e)) {
		switch (e.type) {
			case SDL_QUIT: {
				isClosed = true;
				break;
			}
			case SDL_KEYDOWN: {
				switch (e.key.keysym.sym) {
					case SDLK_LEFT:
						camera.GetPos() -= glm::normalize(glm::cross(camera.GetFwd(), camera.GetUp())) * 0.05f;
						break;					
					case SDLK_RIGHT: 
						camera.GetPos() += glm::normalize(glm::cross(camera.GetFwd(), camera.GetUp())) * 0.05f;
						break;	
					case SDLK_UP:
						camera.GetPos() = camera.GetPos() + camera.GetFwd() * 0.05f;
						break;
					case SDLK_DOWN:
						camera.GetPos() = camera.GetPos() - camera.GetFwd() * 0.05f;
						break;
					case SDLK_p:						
						sim.ToggleStop();
						break;
					case SDLK_SPACE: {
						glm::vec3 spherepos = camera.GetPos() + camera.GetFwd()*2.0f;
						sim.GetObjects().push_back(new Object());
						sim.GetObjects().back()->InitOBJTest("obj_models\\simple_sphere.obj", 2.0f, spherepos, camera.GetFwd() * 50.0f);
						sim.GetObjects().back()->GenerateBoundingBox();
						break;
					}
					default:
						break;
				}
				break;
			}
			case SDL_MOUSEMOTION: {
				glm::vec2 mousePos;
				
				int X, Y;
				SDL_GetMouseState(&X, &Y);

				glm::vec2 mouseDelta = glm::vec2(X, Y) - glm::vec2(oldX, oldY);

				if (glm::length(mouseDelta) > 20.0f) {
					oldX = X;
					oldY = Y;
					return;
				}

				static const float ROT_SPEED = 0.005f;

				camera.GetFwd() = glm::mat3(glm::rotate(-mouseDelta.x * ROT_SPEED, camera.GetUp())) * camera.GetFwd();

				glm::vec3 toRotateAround = glm::cross(camera.GetFwd(), camera.GetUp());

				camera.GetFwd() = glm::mat3(glm::rotate(-mouseDelta.y * ROT_SPEED, toRotateAround)) * camera.GetFwd();

				oldX = X;
				oldY = Y;		
			}
			default:	
				break;	
		}
	}
	
}

bool Display::IsClosed()
{
	return isClosed;
}

void Display::Clear(float r, float g, float b, float a){
	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

Display::~Display(){
	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void Display::SetGLAttributes(){
	//glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CW);

	glEnable(GL_DEPTH_TEST);

}
