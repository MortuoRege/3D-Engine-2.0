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

#include "shader.h"
#include "camera.h"
#include "mesh.h"
#include "stb_image.h"
#include <vector>
#include <memory>

class Engine {
public:
    Engine();
    ~Engine();
    bool init();
    void run();
    void cleanup();

private:
    void update();
    void render();
    void checkCompileErrors(GLuint shader, std::string type);
    void processEventsAndInputs();
    bool isrunning = true;

    unsigned int texture;

    float deltaTime = 0.0f;
    float lastFrame = 0.0f;


    SDL_Window *window;
    SDL_GLContext glctx;

    std::unique_ptr<Shader> myshader;
    std::unique_ptr<Camera> camera = std::make_unique<Camera>();
    std::unique_ptr<Mesh> mesh = std::make_unique<Mesh>(std::vector<Mesh::Vertex>{
        // back face
            {{-0.5f, -0.5f, -0.5f}, {0,0,0}, {0.0f, 0.0f}, {1,1,1,1}},
            {{ 0.5f, -0.5f, -0.5f}, {0,0,0}, {1.0f, 0.0f}, {1,1,1,1}},
            {{ 0.5f,  0.5f, -0.5f}, {0,0,0}, {1.0f, 1.0f}, {1,1,1,1}},
            {{-0.5f,  0.5f, -0.5f}, {0,0,0}, {0.0f, 1.0f}, {1,1,1,1}},

            // front face
            {{-0.5f, -0.5f,  0.5f}, {0,0,0}, {0.0f, 0.0f}, {1,1,1,1}},
            {{ 0.5f, -0.5f,  0.5f}, {0,0,0}, {1.0f, 0.0f}, {1,1,1,1}},
            {{ 0.5f,  0.5f,  0.5f}, {0,0,0}, {1.0f, 1.0f}, {1,1,1,1}},
            {{-0.5f,  0.5f,  0.5f}, {0,0,0}, {0.0f, 1.0f}, {1,1,1,1}},
    }, std::vector<unsigned int>{

        0, 1, 2,
        2, 3, 0,

        // front face
        4, 5, 6,
        6, 7, 4,

        // left face
        4, 7, 3,
        3, 0, 4,

        // right face
        1, 5, 6,
        6, 2, 1,

        // bottom face
        0, 1, 5,
        5, 4, 0,

        // top face
        3, 2, 6,
        6, 7, 3});

    float texCoords[100];

};
