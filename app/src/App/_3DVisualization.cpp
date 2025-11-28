#include <raygui.h>
#include <raymath.h>
#include <rlgl.h>

#include <_3DVisualization.h>

_3DVisualization::_3DVisualization(float x, float y, float width, float height)
    : Component(x, y, width, height),
      camera((Vector3) { 0.0f, 5.0f, 10.0f }, // Camera position
             (Vector3) { 0.0f, 0.0f, 0.0f },  // Camera looking at point
             (Vector3) { 0.0f, 1.0f, 0.0f },  // Camera up vector (rotation towards target)
             45.0f,                           // Camera field-of-view Y
             CAMERA_PERSPECTIVE) {}

void _3DVisualization::draw(int window_width, int window_height, float window_to_screen_ratio) {
    float box_width = window_width * width;
    float box_height = window_height * height;
    Rectangle _3d_visualization_box { x * window_width, y * window_height, box_width, box_height };

    BeginScissorMode(_3d_visualization_box.x, _3d_visualization_box.y, _3d_visualization_box.width, _3d_visualization_box.height);
    {
        rlViewport(_3d_visualization_box.x, _3d_visualization_box.y, _3d_visualization_box.width, _3d_visualization_box.height);
        BeginMode3D(camera);
        {
            rlSetMatrixProjection(MatrixPerspective(camera.fovy * DEG2RAD, box_width / box_height, 0.01f, 1000.0f));
            DrawCube((Vector3) { 0, 0, 0 }, 1, 1, 1, RED);
            DrawGrid(20, 2.0f);
        }
        EndMode3D();

        rlMatrixMode(RL_PROJECTION);                              // Enable internal projection matrix
        rlLoadIdentity();                                         // Reset internal projection matrix
        rlOrtho(0.0, window_width, window_height, 0.0, 0.0, 1.0); // Recalculate internal projection matrix
        rlMatrixMode(RL_MODELVIEW);                               // Enable internal modelview matrix
        rlLoadIdentity();
    }
    EndScissorMode();

    rlViewport(0, 0, window_width, window_height);

    GuiGroupBox(_3d_visualization_box, "3D Visualization");
}