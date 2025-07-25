//
// COMP 371 Labs Framework
//
// Created by Nicolas Bergeron on 20/06/2019.
//

#include <algorithm>
#include <iostream>

#define GLEW_STATIC                                                            \
  1 // This allows linking with Static Library on Windows, without DLL
#include <GL/glew.h> // Include GLEW - OpenGL Extension Wrangler

#include <GLFW/glfw3.h> // GLFW provides a cross-platform interface for creating a graphical context,
                        // initializing OpenGL and binding inputs

#include <glm/common.hpp>
#include <glm/glm.hpp> // GLM is an optimized math library with syntax to similar to OpenGL Shading Language
#include <glm/gtc/matrix_transform.hpp> // include this to create transformation matrices

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

using namespace glm;
using namespace std;

GLuint loadTexture(const char *filename);

const char *getVertexShaderSource();

const char *getFragmentShaderSource();

const char *getTexturedVertexShaderSource();

const char *getTexturedFragmentShaderSource();

int compileAndLinkShaders(const char *vertexShaderSource,
                          const char *fragmentShaderSource);

struct TexturedColoredVertex {
  TexturedColoredVertex(vec3 _position, vec3 _color, vec2 _uv)
      : position(_position), color(_color), uv(_uv) {}

  vec3 position;
  vec3 color;
  vec2 uv;
};

// Textured Cube model
const TexturedColoredVertex texturedCubeVertexArray[] = { // position, color
    TexturedColoredVertex(vec3(-0.5f, -0.5f, -0.5f), 
                          vec3(1.0f, 0.0f, 0.0f),
                          vec2(0.0f, 0.0f)), // left - red
    TexturedColoredVertex(vec3(-0.5f, -0.5f, 0.5f), 
                          vec3(1.0f, 0.0f, 0.0f),
                          vec2(0.0f, 1.0f)),
    TexturedColoredVertex(vec3(-0.5f, 0.5f, 0.5f), 
                          vec3(1.0f, 0.0f, 0.0f),
                          vec2(1.0f, 1.0f)),

    TexturedColoredVertex(vec3(-0.5f, -0.5f, -0.5f), vec3(1.0f, 0.0f, 0.0f),
                          vec2(0.0f, 0.0f)),
    TexturedColoredVertex(vec3(-0.5f, 0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f),
                          vec2(1.0f, 1.0f)),
    TexturedColoredVertex(vec3(-0.5f, 0.5f, -0.5f), vec3(1.0f, 0.0f, 0.0f),
                          vec2(1.0f, 0.0f)),

    TexturedColoredVertex(vec3(0.5f, 0.5f, -0.5f), vec3(0.0f, 0.0f, 1.0f),
                          vec2(1.0f, 1.0f)), // far - blue
    TexturedColoredVertex(vec3(-0.5f, -0.5f, -0.5f), vec3(0.0f, 0.0f, 1.0f),
                          vec2(0.0f, 0.0f)),
    TexturedColoredVertex(vec3(-0.5f, 0.5f, -0.5f), vec3(0.0f, 0.0f, 1.0f),
                          vec2(0.0f, 1.0f)),

    TexturedColoredVertex(vec3(0.5f, 0.5f, -0.5f), vec3(0.0f, 0.0f, 1.0f),
                          vec2(1.0f, 1.0f)),
    TexturedColoredVertex(vec3(0.5f, -0.5f, -0.5f), vec3(0.0f, 0.0f, 1.0f),
                          vec2(1.0f, 0.0f)),
    TexturedColoredVertex(vec3(-0.5f, -0.5f, -0.5f), vec3(0.0f, 0.0f, 1.0f),
                          vec2(0.0f, 0.0f)),

    TexturedColoredVertex(vec3(0.5f, -0.5f, 0.5f), vec3(0.0f, 1.0f, 1.0f),
                          vec2(1.0f, 1.0f)), // bottom - turquoise
    TexturedColoredVertex(vec3(-0.5f, -0.5f, -0.5f), vec3(0.0f, 1.0f, 1.0f),
                          vec2(0.0f, 0.0f)),
    TexturedColoredVertex(vec3(0.5f, -0.5f, -0.5f), vec3(0.0f, 1.0f, 1.0f),
                          vec2(1.0f, 0.0f)),

    TexturedColoredVertex(vec3(0.5f, -0.5f, 0.5f), vec3(0.0f, 1.0f, 1.0f),
                          vec2(1.0f, 1.0f)),
    TexturedColoredVertex(vec3(-0.5f, -0.5f, 0.5f), vec3(0.0f, 1.0f, 1.0f),
                          vec2(0.0f, 1.0f)),
    TexturedColoredVertex(vec3(-0.5f, -0.5f, -0.5f), vec3(0.0f, 1.0f, 1.0f),
                          vec2(0.0f, 0.0f)),

    TexturedColoredVertex(vec3(-0.5f, 0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f),
                          vec2(0.0f, 1.0f)), // near - green
    TexturedColoredVertex(vec3(-0.5f, -0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f),
                          vec2(0.0f, 0.0f)),
    TexturedColoredVertex(vec3(0.5f, -0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f),
                          vec2(1.0f, 0.0f)),

    TexturedColoredVertex(vec3(0.5f, 0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f),
                          vec2(1.0f, 1.0f)),
    TexturedColoredVertex(vec3(-0.5f, 0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f),
                          vec2(0.0f, 1.0f)),
    TexturedColoredVertex(vec3(0.5f, -0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f),
                          vec2(1.0f, 0.0f)),

    TexturedColoredVertex(vec3(0.5f, 0.5f, 0.5f), vec3(1.0f, 0.0f, 1.0f),
                          vec2(1.0f, 1.0f)), // right - purple
    TexturedColoredVertex(vec3(0.5f, -0.5f, -0.5f), vec3(1.0f, 0.0f, 1.0f),
                          vec2(0.0f, 0.0f)),
    TexturedColoredVertex(vec3(0.5f, 0.5f, -0.5f), vec3(1.0f, 0.0f, 1.0f),
                          vec2(1.0f, 0.0f)),

    TexturedColoredVertex(vec3(0.5f, -0.5f, -0.5f), vec3(1.0f, 0.0f, 1.0f),
                          vec2(0.0f, 0.0f)),
    TexturedColoredVertex(vec3(0.5f, 0.5f, 0.5f), vec3(1.0f, 0.0f, 1.0f),
                          vec2(1.0f, 1.0f)),
    TexturedColoredVertex(vec3(0.5f, -0.5f, 0.5f), vec3(1.0f, 0.0f, 1.0f),
                          vec2(0.0f, 1.0f)),

    TexturedColoredVertex(vec3(0.5f, 0.5f, 0.5f), vec3(1.0f, 1.0f, 0.0f),
                          vec2(1.0f, 1.0f)), // top - yellow
    TexturedColoredVertex(vec3(0.5f, 0.5f, -0.5f), vec3(1.0f, 1.0f, 0.0f),
                          vec2(1.0f, 0.0f)),
    TexturedColoredVertex(vec3(-0.5f, 0.5f, -0.5f), vec3(1.0f, 1.0f, 0.0f),
                          vec2(0.0f, 0.0f)),

    TexturedColoredVertex(vec3(0.5f, 0.5f, 0.5f), vec3(1.0f, 1.0f, 0.0f),
                          vec2(1.0f, 1.0f)),
    TexturedColoredVertex(vec3(-0.5f, 0.5f, -0.5f), vec3(1.0f, 1.0f, 0.0f),
                          vec2(0.0f, 0.0f)),
    TexturedColoredVertex(vec3(-0.5f, 0.5f, 0.5f), vec3(1.0f, 1.0f, 0.0f),
                          vec2(0.0f, 1.0f))
};




int createTexturedCubeVertexArrayObject();

void setProjectionMatrix(int shaderProgram, mat4 projectionMatrix) {
  glUseProgram(shaderProgram);
  GLuint projectionMatrixLocation =
      glGetUniformLocation(shaderProgram, "projectionMatrix");
  glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE,
                     &projectionMatrix[0][0]);
}

void setViewMatrix(int shaderProgram, mat4 viewMatrix) {
  glUseProgram(shaderProgram);
  GLuint viewMatrixLocation = glGetUniformLocation(shaderProgram, "viewMatrix");
  glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);
}

void setWorldMatrix(int shaderProgram, mat4 worldMatrix) {
  glUseProgram(shaderProgram);
  GLuint worldMatrixLocation =
      glGetUniformLocation(shaderProgram, "worldMatrix");
  glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
}

int main(int argc, char *argv[]) {
  // Initialize GLFW and OpenGL version
  glfwInit();

#if defined(PLATFORM_OSX)
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#else
  // On windows, we set OpenGL version to 2.1, to support more hardware
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
#endif

  // Create Window and rendering context using GLFW, resolution is 800x600
  GLFWwindow *window =
      glfwCreateWindow(800, 600, "Comp371 - Assignment 1", NULL, NULL);
  if (window == NULL) {
    std::cerr << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
  glfwMakeContextCurrent(window);

  // Initialize GLEW
  glewExperimental = true; // Needed for core profile
  if (glewInit() != GLEW_OK) {
    std::cerr << "Failed to create GLEW" << std::endl;
    glfwTerminate();
    return -1;
  }

  // Load Textures
  GLuint brickTextureID = loadTexture("Textures/brick.jpg");
  GLuint cementTextureID = loadTexture("Textures/cement.jpg");
  GLuint sky_posx = loadTexture("Skybox/posx.jpg");
  GLuint sky_negx = loadTexture("Skybox/negx.jpg");
  GLuint sky_posy = loadTexture("Skybox/posy.jpg");
  GLuint sky_negy = loadTexture("Skybox/negy.jpg");
  GLuint sky_posz = loadTexture("Skybox/posz.jpg");
  GLuint sky_negz = loadTexture("Skybox/negz.jpg");
  // Black background
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

  // Compile and link shaders here ...
  int colorShaderProgram =
      compileAndLinkShaders(getVertexShaderSource(), getFragmentShaderSource());
  int texturedShaderProgram = compileAndLinkShaders(
      getTexturedVertexShaderSource(), getTexturedFragmentShaderSource());

  // Camera parameters for view transform
  vec3 cameraPosition(0.6f, 1.0f, 10.0f);
  vec3 cameraLookAt(0.0f, 0.0f, -1.0f);
  vec3 cameraUp(0.0f, 1.0f, 0.0f);

  // Other camera parameters
  float cameraSpeed = 1.0f;
  float cameraFastSpeed = 10 * cameraSpeed;
  float cameraHorizontalAngle = 90.0f;
  float cameraVerticalAngle = 0.0f;
  bool cameraFirstPerson = true; // press 1 or 2 to toggle this variable

  // Spinning cube at camera position
  float spinningCubeAngle = 0.0f;

  // Set projection matrix for shader, this won't change
  mat4 projectionMatrix =
      glm::perspective(70.0f,           // field of view in degrees
                       800.0f / 600.0f, // aspect ratio
                       0.01f, 100.0f);  // near and far (near > 0)

  // Set initial view matrix
  mat4 viewMatrix = lookAt(cameraPosition,                // eye
                           cameraPosition + cameraLookAt, // center
                           cameraUp);                     // up

  // Set View and Projection matrices on both shaders
  setViewMatrix(colorShaderProgram, viewMatrix);
  setViewMatrix(texturedShaderProgram, viewMatrix);

  setProjectionMatrix(colorShaderProgram, projectionMatrix);
  setProjectionMatrix(texturedShaderProgram, projectionMatrix);

  // Define and upload geometry to the GPU here ...
  int texturedCubeVAO = createTexturedCubeVertexArrayObject();

  // For frame time
  float lastFrameTime = glfwGetTime();
  int lastMouseLeftState = GLFW_RELEASE;
  double lastMousePosX, lastMousePosY;
  glfwGetCursorPos(window, &lastMousePosX, &lastMousePosY);

  // Other OpenGL states to set once
  // Enable Backface culling
  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);

  // we only draw cubes
  glBindVertexArray(texturedCubeVAO);

  // variables
  float angle = 0.0f;
  float x, z = 0.0f;
  float rotationSpeed = 2.0f; // 1 degrees per second

  // Entering Main Loop
  while (!glfwWindowShouldClose(window)) {


    // Frame time calculation
    float dt = glfwGetTime() - lastFrameTime;
    lastFrameTime += dt;

    // Each frame, reset color of each pixel to glClearColor
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDepthMask(GL_FALSE); // Disable depth writing

    // Draw textured geometry
    glUseProgram(texturedShaderProgram);
    mat4 skyboxWorldMatrix = scale(mat4(1.0f), vec3(100.0f));
    setWorldMatrix(texturedShaderProgram, skyboxWorldMatrix);
    glCullFace(GL_FRONT); // Add before drawing the skybox

    // Draw each face with the correct texture
    // Left (-X)
    glBindTexture(GL_TEXTURE_2D, sky_negx);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    // Back (-Z)
    glBindTexture(GL_TEXTURE_2D, sky_negz);
    glDrawArrays(GL_TRIANGLES, 6, 6);
    // Bottom (-Y)
    glBindTexture(GL_TEXTURE_2D, sky_negy);
    glDrawArrays(GL_TRIANGLES, 12, 6);
    // Front (+Z)
    glBindTexture(GL_TEXTURE_2D, sky_posz);
    glDrawArrays(GL_TRIANGLES, 18, 6);
    // Right (+X)
    glBindTexture(GL_TEXTURE_2D, sky_posx);
    glDrawArrays(GL_TRIANGLES, 24, 6);
    // Top (+Y)
    glBindTexture(GL_TEXTURE_2D, sky_posy);
    glDrawArrays(GL_TRIANGLES, 30, 6);

    glDepthMask(GL_TRUE);
    glCullFace(GL_BACK); // Restore after drawing the skybox
    glActiveTexture(GL_TEXTURE0);
    GLuint textureLocation =
        glGetUniformLocation(texturedShaderProgram, "textureSampler");
    glBindTexture(GL_TEXTURE_2D, brickTextureID);
    glUniform1i(textureLocation,
                0); // Set our Texture sampler to user Texture Unit 0

    // rate of rotation
    angle = (angle + rotationSpeed * dt); // angles in degrees, but glm expects
                                          // radians (conversion below);
    // glm::mat4 rotationMatrix = glm::rotate(translationMatrix,
    // glm::radians(angle)+radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    //  car vector
    float radius = 5.0f;
    vec3 carLocation = vec3(radius * cos(angle), 0.0f, radius * sin(angle));
    mat4 translationMatrix = translate(mat4(1.0f), carLocation);

    glm::mat4 rotationMatrix =
        glm::rotate(mat4(1.0f), -angle+radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    // Draw simple car to go in circles
    glBindTexture(GL_TEXTURE_2D, cementTextureID);
    mat4 carWorldMatrix = translationMatrix * rotationMatrix;
    setWorldMatrix(texturedShaderProgram, carWorldMatrix);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // drawing a top part relative to the bottom part of the car.
    glBindTexture(GL_TEXTURE_2D, brickTextureID);
    mat4 carTOPWorldMatrix = carWorldMatrix *
                             scale(mat4(1.0f), vec3(0.5f, 0.25f, 1.0f)) *
                             translate(mat4(1.0f), vec3(0.0f, 2.5f, 0.0f));
    setWorldMatrix(texturedShaderProgram, carTOPWorldMatrix);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // set to draw 4 simplified cube wheels now relative to mainbody.
    //glBindTexture(GL_TEXTURE_2D, wheelTextureID);
    const float carPosX[4] = {-0.5f,-0.5f,0.5f,0.5f};
    const float carPosZ[4] = {0.5f,-0.5f,0.5f,-0.5f};
    for(int i = 0; i < 4; i++) {
      mat4 carWheelWorldMatrix = carWorldMatrix * 
                            scale(mat4(1.0f), vec3(0.4f, 0.4f, 0.2f)) *
                            translate(mat4(1.0f), vec3(5*carPosX[i], -0.5f, 5*carPosZ[i]));
      rotationMatrix =
        glm::rotate(mat4(1.0f), 10*angle, glm::vec3(0.0f, 0.0f, 1.0f));
      carWheelWorldMatrix = carWheelWorldMatrix * rotationMatrix;
      setWorldMatrix(texturedShaderProgram, carWheelWorldMatrix);
      glDrawArrays(GL_TRIANGLES, 0, 36);
    }


    // Draw colored geometry
    glUseProgram(colorShaderProgram);

    // Spinning cube at camera position
    spinningCubeAngle += 180.0f * dt;

    // Draw avatar in view space for first person camera
    // and in world space for third person camera
    if (cameraFirstPerson) {
      // Wolrd matrix is identity, but view transform like a world transform
      // relative to camera basis (1 unit in front of camera)
      //
      // This is similar to a weapon moving with camera in a shooter game
      mat4 spinningCubeViewMatrix =
          translate(mat4(1.0f), vec3(0.0f, 0.0f, -1.0f)) *
          rotate(mat4(1.0f), radians(spinningCubeAngle),
                 vec3(0.0f, 1.0f, 0.0f)) *
          scale(mat4(1.0f), vec3(0.01f, 0.01f, 0.01f));

      setWorldMatrix(colorShaderProgram, mat4(1.0f));
      setViewMatrix(colorShaderProgram, spinningCubeViewMatrix);
    } else {
      // In third person view, let's draw the spinning cube in world space, like
      // any other models
      mat4 spinningCubeWorldMatrix =
          translate(mat4(1.0f), cameraPosition) *
          rotate(mat4(1.0f), radians(spinningCubeAngle),
                 vec3(0.0f, 1.0f, 0.0f)) *
          scale(mat4(1.0f), vec3(0.1f, 0.1f, 0.1f));

      setWorldMatrix(colorShaderProgram, spinningCubeWorldMatrix);
    }
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // End Frame
    glfwSwapBuffers(window);
    glfwPollEvents();

    // Handle inputs
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
      glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) // move camera down
    {
      cameraFirstPerson = true;
    }

    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) // move camera down
    {
      cameraFirstPerson = false;
    }

    // This was solution for Lab02 - Moving camera exercise
    // We'll change this to be a first or third person camera
    bool fastCam = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS ||
                   glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS;
    float currentCameraSpeed = (fastCam) ? cameraFastSpeed : cameraSpeed;

    // - Calculate mouse motion dx and dy
    // - Update camera horizontal and vertical angle
    double mousePosX, mousePosY;
    glfwGetCursorPos(window, &mousePosX, &mousePosY);

    double dx = mousePosX - lastMousePosX;
    double dy = mousePosY - lastMousePosY;

    lastMousePosX = mousePosX;
    lastMousePosY = mousePosY;

    // Convert to spherical coordinates
    const float cameraAngularSpeed = 60.0f;
    cameraHorizontalAngle -= dx * cameraAngularSpeed * dt;
    cameraVerticalAngle -= dy * cameraAngularSpeed * dt;

    // Clamp vertical angle to [-85, 85] degrees
    cameraVerticalAngle =
        std::max(-85.0f, std::min(85.0f, cameraVerticalAngle));

    float theta = radians(cameraHorizontalAngle);
    float phi = radians(cameraVerticalAngle);

    cameraLookAt =
        vec3(cosf(phi) * cosf(theta), sinf(phi), -cosf(phi) * sinf(theta));
    vec3 cameraSideVector = glm::cross(cameraLookAt, vec3(0.0f, 1.0f, 0.0f));

    cameraSideVector = glm::normalize(cameraSideVector);

    // Use camera lookat and side vectors to update positions with ASDW
    if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS) {
      mat4 viewMatrix1 = glm::mat4(1.0f);

      setViewMatrix(colorShaderProgram, viewMatrix1);
      setViewMatrix(texturedShaderProgram, viewMatrix1);
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
      cameraPosition += cameraLookAt * dt * currentCameraSpeed;
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
      cameraPosition -= cameraLookAt * dt * currentCameraSpeed;
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
      cameraPosition += cameraSideVector * dt * currentCameraSpeed;
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
      cameraPosition -= cameraSideVector * dt * currentCameraSpeed;
    }

    // Set the view matrix for first and third person cameras
    // - In first person, camera lookat is set like below
    // - In third person, camera position is on a sphere looking towards center
    mat4 viewMatrix(1.0f);

    if (cameraFirstPerson) {
      viewMatrix =
          lookAt(cameraPosition, cameraPosition + cameraLookAt, cameraUp);
    } else {
      // Position of the camera is on the sphere looking at the point of
      // interest (cameraPosition)
      float radius = 5.0f;
      vec3 position = cameraPosition - vec3(radius * cosf(phi) * cosf(theta),
                                            radius * sinf(phi),
                                            -radius * cosf(phi) * sinf(theta));
      ;
      viewMatrix = lookAt(position, cameraPosition, cameraUp);
    }

    setViewMatrix(colorShaderProgram, viewMatrix);
    setViewMatrix(texturedShaderProgram, viewMatrix);

    // Shoot projectiles on mouse left click
    // To detect onPress events, we need to check the last state and the current
    // state to detect the state change Otherwise, you would shoot many
    // projectiles on each mouse press
  }

  glfwTerminate();

  return 0;
}

const char *getVertexShaderSource() {
  // For now, you use a string for your shader code, in the assignment, shaders
  // will be stored in .glsl files
  return "#version 330 core\n"
         "layout (location = 0) in vec3 aPos;"
         "layout (location = 1) in vec3 aColor;"
         ""
         "uniform mat4 worldMatrix;"
         "uniform mat4 viewMatrix = mat4(1.0);" // default value for view matrix
                                                // (identity)
         "uniform mat4 projectionMatrix = mat4(1.0);"
         ""
         "out vec3 vertexColor;"
         "void main()"
         "{"
         "   vertexColor = aColor;"
         "   mat4 modelViewProjection = projectionMatrix * viewMatrix * "
         "worldMatrix;"
         "   gl_Position = modelViewProjection * vec4(aPos.x, aPos.y, aPos.z, "
         "1.0);"
         "}";
}

const char *getFragmentShaderSource() {
  return "#version 330 core\n"
         "in vec3 vertexColor;"
         "out vec4 FragColor;"
         "void main()"
         "{"
         "   FragColor = vec4(vertexColor.r, vertexColor.g, vertexColor.b, "
         "1.0f);"
         "}";
}

const char *getTexturedVertexShaderSource() {
  return "#version 330 core\n"
         "layout (location = 0) in vec3 aPos;"
         "layout (location = 1) in vec3 aColor;"
         "layout (location = 2) in vec2 aUV;"
         ""
         "uniform mat4 worldMatrix;"
         "uniform mat4 viewMatrix = mat4(1.0);" // default value for view matrix
                                                // (identity)
         "uniform mat4 projectionMatrix = mat4(1.0);"
         ""
         "out vec3 vertexColor;"
         "out vec2 vertexUV;"
         ""
         "void main()"
         "{"
         "   vertexColor = aColor;"
         "   mat4 modelViewProjection = projectionMatrix * viewMatrix * "
         "worldMatrix;"
         "   gl_Position = modelViewProjection * vec4(aPos.x, aPos.y, aPos.z, "
         "1.0);"
         "   vertexUV = aUV;"
         "}";
}

const char *getTexturedFragmentShaderSource() {
  return "#version 330 core\n"
         "in vec3 vertexColor;"
         "in vec2 vertexUV;"
         "uniform sampler2D textureSampler;"
         ""
         "out vec4 FragColor;"
         "void main()"
         "{"
         "   vec4 textureColor = texture(    textureSampler, vertexUV    );"
         "   FragColor = textureColor;"
         "}";
}

int compileAndLinkShaders(const char *vertexShaderSource,
                          const char *fragmentShaderSource) {
  // compile and link shader program
  // return shader program id
  // ------------------------------------

  // vertex shader
  int vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);

  // check for shader compile errors
  int success;
  char infoLog[512];
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
              << infoLog << std::endl;
  }

  // fragment shader
  int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
  glCompileShader(fragmentShader);

  // check for shader compile errors
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
              << infoLog << std::endl;
  }

  // link shaders
  int shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  // check for linking errors
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
              << infoLog << std::endl;
  }

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  return shaderProgram;
}

GLuint loadTexture(const char *filename) {
  // Step 1 load textures with dimensional data
  int width, height, nrChannels;
  unsigned char *data = stbi_load(filename, &width, &height, &nrChannels, 0);
  if (!data) {
  std:
    cerr << "Error::Texture could not load texture files" << filename
         << std::endl;
    return 0;
  }

  // Step 2 Create and bind textures
  GLuint textureId = 0;
  glGenTextures(1, &textureId);
  assert(textureId != 0);

  glBindTexture(GL_TEXTURE_2D, textureId);

  // Step 3 set filter parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // Step 4 Upload textures to fv
  GLenum format = 0;
  if (nrChannels == 1)
    format = GL_RED;
  else if (nrChannels == 3)
    format = GL_RGB;
  else if (nrChannels == 4)
    format = GL_RGBA;
  glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format,
               GL_UNSIGNED_BYTE, data);

  // Step 5 Free resources
  stbi_image_free(data);
  glBindTexture(GL_TEXTURE, 0);
  return textureId;
};

int createTexturedCubeVertexArrayObject() {
  // Create a vertex array
  GLuint vertexArrayObject;
  glGenVertexArrays(1, &vertexArrayObject);
  glBindVertexArray(vertexArrayObject);

  // Upload Vertex Buffer to the GPU, keep a reference to it
  // (vertexBufferObject)
  GLuint vertexBufferObject;
  glGenBuffers(1, &vertexBufferObject);
  glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
  glBufferData(GL_ARRAY_BUFFER, sizeof(texturedCubeVertexArray),
               texturedCubeVertexArray, GL_STATIC_DRAW);

  glVertexAttribPointer(
      0,        // attribute 0 matches aPos in Vertex Shader
      3,        // size
      GL_FLOAT, // type
      GL_FALSE, // normalized?
      sizeof(TexturedColoredVertex), // stride - each vertex contain 2 vec3
                                     // (position, color)
      (void *)0                      // array buffer offset
  );
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(
      1, // attribute 1 matches aColor in Vertex Shader
      3, GL_FLOAT, GL_FALSE, sizeof(TexturedColoredVertex),
      (void *)sizeof(vec3) // color is offseted a vec3 (comes after position)
  );
  glEnableVertexAttribArray(1);

  glVertexAttribPointer(
      2, // attribute 2 matches aUV in Vertex Shader
      2, GL_FLOAT, GL_FALSE, sizeof(TexturedColoredVertex),
      (void *)(2 * sizeof(vec3)) // uv is offseted by 2 vec3 (comes after
                                 // position and color)
  );
  glEnableVertexAttribArray(2);

  return vertexArrayObject;
}

