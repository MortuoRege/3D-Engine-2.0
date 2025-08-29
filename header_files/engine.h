#include <glad/gl.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_oldnames.h>
#include <SDL3/SDL_video.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "./shader.h"
#include "./camera.h"
#include "./mesh.h"
#include "./stb_image.h"
#include <memory>

class Engine {
public:
    Engine();
    ~Engine();
    bool init();
    void run();
    void cleanup();

private:
    void render();
    void processEventsAndInputs();
    bool isrunning = true;

    unsigned int texture;

    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    SDL_Window *window;
    SDL_GLContext glctx;

    std::unique_ptr<Shader> myshader;
    std::unique_ptr<Camera> camera;
    std::unique_ptr<Mesh> mesh;

    float texCoords[100];

};
