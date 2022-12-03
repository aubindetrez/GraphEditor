#include "raylib.h"

// Draw text using font inside rectangle limits
static void DrawTextBoxed(Font font, const char *text, Rectangle rec, float fontSize, float spacing,
        bool wordWrap, Color tint);
// Draw text using font inside rectangle limits with support for text selection
static void DrawTextBoxedSelectable(Font font, const char *text, Rectangle rec, float fontSize,
        float spacing, bool wordWrap, Color tint, int selectStart, int selectLength,
        Color selectTint, Color selectBackTint);

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;
    const int handleSize = 14;
    const int boxLineWidth = 3;
    const int boxLeftMargin = 50;
    const int boxRigthMargin = 50;
    const int boxUpMargin = 50;
    const int boxDownMargin = 50;

    InitWindow(screenWidth, screenHeight, "Diagram editor");

    const char text[] = "Here goes your text";

    bool resizing = false;
    bool wordWrap = true;

    Rectangle container = { 25.0f, 25.0f, screenWidth - 50.0f, screenHeight - 250.0f };
    Rectangle resizer = { container.x + container.width - handleSize - boxLineWidth, container.y + container.height - handleSize - boxLineWidth, handleSize, handleSize };
    Rectangle mover = { container.x, container.y, handleSize, handleSize };

    // Minimum width and heigh for the container rectangle
    const float minWidth = 60;
    const float minHeight = 60;
    const float maxWidth = screenWidth - 50.0f;
    const float maxHeight = screenHeight - 160.0f;

    Vector2 lastMouse = { 0.0f, 0.0f }; // Stores last mouse coordinates
    Color borderColor = MAROON;         // Container border color
    Font font = GetFontDefault();       // Get default system font

    SetTargetFPS(60);                   // Set our game to run at 60 frames-per-second
                                        //--------------------------------------------------------------------------------------

                                        // Main game loop
    while (!WindowShouldClose())        // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        if (IsKeyPressed(KEY_SPACE)) wordWrap = !wordWrap;

        Vector2 mouse = GetMousePosition();

        // Check if the mouse is inside the container and toggle border color
        if (CheckCollisionPointRec(mouse, container)) borderColor = Fade(MAROON, 0.4f);
        else if (!resizing) borderColor = MAROON;

        // Container resizing logic
        if (resizing)
        {
            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) resizing = false;

            float width = container.width + (mouse.x - lastMouse.x);
            container.width = (width > minWidth)? ((width < maxWidth)? width : maxWidth) : minWidth;

            float height = container.height + (mouse.y - lastMouse.y);
            container.height = (height > minHeight)? ((height < maxHeight)? height : maxHeight) : minHeight;
        }
        else
        {
            // Check if we're resizing
            if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mouse, resizer)) resizing = true;
        }

        // Move resizer rectangle properly
        resizer.x = container.x + container.width - handleSize - boxLineWidth;
        resizer.y = container.y + container.height - handleSize - boxLineWidth;

        // Move mover rectangle properly
        mover.x = container.x;
        mover.y = container.y;

        lastMouse = mouse; // Update mouse
                           //----------------------------------------------------------------------------------

                           // Draw
                           //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(RAYWHITE);

        DrawRectangleLinesEx(container, boxLineWidth, borderColor);    // Draw container border

        // Draw text in container (add some padding)
        DrawTextBoxed(font, text, (Rectangle){
                    container.x + boxLineWidth + boxLeftMargin,
                    container.y + boxLineWidth + boxUpMargin,
                    container.width - boxLineWidth - boxRigthMargin,
                    container.height - boxLineWidth - boxDownMargin
                }, 20.0f, 2.0f, wordWrap, GRAY);

        DrawRectangleRec(resizer, borderColor);             // Draw the resize box
        DrawRectangleRec(mover, borderColor);             // Draw the move box

        // Draw bottom info
        //DrawRectangle(0, screenHeight - 54, screenWidth, 54, GRAY);
        //DrawRectangleRec((Rectangle){ 382.0f, screenHeight - 34.0f, 12.0f, 12.0f }, MAROON);

        //DrawText("Word Wrap: ", 313, screenHeight-115, 20, BLACK);
        //if (wordWrap) DrawText("ON", 447, screenHeight - 115, 20, RED);
        //else DrawText("OFF", 447, screenHeight - 115, 20, BLACK);

        //DrawText("Press [SPACE] to toggle word wrap", 218, screenHeight - 86, 20, GRAY);

        //DrawText("Click hold & drag the    to resize the container", 155, screenHeight - 38, 20, RAYWHITE);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
                          //--------------------------------------------------------------------------------------

    return 0;
}

//--------------------------------------------------------------------------------------
// Module functions definition
//--------------------------------------------------------------------------------------

// Draw text using font inside rectangle limits
static void DrawTextBoxed(Font font, const char *text, Rectangle rec, float fontSize, float spacing, bool wordWrap, Color tint)
{
    DrawTextBoxedSelectable(font, text, rec, fontSize, spacing, wordWrap, tint, 0, 0, WHITE, WHITE);
}

// Draw text using font inside rectangle limits with support for text selection
static void DrawTextBoxedSelectable(Font font, const char *text, Rectangle rec, float fontSize, float spacing, bool wordWrap, Color tint, int selectStart, int selectLength, Color selectTint, Color selectBackTint)
{
    int length = TextLength(text);  // Total length in bytes of the text, scanned by codepoints in loop

    float textOffsetY = 0;          // Offset between lines (on line break '\n')
    float textOffsetX = 0.0f;       // Offset X to next character to draw

    float scaleFactor = fontSize/(float)font.baseSize;     // Character rectangle scaling factor

    // Word/character wrapping mechanism variables
    enum { MEASURE_STATE = 0, DRAW_STATE = 1 };
    int state = wordWrap? MEASURE_STATE : DRAW_STATE;

    int startLine = -1;         // Index where to begin drawing (where a line begins)
    int endLine = -1;           // Index where to stop drawing (where a line ends)
    int lastk = -1;             // Holds last value of the character position

    for (int i = 0, k = 0; i < length; i++, k++)
    {
        // Get next codepoint from byte string and glyph index in font
        int codepointByteCount = 0;
        int codepoint = GetCodepoint(&text[i], &codepointByteCount);
        int index = GetGlyphIndex(font, codepoint);

        // NOTE: Normally we exit the decoding sequence as soon as a bad byte is found (and return 0x3f)
        // but we need to draw all of the bad bytes using the '?' symbol moving one byte
        if (codepoint == 0x3f) codepointByteCount = 1;
        i += (codepointByteCount - 1);

        float glyphWidth = 0;
        if (codepoint != '\n')
        {
            glyphWidth = (font.glyphs[index].advanceX == 0) ? font.recs[index].width*scaleFactor : font.glyphs[index].advanceX*scaleFactor;

            if (i + 1 < length) glyphWidth = glyphWidth + spacing;
        }

        // NOTE: When wordWrap is ON we first measure how much of the text we can draw before going outside of the rec container
        // We store this info in startLine and endLine, then we change states, draw the text between those two variables
        // and change states again and again recursively until the end of the text (or until we get outside of the container).
        // When wordWrap is OFF we don't need the measure state so we go to the drawing state immediately
        // and begin drawing on the next line before we can get outside the container.
        if (state == MEASURE_STATE)
        {
            // TODO: There are multiple types of spaces in UNICODE, maybe it's a good idea to add support for more
            // Ref: http://jkorpela.fi/chars/spaces.html
            if ((codepoint == ' ') || (codepoint == '\t') || (codepoint == '\n')) endLine = i;

            if ((textOffsetX + glyphWidth) > rec.width)
            {
                endLine = (endLine < 1)? i : endLine;
                if (i == endLine) endLine -= codepointByteCount;
                if ((startLine + codepointByteCount) == endLine) endLine = (i - codepointByteCount);

                state = !state;
            }
            else if ((i + 1) == length)
            {
                endLine = i;
                state = !state;
            }
            else if (codepoint == '\n') state = !state;

            if (state == DRAW_STATE)
            {
                textOffsetX = 0;
                i = startLine;
                glyphWidth = 0;

                // Save character position when we switch states
                int tmp = lastk;
                lastk = k - 1;
                k = tmp;
            }
        }
        else
        {
            if (codepoint == '\n')
            {
                if (!wordWrap)
                {
                    textOffsetY += (font.baseSize + font.baseSize/2)*scaleFactor;
                    textOffsetX = 0;
                }
            }
            else
            {
                if (!wordWrap && ((textOffsetX + glyphWidth) > rec.width))
                {
                    textOffsetY += (font.baseSize + font.baseSize/2)*scaleFactor;
                    textOffsetX = 0;
                }

                // When text overflows rectangle height limit, just stop drawing
                if ((textOffsetY + font.baseSize*scaleFactor) > rec.height) break;

                // Draw selection background
                bool isGlyphSelected = false;
                if ((selectStart >= 0) && (k >= selectStart) && (k < (selectStart + selectLength)))
                {
                    DrawRectangleRec((Rectangle){
                            /* X */rec.x + textOffsetX - 1,
                            /* Y */ rec.y + textOffsetY,
                            /* Width */ glyphWidth,
                            /* Height */ (float)font.baseSize*scaleFactor },
                            /* Color */ selectBackTint);
                    isGlyphSelected = true;
                }

                // Draw current character glyph
                if ((codepoint != ' ') && (codepoint != '\t'))
                {
                    DrawTextCodepoint(font, codepoint, (Vector2){ rec.x + textOffsetX, rec.y + textOffsetY }, fontSize, isGlyphSelected? selectTint : tint);
                }
            }

            if (wordWrap && (i == endLine))
            {
                textOffsetY += (font.baseSize + font.baseSize/2)*scaleFactor;
                textOffsetX = 0;
                startLine = endLine;
                endLine = -1;
                glyphWidth = 0;
                selectStart += lastk - k;
                k = lastk;

                state = !state;
            }
        }

        if ((textOffsetX != 0) || (codepoint != ' ')) textOffsetX += glyphWidth;  // avoid leading spaces
    }
}
