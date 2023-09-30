#include "raylib.h"
#include "rts.h"

#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

rts_t *rts;

void Init(void);
void UnInit(void);
void UpdateDrawFrame(void); // Update and Draw one frame

float map(float x, float in_min, float in_max, float out_min, float out_max);
Vector2 GetAdjustedMousePos();

const int canvasWidth = 400;
const int canvasHeight = 225;
const float aspect_ratio = (float)canvasWidth / canvasHeight;

const int scale = 2;
const int screenWidth = canvasWidth * scale;
const int screenHeight = canvasHeight * scale;

bool show_fps = true;

RenderTexture2D renderTexture;
Rectangle source = { 0, 0, (float)canvasWidth, (float)-canvasHeight };

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void) {
    // Initialization
    //--------------------------------------------------------------------------------------
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);

    InitWindow(screenWidth, screenHeight, "Holo RTS");

    SetTargetFPS(60);

    renderTexture = LoadRenderTexture(canvasWidth, canvasHeight);

    // Initialization
    //--------------------------------------------------------------------------------------
    Init();
    //--------------------------------------------------------------------------------------

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(UpdateDrawFrame, 0, 1);
#else
    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        UpdateDrawFrame();
    }
#endif

    // De-Initialization
    //--------------------------------------------------------------------------------------
    // UnloadTexture(texture);       // Texture unloading
    UnloadRenderTexture(renderTexture);

    UnInit();

    CloseWindow(); // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

void Init(void) { rts = rts_new(); }

void UnInit(void) { rts_free(rts); }

void UpdateDrawFrame(void) { // Update and Draw one frame
    // Update
    //----------------------------------------------------------------------------------
    rts->mouse_pos = GetAdjustedMousePos();
    rts_input(rts);
    rts_update(rts);

    //----------------------------------------------------------------------------------

    // Draw to render texture
    //----------------------------------------------------------------------------------
    BeginTextureMode(renderTexture);
    {
        ClearBackground(BLACK);
        rts_draw(rts);
    }
    EndTextureMode();

    // Draw render texture to screen
    //----------------------------------------------------------------------------------
    BeginDrawing();
    {
        ClearBackground(BLACK);

        bool wide =
            ((float)GetScreenWidth() / GetScreenHeight()) < aspect_ratio;

        float adjusted_width =
            wide ? GetScreenWidth() : (float)GetScreenHeight() * aspect_ratio;
        float adjusted_height =
            !wide ? GetScreenHeight() : (float)GetScreenWidth() / aspect_ratio;

        float adjusted_x =
            !wide ? (float)(GetScreenWidth() - adjusted_width) / 2 : 0;
        float adjusted_y =
            wide ? (float)(GetScreenHeight() - adjusted_height) / 2 : 0;

        Rectangle dest = { adjusted_x, adjusted_y, adjusted_width,
                           adjusted_height };
        DrawTexturePro(renderTexture.texture, source, dest, (Vector2){ 0, 0 },
                       0.0f, WHITE);

        if (show_fps)
            DrawFPS(16, 16);
    }
    EndDrawing();
    //----------------------------------------------------------------------------------
}

float map(float x, float in_min, float in_max, float out_min, float out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

Vector2 GetAdjustedMousePos() {
    bool wide = ((float)GetScreenWidth() / GetScreenHeight()) < aspect_ratio;

    float adjusted_width =
        wide ? GetScreenWidth() : (float)GetScreenHeight() * aspect_ratio;
    float adjusted_height =
        !wide ? GetScreenHeight() : (float)GetScreenWidth() / aspect_ratio;

    float subtracted_w = GetScreenWidth() - adjusted_width;
    float start_w = subtracted_w / 2;
    float end_w = start_w + adjusted_width;
    float mx = (float)GetMouseX();
    if (mx < start_w) {
        mx = start_w;
    }
    if (mx > end_w) {
        mx = end_w;
    }
    mx = map(mx, start_w, end_w, 0, adjusted_width);

    float subtracted_h = GetScreenHeight() - adjusted_height;
    float start_h = subtracted_h / 2;
    float end_h = start_h + adjusted_height;
    float my = (float)GetMouseY();
    if (my < start_h) {
        my = start_h;
    }
    if (my > end_h) {
        my = end_h;
    }
    my = map(my, start_h, end_h, 0, adjusted_height);

    return (Vector2){ mx / (adjusted_width / canvasWidth),
                      my / (adjusted_height / canvasHeight) };
}
