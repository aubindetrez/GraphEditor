#include <stdio.h>
#include <string.h>
#include "raylib.h"

// To do:
// - Add cursor when text is entered (in editing mode)
// - Only redraw screen when needed (not every frame)
// - Add Vim mode (in editing mode)
// - Add "[...]" at the bottom if the text doesn't fit

#define MAX_INPUT_CHARS 255

// Draw text using font inside rectangle limits
static void DrawTextBoxed(Font font, const char *text, Rectangle rec, float fontSize, float spacing,
        Color tint);
static void DrawSelectableTextBox(Font font, const char *text, Rectangle rec, float fontSize,
        float spacing, Color tint, int selectStart, int selectLength,
        Color selectTint, Color selectBackTint);
static void DrawChar(const Font &, int, const Vector2 &, float, const Color &);
static void DrawNewLine(Font, float*, float*, const float&);

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    //--------------------------------------------------------------------------------------
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;
    const int handleSize = 14;
    const int boxLineWidth = 3;
    const int boxLeftMargin = 50;
    const int boxRigthMargin = 50;
    const int boxTopMargin = 50;
    const int boxBottomMargin = 50;

    InitWindow(screenWidth, screenHeight, "Diagram editor");

    char text[MAX_INPUT_CHARS] = "";

    bool resizing = false;
    bool moving = false;
    bool insert_mode = false;

    Rectangle container = { 25.0f, 25.0f, screenWidth - 50.0f, screenHeight - 250.0f };
    Rectangle resizer = {
        container.x + container.width - handleSize - boxLineWidth,
        container.y + container.height - handleSize - boxLineWidth,
        handleSize,
        handleSize
    };
    Rectangle mover = { container.x, container.y, handleSize, handleSize };

    // Minimum width and heigh for the container rectangle
    const float minWidth = 60;
    const float minHeight = 60;
    const float maxWidth = screenWidth - 50.0f;
    const float maxHeight = screenHeight - 160.0f;

    Vector2 lastMouse = { 0.0f, 0.0f }; // Stores last mouse coordinates
    Color borderColor = Fade(MAROON, 0.4f); // Container border color
    Font font = GetFontDefault();       // Get default system font

    SetTargetFPS(60);                   // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

                                        // Main game loop
    while (!WindowShouldClose())        // Detect window close button or ESC key
    {
        Vector2 mouse = GetMousePosition();

        // Check if the mouse is inside the container and toggle border color
        if (CheckCollisionPointRec(mouse, container)) borderColor = MAROON;
        else if (!resizing && !moving) borderColor = Fade(MAROON, 0.4f);

        // Container resizing logic
        if (resizing)
        {
            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) resizing = false;

            float width = container.width + (mouse.x - lastMouse.x);
            container.width = (width > minWidth)? ((width < maxWidth)? width : maxWidth) : minWidth;

            float height = container.height + (mouse.y - lastMouse.y);
            container.height = (height > minHeight)? ((height < maxHeight)? height : maxHeight) : minHeight;
        }
        // Container moving logic
        else if (moving)
        {
            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) moving = false;

            float x = mouse.x - lastMouse.x;
            container.x += x;

            float y = mouse.y - lastMouse.y;
            container.y += y;
        }
        else
        {
            // Check if we're resizing
            if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
                if (CheckCollisionPointRec(mouse, resizer)) resizing = true;
                else if (CheckCollisionPointRec(mouse, mover)) moving = true;

                // Click on the rectangle to type, click somewhere else to exit typing mode
                if (CheckCollisionPointRec(mouse, container)) insert_mode = true;
                else insert_mode = false;
            }
        }

        // Typing in rectangle
        if (insert_mode)
        {
            SetMouseCursor(MOUSE_CURSOR_IBEAM);
            // Get char pressed (unicode character) on the queue
            int letterCount = strlen(text);

            // Keys (backspace and enter)
            int key_pressed = GetKeyPressed();
            while (key_pressed > 0) { // Handle keys until buffer is empty
                if (key_pressed == KEY_BACKSPACE) {
                    if (letterCount > 0) {
                        text[letterCount-1] = '\0';
                        letterCount--;
                    }
                } if (key_pressed == KEY_ENTER) {
                    if (letterCount < MAX_INPUT_CHARS) {
                        text[letterCount] = '\n';
                        text[letterCount+1] = '\0'; // Add null terminator
                        letterCount++;
                    }
                }
                key_pressed = GetKeyPressed();
            }

            // All other chars
            int char_pressed = GetCharPressed();
            while (char_pressed > 0) // Handle keys until buffer is empty
            {
                if (letterCount < MAX_INPUT_CHARS) {
                    // NOTE: Only allow printable keys [32..126] = [' '..'~'] 
                    if ((char_pressed >= ' ') && (char_pressed <= '~'))
                    {
                        text[letterCount] = (char)char_pressed;
                        text[letterCount+1] = '\0'; // Add null terminator at the end of the string.
                        letterCount++;
                    } else {
                        printf("Non printable character: %d\n", char_pressed);
                    }
                }
                char_pressed = GetCharPressed();  // Check next character in the queue
            }
        } else {
            SetMouseCursor(MOUSE_CURSOR_DEFAULT);
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


        Rectangle box_with_margin = {
                    container.x + boxLineWidth + boxLeftMargin,
                    container.y + boxLineWidth + boxTopMargin,
                    container.width - boxLineWidth - boxLeftMargin - boxRigthMargin,
                    container.height - boxLineWidth - boxTopMargin - boxBottomMargin 
        };
        // Draw text in container (add some padding)
        DrawTextBoxed(font, text, box_with_margin, /*font size*/20.0f, /*spacing*/2.0f, /*color*/GRAY);

        DrawRectangleRec(resizer, borderColor);             // Draw the resize box
        DrawRectangleRec(mover, borderColor);             // Draw the move box

        // Draw bottom info
        //DrawRectangle(0, screenHeight - 54, screenWidth, 54, GRAY);
        //DrawRectangleRec((Rectangle){ 382.0f, screenHeight - 34.0f, 12.0f, 12.0f }, MAROON);

        //DrawText("Word Wrap: ", 313, screenHeight-115, 20, BLACK);
        //else DrawText("OFF", 447, screenHeight - 115, 20, BLACK);

        //DrawText("Press [SPACE] to toggle word wrap", 218, screenHeight - 86, 20, GRAY);

        //DrawText("Click hold & drag the    to resize the container", 155, screenHeight - 38, 20, RAYWHITE);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow(); // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

//--------------------------------------------------------------------------------------
// Module functions definition
//--------------------------------------------------------------------------------------

// Draw text using font inside rectangle limits
static void DrawTextBoxed(Font font, const char *text, Rectangle rec, float
        fontSize, float spacing, Color tint) {
    DrawSelectableTextBox(font, text, rec, fontSize, spacing, tint, 0, 0, WHITE, WHITE);
}

// Takes a codepoint returned by GetCodepoint() and returns true if it is a whitespace
bool is_codepoint_whtspace(int codepoint) {
    return (codepoint == ' ') || (codepoint == '\t') || (codepoint == '\n');
}

// Draw text using font inside rectangle limits with support for text selection
// WRAP_WORD must be implemented separatly because it would complexify the common code to much. TODO
static void DrawSelectableTextBox(Font font, const char *text, Rectangle rec,
        float fontSize, float spacing, Color tint, int
        selectStart, int selectLength, Color selectTint, Color selectBackTint) {

    // NO_WRAP: Do not wrap lines
    // WRAP_CHAR: Wrap line on any character
    const enum {NO_WRAP=0, WRAP_CHAR=2} wrap_mode = WRAP_CHAR;

    size_t length_byte = TextLength(text);  // Total length in bytes of the text
    float textOffsetX = 0.0f; // Horizontal position of the next character to be drawn
    float textOffsetY = 0.0f; // Vertical position of the next character to be drawn
    float scaleFactor = fontSize/(float)font.baseSize; // Character rectangle scaling factor

    // Because it is UTF-8 a character can be more than 1-byte
    size_t byte = 0; // Index (byte) of the current glyph in 'text'

    // For all glyphs we need first to check if we need to wrap the line before we start drawing
    // the characters one by one. Stop when there is no more vertical space
    while (byte < length_byte) {
        int codepointByteCount = 0;
        int codepoint = GetCodepoint(&text[byte], &codepointByteCount);
        //if (codepoint == '?' /*0x3f*/) codepointByteCount = 1;
        int index = GetGlyphIndex(font, codepoint);

        // With UTF-8 a glygh can be many byte
        byte += codepointByteCount;

        // TODO expand tabs

        // Get the glyph width
        float glyphWidth = 0;
        if (codepoint != '\n') {
            if (font.glyphs[index].advanceX == 0)
                glyphWidth = font.recs[index].width*scaleFactor;
            else
                glyphWidth = font.glyphs[index].advanceX*scaleFactor;

            // Spaceout characters
            glyphWidth = glyphWidth + spacing;
        }

        if (wrap_mode == WRAP_CHAR) {
            // if the current character does not fit on screen
            if (codepoint == '\n' || textOffsetX+glyphWidth > rec.width) {
                DrawNewLine(font, &textOffsetX, &textOffsetY, scaleFactor);

                // If the next line does not fit in the rectangle, stop drawing
                if (textOffsetY > rec.height) {
                    break;
                }
                // TODO draw something like "..." to indicated not all lines were printed
            }

            // Draw the character
            // TODO change tint
            if (codepoint != '\n') {
                Vector2 position = { rec.x + textOffsetX, rec.y + textOffsetY };
                DrawChar(font,
                        codepoint,
                        position,
                        fontSize,
                        tint);
                textOffsetX += glyphWidth;
            }
            
        } else if (wrap_mode == NO_WRAP) {
            if (codepoint == '\n') {
                DrawNewLine(font, &textOffsetX, &textOffsetY, scaleFactor);

                // If the next line does not fit in the rectangle, stop drawing
                if (textOffsetY > rec.height) {
                    break;
                }
                // TODO draw something like "..." to indicated not all lines were printed
            }
            else if (textOffsetX+glyphWidth > rec.width) {
                // If the character does not fit on the screen, ignore it
            } else {
                Vector2 position = { rec.x + textOffsetX, rec.y + textOffsetY };
                DrawChar(font,
                        codepoint, 
                        position,
                        fontSize,
                        tint);
                textOffsetX += glyphWidth;
            }
        }

        // TODO draw the cursor
    }
}

static void DrawNewLine(Font font, float *textOffsetX, float *textOffsetY, const float &scaleFactor) {
    *textOffsetY += (font.baseSize + font.baseSize/2)*scaleFactor;
    *textOffsetX = 0;
}

static void DrawChar(const Font &font, int codepoint, const Vector2 &position, float fontSize, const Color &tint) {
    DrawTextCodepoint(
            font,
            codepoint,
            position,
            fontSize,
            tint
            );
}

