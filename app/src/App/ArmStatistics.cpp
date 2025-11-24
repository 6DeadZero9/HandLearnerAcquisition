#include <cmath>
#include <iomanip>

#include <raygui.h>

#include <ArmStatistics.h>
#include <Hand.h>
#include <Logger.h>

ArmStatistics::ArmStatistics(float x, float y, float width, float height)
    : Component(x, y, width, height),
      outlines_color(GetColor(0xFD013AFF)),
      font(GetFontDefault()) {

    Image image = LoadImage("./app/assets/images/arm.png");
    arm_image = LoadTextureFromImage(image);
    UnloadImage(image);
    image_ratio = (float)arm_image.width / (float)arm_image.height;
}

void ArmStatistics::draw(int window_width, int window_height, float window_to_screen_ratio) {
    std::lock_guard<std::mutex> lock(HAND_INSTANCE.hand_usage_mutex);

    // --- Layout base ---
    float box_width = window_width * width;
    float box_height = window_height * height;
    float line_thickness = MIN_LINE_THICKNESS_SIZE + MAX_LINE_THICKNESS_SIZE * window_to_screen_ratio;
    float statistics_text_size = MIN_ARM_STATISTICS_TEXT_SIZE + MAX_ARM_STATISTICS_TEXT_SIZE * window_to_screen_ratio;
    float target_ratio = box_width / box_height;

    Rectangle arm_statistics_box { x * window_width, y * window_height, box_width, box_height };
    Rectangle img_box = arm_statistics_box;

    GeneralHandLearnerData general_data = HAND_INSTANCE.get_general_data();

    // --- Fit image ---
    if (image_ratio > target_ratio) {
        img_box.height = box_width / image_ratio;
        img_box.y += (box_height - img_box.height) / 2;
    } else {
        img_box.width = box_height * image_ratio;
        img_box.x += (box_width - img_box.width) / 2;
    }

    // --- Vertical padding ---
    img_box.y += box_height * ARM_IMAGE_BOX_PADDING_SCALE;
    img_box.height -= box_height * (ARM_IMAGE_BOX_PADDING_SCALE * 2);

    std::ostringstream general_data_output;
    general_data_output << "ACCEL_DATA:"
                        << "\n\tX:" << std::fixed << std::setprecision(2) << general_data.accel_data.x << "\n\tY:" << std::fixed
                        << std::setprecision(2) << general_data.accel_data.y << "\n\tZ:" << std::fixed << std::setprecision(2)
                        << general_data.accel_data.z << "\n\nGYRO_DATA:" << "\n\tX:" << std::fixed << std::setprecision(2)
                        << general_data.gyro_data.x << "\n\tY:" << std::fixed << std::setprecision(2) << general_data.gyro_data.y
                        << "\n\tZ:" << std::fixed << std::setprecision(2) << general_data.gyro_data.z << "\n\nIMU_TEMP: " << std::fixed
                        << std::setprecision(2) << general_data.temp_data << "°C" << "\n\nFPS: " << GetFPS();
    Vector2 general_data_output_text_size = MeasureTextEx(font, general_data_output.str().c_str(), statistics_text_size, 1);
    Rectangle general_statistics_box { arm_statistics_box.x + arm_statistics_box.width * GENERAL_STATISTICS_X_TEST_POSITION_OFFSET,
                                       arm_statistics_box.y + arm_statistics_box.height -
                                           (arm_statistics_box.height * GENERAL_STATISTICS_Y_TEST_POSITION_OFFSET +
                                            general_data_output_text_size.y),
                                       general_data_output_text_size.x,
                                       general_data_output_text_size.y };

    GuiGroupBox(arm_statistics_box, "Arm Statistics");
    DrawTexturePro(arm_image, { 0, 0, (float)arm_image.width, (float)arm_image.height }, img_box, { 0, 0 }, 0, WHITE);
    DrawTextEx(font,
               general_data_output.str().c_str(),
               { general_statistics_box.x, general_statistics_box.y },
               statistics_text_size,
               1,
               outlines_color);

    // --- Expand text border ---
    general_statistics_box.x -= general_data_output_text_size.x * GENERAL_STATISTICS_X_PADDING;
    general_statistics_box.y -= general_data_output_text_size.y * GENERAL_STATISTICS_Y_PADDING;
    general_statistics_box.width += general_data_output_text_size.x * (GENERAL_STATISTICS_X_PADDING * 2);
    general_statistics_box.height += general_data_output_text_size.y * (GENERAL_STATISTICS_Y_PADDING * 2);

    DrawRectangleLinesEx(general_statistics_box, line_thickness, outlines_color);

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
            DrawLineEx({ joint.x, joint_mid_y }, { joint.x - offset, joint_mid_y }, line_thickness, outlines_color);
            DrawLineEx({ text_box.x + text_box.width, text_mid_y },
                       { text_box.x + text_box.width + offset, text_mid_y },
                       line_thickness,
                       outlines_color);
            DrawLineEx(
                { joint.x - offset, joint_mid_y }, { text_box.x + text_box.width + offset, text_mid_y }, line_thickness, outlines_color);
        } else {
            DrawLineEx(
                { joint.x + joint.width, joint_mid_y }, { joint.x + joint.width + offset, joint_mid_y }, line_thickness, outlines_color);
            DrawLineEx({ text_box.x, text_mid_y }, { text_box.x - offset, text_mid_y }, line_thickness, outlines_color);
            DrawLineEx(
                { joint.x + joint.width + offset, joint_mid_y }, { text_box.x - offset, text_mid_y }, line_thickness, outlines_color);
        }
    };

    // --- Loop through hand parts ---
    for (const auto& hand_part : HAND_INSTANCE) {
        float rect_size = MIN_RECTANGLE_POINTER_SIZE + MAX_RECTANGLE_POINTER_SIZE * window_to_screen_ratio;
        GUIData gui = hand_part.get_gui_data();
        std::string display_text = hand_part.get_name() + ": " + format_angle(hand_part.get_angle()) + "°";
        Vector2 text_size = MeasureTextEx(font, display_text.c_str(), statistics_text_size, 1);

        // --- Joint rectangle ---
        Rectangle joint_box { img_box.x + img_box.width * gui.box_pointer.x - rect_size / 2,
                              img_box.y + img_box.height * gui.box_pointer.y - rect_size / 2,
                              rect_size,
                              rect_size };

        // --- Text box ---
        Rectangle text_box { arm_statistics_box.x + box_width * gui.text_pointer.x - text_size.x / 2,
                             arm_statistics_box.y + box_height * gui.text_pointer.y - text_size.y / 2,
                             text_size.x,
                             text_size.y };

        // --- Draw text ---
        DrawTextEx(font, display_text.c_str(), { text_box.x, text_box.y }, statistics_text_size, 1, outlines_color);

        // --- Expand text box border ---
        text_box.x -= text_size.x * ANGLE_TEXT_BOX_X_PADDING;
        text_box.y -= text_size.y * ANGLE_TEXT_BOX_Y_PADDING;
        text_box.width += text_size.x * (ANGLE_TEXT_BOX_X_PADDING * 2);
        text_box.height += text_size.y * (ANGLE_TEXT_BOX_Y_PADDING * 2);

        // --- Draw boxes ---
        DrawRectangleLinesEx(joint_box, line_thickness, outlines_color);
        DrawRectangleLinesEx(text_box, line_thickness, outlines_color);

        // --- Draw connector ---
        draw_connector(gui.alignment == LEFT, joint_box, text_box);
    }
}
