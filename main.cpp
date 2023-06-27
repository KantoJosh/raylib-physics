/*******************************************************************************************
 *
 *   raylib [core] example - Initialize 3d camera free
 *
 *   Example originally created with raylib 1.3, last time updated with
 *raylib 1.3
 *
 *   Example licensed under an unmodified zlib/libpng license, which is an
 *OSI-certified, BSD-like license that allows static linking with closed source
 *software
 *
 *   Copyright (c) 2015-2023 Ramon Santamaria (@raysan5)
 *
 ********************************************************************************************/

// clang-format off
#include <memory>
#include <string>
#include <stdexcept>

#include "raylib.h"
#include "physics.h"
#include "raymath.h"
// clang-format on
#include <iostream>

template <typename... Args>
std::string string_format(const std::string &format, Args... args)
{
  int size_s = std::snprintf(nullptr, 0, format.c_str(), args...) + 1; // Extra space for '\0'
  if (size_s <= 0)
  {
    throw std::runtime_error("Error during formatting.");
  }
  auto size = static_cast<size_t>(size_s);
  std::unique_ptr<char[]> buf(new char[size]);
  std::snprintf(buf.get(), size, format.c_str(), args...);
  return std::string(buf.get(), buf.get() + size - 1); // We don't want the '\0' inside
}

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
  // Initialization
  //--------------------------------------------------------------------------------------
  const int screenWidth = 1000;
  const int screenHeight = 1000;

  Scene physicsScene;
  physicsScene.Initialize();

  InitWindow(screenWidth, screenHeight,
             "raylib [core] example - 3d camera free");

  // Define the camera to look into our 3d world
  Camera3D camera = {0};
  camera.position = (Vector3){10.0f, 10.0f, 10.0f}; // Camera position
  camera.target = (Vector3){0.0f, 0.0f, 0.0f};      // Camera looking at point
  camera.up =
      (Vector3){0.0f, 1.0f, 0.0f};        // Camera up vector (rotation towards target)
  camera.fovy = 45.0f;                    // Camera field-of-view Y
  camera.projection = CAMERA_PERSPECTIVE; // Camera projection type

  //   Vector3 cubePosition = {0.0f, 0.0f, 0.0f};

  DisableCursor(); // Limit cursor to relative movement inside the window

  SetTargetFPS(60); // Set our game to run at 60 frames-per-second
  //--------------------------------------------------------------------------------------

  // Main game loop
  while (!WindowShouldClose()) // Detect window close button or ESC key
  {
    // Update
    //----------------------------------------------------------------------------------
    UpdateCamera(&camera, CAMERA_FREE);
    physicsScene.Update(1.0f / 60);

    if (IsKeyDown('Z'))
      camera.target = (Vector3){0.0f, 0.0f, 0.0f};
    //----------------------------------------------------------------------------------

    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();

    ClearBackground(RAYWHITE);

    BeginMode3D(camera);

    // DrawCube(cubePosition, 2.0f, 2.0f, 2.0f, RED);
    // DrawCubeWires(cubePosition, 2.0f, 2.0f, 2.0f, MAROON);
    Color colors[] = {RED, BLUE, GREEN};
    int colorsLength = 3;
    int colorIndex = 0;

    int Xpos = 150;
    int Ypos = 150;
    for (auto &body : physicsScene.bodies)
    {
      if (body.shape->GetType() == Shape::SPHERE)
      {
        Sphere *sphereShape = dynamic_cast<Sphere *>(body.shape);
        if (sphereShape != nullptr)
        {
          DrawSphere(body.position, sphereShape->radius, colors[colorIndex]);
          DrawSphereWires(body.position, 2.0f, 2.0f, 2.0f, colors[colorIndex]);
          colorIndex = (colorIndex + 1) % colorsLength;
        }
      }
    }
    // DrawSphere({0, 0, 0}, 5.0f, RED);
    // DrawSphereWires()

    DrawGrid(10, 1.0f);

    EndMode3D();

    DrawRectangle(10, 10, 320, 133, Fade(SKYBLUE, 0.5f));
    DrawRectangleLines(10, 10, 320, 133, BLUE);

    DrawText("Free camera default controls:", 20, 20, 10, BLACK);
    DrawText("- Mouse Wheel to Zoom in-out", 40, 40, 10, DARKGRAY);
    DrawText("- Mouse Wheel Pressed to Pan", 40, 60, 10, DARKGRAY);
    DrawText("- Alt + Mouse Wheel Pressed to Rotate", 40, 80, 10, DARKGRAY);
    DrawText("- Alt + Ctrl + Mouse Wheel Pressed for Smooth Zoom", 40, 100, 10,
             DARKGRAY);
    DrawText("- Z to zoom to (0, 0, 0)", 40, 120, 10, DARKGRAY);
    for (auto &body : physicsScene.bodies)
    {
      Sphere *sphereShape = dynamic_cast<Sphere *>(body.shape);
      if (sphereShape != nullptr)
      {
        std::string position = string_format("Position: <X:%.1f, Y:%.1f, Z:%.1f>", body.position.x, body.position.y, body.position.z);
        DrawText(position.c_str(), Xpos, Ypos, 20, BLACK);

        std::string velocity = string_format("Velocity: <X:%.1f, Y:%.1f, Z:%.1f>", body.linearVelocity.x, body.linearVelocity.y, body.linearVelocity.z);
        DrawText(velocity.c_str(), Xpos, Ypos + 30, 20, BLACK);
        Ypos += 60;
      }
    }

    EndDrawing();
    //----------------------------------------------------------------------------------
  }

  // De-Initialization
  //--------------------------------------------------------------------------------------
  CloseWindow(); // Close window and OpenGL context
  //--------------------------------------------------------------------------------------

  return 0;
}
