#include <iostream>
#include <ostream>
#include <raygui.h>

#include <ArmStatistics.h>
#include <Hand.h>

ArmStatistics::ArmStatistics(float x, float y, float width, float height)
    : Component(x, y, width, height) {
    arm_image = LoadTextureFromImage(LoadImage("./app/assets/arm.png"));
}

void ArmStatistics::draw(int screen_width, int screen_height) {
    float arm_image_box_width = screen_width * width;
    float arm_image_box_height = screen_height * height;
    float texture_ratio = (float)arm_image.width / (float)arm_image.height;
    float target_ratio = (arm_image_box_width) / (arm_image_box_height);
    Color outlines = GetColor(0xfd013aff);

    Rectangle arm_statistics_box { x * screen_width, y * screen_height, arm_image_box_width, arm_image_box_height };
    Rectangle arm_statistics_image_box { x * screen_width, y * screen_height, arm_image_box_width, arm_image_box_height };

    if (texture_ratio > target_ratio) {
        arm_statistics_image_box.height = arm_image_box_width / texture_ratio;
        arm_statistics_image_box.y += (arm_image_box_height - arm_statistics_image_box.height) / 2;
    } else {
        arm_statistics_image_box.width = arm_image_box_height * texture_ratio;
        arm_statistics_image_box.x += (arm_image_box_width - arm_statistics_image_box.width) / 2;
    }

    arm_statistics_image_box.y += arm_image_box_height * 0.01f;
    arm_statistics_image_box.height -= arm_image_box_height * 0.02f;

    GuiGroupBox(arm_statistics_box, "Arm Statistics");
    DrawTexturePro(arm_image, { 0, 0, arm_image.width, arm_image.height }, arm_statistics_image_box, { 0, 0 }, 0, WHITE);

    // if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
    //     Vector2 lastMousePosition = GetMousePosition();

    //     std::cout << "X: " << (lastMousePosition.x - arm_statistics_box.x) / arm_statistics_box.width
    //               << " Y: " << (lastMousePosition.y - arm_statistics_box.y) / arm_statistics_box.height << std::endl;
    // }

    for (const auto hand_part : HAND_INSTANCE) {
        const std::string current_hand_part_name = hand_part.get_name();
        const GUIData current_hand_part_data = hand_part.get_gui_data();
        const float rectangle_size = RECTANGLE_POINTER_SIZE + arm_statistics_image_box.width * 0.02f;

        const Rectangle joint_box {
            arm_statistics_image_box.x + arm_statistics_image_box.width * current_hand_part_data.box_pointer.x - rectangle_size / 2,
            arm_statistics_image_box.y + arm_statistics_image_box.height * current_hand_part_data.box_pointer.y - rectangle_size / 2,
            rectangle_size,
            rectangle_size
        };

        const Rectangle text_box {
            arm_statistics_box.x + arm_statistics_box.width * current_hand_part_data.text_pointer.x - rectangle_size / 2,
            arm_statistics_box.y + arm_statistics_box.height * current_hand_part_data.text_pointer.y - rectangle_size / 2,
            rectangle_size * 3,
            rectangle_size
        };

        const float line_thickness = arm_statistics_image_box.width * 0.005f;

        DrawRectangleLinesEx(joint_box, line_thickness, outlines);
        DrawRectangleLinesEx(text_box, line_thickness, outlines);

        switch (current_hand_part_data.alignment) {
            case LEFT:
                DrawLineEx((Vector2) { joint_box.x, joint_box.y + joint_box.height / 2 },
                           (Vector2) { joint_box.x - joint_box.width / 2, joint_box.y + joint_box.height / 2 },
                           line_thickness,
                           outlines);
                DrawLineEx((Vector2) { text_box.x + text_box.width, text_box.y + text_box.height / 2 },
                           (Vector2) { text_box.x + text_box.width + joint_box.width / 2, text_box.y + text_box.height / 2 },
                           line_thickness,
                           outlines);
                DrawLineEx((Vector2) { joint_box.x - joint_box.width / 2, joint_box.y + joint_box.height / 2 },
                           (Vector2) { text_box.x + text_box.width + joint_box.width / 2, text_box.y + text_box.height / 2 },
                           line_thickness,
                           outlines);

                break;
            case RIGHT:
                DrawLineEx((Vector2) { joint_box.x + joint_box.width, joint_box.y + joint_box.height / 2 },
                           (Vector2) { joint_box.x + joint_box.width + joint_box.width / 2, joint_box.y + joint_box.height / 2 },
                           line_thickness,
                           outlines);
                DrawLineEx((Vector2) { text_box.x, text_box.y + text_box.height / 2 },
                           (Vector2) { text_box.x - joint_box.width / 2, text_box.y + text_box.height / 2 },
                           line_thickness,
                           outlines);
                DrawLineEx((Vector2) { joint_box.x + joint_box.width + joint_box.width / 2, joint_box.y + joint_box.height / 2 },
                           (Vector2) { text_box.x - joint_box.width / 2, text_box.y + text_box.height / 2 },
                           line_thickness,
                           outlines);
                break;
            default:
                break;
        }
    }
}