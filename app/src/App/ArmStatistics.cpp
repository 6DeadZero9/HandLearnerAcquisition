#include <cmath>
#include <iomanip>

#include <raygui.h>

#include <ArmStatistics.h>
#include <Hand.h>

ArmStatistics::ArmStatistics(float x, float y, float width, float height)
    : Component(x, y, width, height) {
    arm_image = LoadTextureFromImage(LoadImage("./app/assets/images/arm.png"));
}

void ArmStatistics::draw(int screen_width, int screen_height) {
    std::lock_guard<std::mutex> lock(HAND_INSTANCE.hand_usage_mutex);

    // --- Layout base ---
    float box_width = screen_width * width;
    float box_height = screen_height * height;
    float line_thickness = box_width * 0.0025f;
    float texture_ratio = (float)arm_image.width / (float)arm_image.height;
    float target_ratio = box_width / box_height;

    Color outlines = GetColor(0xFD013AFF);
    Font font = GetFontDefault();
    Rectangle general_statistics { x + box_width * 0.05f, y + box_height * 0.65f, box_width * 0.35f, box_height * 0.35f };
    Rectangle stats_box { x * screen_width, y * screen_height, box_width, box_height };
    Rectangle img_box = stats_box;

    GeneralHandLearnerData general_data = HAND_INSTANCE.get_general_data();

    // --- Fit image ---
    if (texture_ratio > target_ratio) {
        img_box.height = box_width / texture_ratio;
        img_box.y += (box_height - img_box.height) / 2;
    } else {
        img_box.width = box_height * texture_ratio;
        img_box.x += (box_width - img_box.width) / 2;
    }

    // --- Vertical padding ---
    img_box.y += box_height * 0.01f;
    img_box.height -= box_height * 0.02f;

    std::ostringstream general_data_output;
    general_data_output << "ACCEL_DATA:"
                        << "\n\tX:" << std::fixed << std::setprecision(2) << general_data.accel_data.x << "\n\tY:" << std::fixed
                        << std::setprecision(2) << general_data.accel_data.y << "\n\tZ:" << std::fixed << std::setprecision(2)
                        << general_data.accel_data.z << "\n\nGYRO_DATA:" << "\n\tX:" << std::fixed << std::setprecision(2)
                        << general_data.gyro_data.x << "\n\tY:" << std::fixed << std::setprecision(2) << general_data.gyro_data.y
                        << "\n\tZ:" << std::fixed << std::setprecision(2) << general_data.gyro_data.z << "\n\nIMU_TEMP: " << std::fixed
                        << std::setprecision(2) << general_data.temp_data << "°C" << "\n\nFPS: " << GetFPS();

    GuiGroupBox(stats_box, "Arm Statistics");
    DrawTexturePro(arm_image, { 0, 0, arm_image.width, arm_image.height }, img_box, { 0, 0 }, 0, WHITE);
    DrawRectangleLinesEx(general_statistics, line_thickness, outlines);
    DrawTextEx(font,
               general_data_output.str().c_str(),
               { general_statistics.x + general_statistics.width * 0.05f, general_statistics.y + general_statistics.height * 0.05f },
               15,
               1,
               outlines);

    // --- Helper: format angle ---
    auto format_angle = [](float radians) {
        std::ostringstream ss;
        ss << std::fixed << std::setprecision(2) << radians * (180.0f / M_PI);
        return ss.str();
    };

    // --- Helper: draw connectors ---
    auto draw_connector = [&](bool is_left, Rectangle joint, Rectangle text_box) {
        float joint_mid_y = joint.y + joint.height / 2;
        float text_mid_y = text_box.y + text_box.height / 2;
        float offset = joint.width / 2;

        if (is_left) {
            DrawLineEx({ joint.x, joint_mid_y }, { joint.x - offset, joint_mid_y }, line_thickness, outlines);
            DrawLineEx({ text_box.x + text_box.width, text_mid_y },
                       { text_box.x + text_box.width + offset, text_mid_y },
                       line_thickness,
                       outlines);
            DrawLineEx({ joint.x - offset, joint_mid_y }, { text_box.x + text_box.width + offset, text_mid_y }, line_thickness, outlines);
        } else {
            DrawLineEx({ joint.x + joint.width, joint_mid_y }, { joint.x + joint.width + offset, joint_mid_y }, line_thickness, outlines);
            DrawLineEx({ text_box.x, text_mid_y }, { text_box.x - offset, text_mid_y }, line_thickness, outlines);
            DrawLineEx({ joint.x + joint.width + offset, joint_mid_y }, { text_box.x - offset, text_mid_y }, line_thickness, outlines);
        }
    };

    // --- Loop through hand parts ---
    for (const auto& hand_part : HAND_INSTANCE) {
        float rect_size = RECTANGLE_POINTER_SIZE + img_box.width * 0.02f;
        GUIData gui = hand_part.get_gui_data();
        std::string display_text = hand_part.get_name() + ": " + format_angle(hand_part.get_angle()) + "°";
        Vector2 text_size = MeasureTextEx(font, display_text.c_str(), 15, 1);

        // --- Joint rectangle ---
        Rectangle joint_box { img_box.x + img_box.width * gui.box_pointer.x - rect_size / 2,
                              img_box.y + img_box.height * gui.box_pointer.y - rect_size / 2,
                              rect_size,
                              rect_size };

        // --- Text box ---
        Rectangle text_box { stats_box.x + box_width * gui.text_pointer.x - text_size.x / 2,
                             stats_box.y + box_height * gui.text_pointer.y - text_size.y / 2,
                             text_size.x,
                             text_size.y };

        // --- Draw text ---
        DrawTextEx(font, display_text.c_str(), { text_box.x, text_box.y }, 15, 1, outlines);

        // --- Expand text border ---
        Rectangle border = text_box;
        border.x -= text_size.x * 0.1f;
        border.y -= text_size.y * 0.15f;
        border.width += text_size.x * 0.2f;
        border.height += text_size.y * 0.3f;

        // --- Draw boxes ---
        DrawRectangleLinesEx(joint_box, line_thickness, outlines);
        DrawRectangleLinesEx(border, line_thickness, outlines);

        // --- Draw connector ---
        draw_connector(gui.alignment == LEFT, joint_box, border);
    }
}
