#include "./header_files/engine.h"
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_oldnames.h>
#include <SDL3/SDL_scancode.h>
#include <SDL3/SDL_timer.h>
#include <math.h>

Engine::Engine(): isrunning(true), window(nullptr), glctx(nullptr), texture(0){}

Engine::~Engine() {
    cleanup();
}

bool Engine::init() {
    //initislizing the SDL
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("could not intialize SDL: %s", SDL_GetError());
        return false;
    }

    //setting the minimal OpenGL version1
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    //initializing the window
    window = SDL_CreateWindow("Engine", 640, 480, SDL_WINDOW_OPENGL);
    if (!window) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create window: %s", SDL_GetError());
        SDL_Quit();
        return false;
    }

    //creates an opengl context
    glctx = SDL_GL_CreateContext(window);
    if (!glctx) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_GL_CreateContext failed: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return false;
    }

    if (!SDL_GL_MakeCurrent(window, glctx)) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_GL_MakeCurrent failed: %s", SDL_GetError());
        SDL_GL_DestroyContext(glctx);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return false;
    }

    if (!gladLoadGL((GLADloadfunc)SDL_GL_GetProcAddress)) {
        std::cout << "Failed to initialize GLAD\n";
        SDL_GL_DestroyContext(glctx);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return false;
    }

    //=========================================
     //SDL_SetWindowRelativeMouseMode(window, true);
    //=========================================

    myshader = std::make_unique<Shader>("./shaders_files/shader.vertex", "./shaders_files/shader.fragment");
    mesh = std::make_unique<Mesh>(
            std::vector<Mesh::Vertex>{
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
    },
    std::vector<unsigned int>{

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
        6, 7, 3}
    );
    camera = std::make_unique<Camera>();

    mesh->createMesh();
    //texture setup note I don't need this many set up for the textures in the future
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

   //loading the texture imgae
    int width, height, nrChannels;
    unsigned char *data = stbi_load("./textures/wall.jpg", &width, &height, &nrChannels, 0);

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    //data was allocated by stbi_load, need to free it
    stbi_image_free(data);

    return true;
}

void Engine::run(){
    //just the game loop calculate the delta time for future use
    while(isrunning)
    {
        float currentFrame = SDL_GetTicks()*0.01f;
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processEventsAndInputs();
        render();
    }
}

void Engine::processEventsAndInputs() {
    SDL_Event event;
    const bool *state = SDL_GetKeyboardState(NULL);

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_EVENT_QUIT:
                isrunning = false;
                break;
            case SDL_EVENT_MOUSE_MOTION: {
                float xrel = static_cast<float>(event.motion.xrel);
                float yrel = static_cast<float>(event.motion.yrel);
                camera->rotate(xrel, -yrel);
                break;
            }
            default:
                break;
        }
    }

    if (state[SDL_SCANCODE_W]) camera->moveForward(deltaTime);
    if (state[SDL_SCANCODE_S]) camera->moveBackward(deltaTime);
    if (state[SDL_SCANCODE_A]) camera->moveLeft(deltaTime);
    if (state[SDL_SCANCODE_D]) camera->moveRight(deltaTime);
}

void Engine::render()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Use program BEFORE setting uniforms
    myshader->use();

    // Build matrices
    int w, h;
    SDL_GetWindowSize(window, &w, &h);
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)w/(float)h, 0.1f, 100.0f);

    glm::mat4 view = camera->lookAt();

    // Upload uniforms to their own locations
    GLint viewLoc  = glGetUniformLocation(myshader->ID, "view");
    GLint projLoc  = glGetUniformLocation(myshader->ID, "projection");

    glUniformMatrix4fv(viewLoc,  1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc,  1, GL_FALSE, glm::value_ptr(projection));

    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(60.f), glm::vec3(1.0f, 0.3f, 0.5f));
    myshader->setMat4("model", model);

    mesh->draw();

    SDL_GL_SwapWindow(window);
}

void Engine::cleanup(){
    if (glctx) SDL_GL_DestroyContext(glctx); glctx = nullptr;
    if(window) SDL_DestroyWindow(window); window = nullptr;
    SDL_Quit();
}
