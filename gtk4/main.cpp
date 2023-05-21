#include <string.h>
#include "raylib.h"

// To do:
// - Add cursor when text is entered (in editing mode)
// - Only redraw screen when needed (not every frame)
// - Add Vim mode (in editing mode)
// - Add "[...]" at the bottom if the text doesn't fit
// - Add key repeat

#define MAX_INPUT_CHARS 255

// Draw text using font inside rectangle limits
static void DrawTextBoxed(Font font, const char *text, Rectangle rec, float fontSize, float spacing,
        Color tint);
// Draw text using font inside rectangle limits with support for text selection
static void DrawTextBoxedSelectable(Font font, const char *text, Rectangle rec, float fontSize,
        float spacing, Color tint, int selectStart, int selectLength,
        Color selectTint, Color selectBackTint);
static void DrawTextBoxedSelectable_refactor(Font font, const char *text, Rectangle rec, float fontSize,
        float spacing, Color tint, int selectStart, int selectLength,
        Color selectTint, Color selectBackTint);

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
            if (IsKeyPressed(KEY_BACKSPACE)) {
                if (letterCount > 0) text[letterCount-1] = '\0';
            } if (IsKeyPressed(KEY_ENTER)) {
                if (letterCount < MAX_INPUT_CHARS) {
                    text[letterCount] = '\n';
                    text[letterCount+1] = '\0'; // Add null terminator
                }
            } else {
                int key = GetCharPressed();
                while (key > 0) // For all unicode character
                {
                    if (letterCount < MAX_INPUT_CHARS) {
                        // NOTE: Only allow printable keys [32..126] = [' '..'~'] 
                        if ((key >= ' ') && (key <= '~'))
                        {
                            text[letterCount] = (char)key;
                            text[letterCount+1] = '\0'; // Add null terminator at the end of the string.
                        }
                    }
                    key = GetCharPressed();  // Check next character in the queue
                }
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

        // Draw text in container (add some padding)
        DrawTextBoxed(font, text, (Rectangle){
                    container.x + boxLineWidth + boxLeftMargin,
                    container.y + boxLineWidth + boxTopMargin,
                    container.width - boxLineWidth - boxLeftMargin - boxRigthMargin,
                    container.height - boxLineWidth - boxTopMargin - boxBottomMargin 
                }, /*font size*/20.0f, /*spacing*/2.0f, /*color*/GRAY);

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
    DrawTextBoxedSelectable_refactor(font, text, rec, fontSize, spacing, tint, 0, 0, WHITE, WHITE);
}

// Takes a codepoint returned by GetCodepoint() and returns true if it is a whitespace
bool is_codepoint_whtspace(int codepoint) {
    return (codepoint == ' ') || (codepoint == '\t') || (codepoint == '\n');
}

// Draw text using font inside rectangle limits with support for text selection
static void DrawTextBoxedSelectable(Font font, const char *text, Rectangle rec,
        float fontSize, float spacing, Color tint, int
        selectStart, int selectLength, Color selectTint, Color selectBackTint) {
    int length = TextLength(text);  // Total length in bytes of the text, scanned by codepoints in loop

    float textOffsetY = 0;          // Offset between lines (on line break '\n')
    float textOffsetX = 0.0f;       // Offset X to next character to draw

    float scaleFactor = fontSize/(float)font.baseSize;     // Character rectangle scaling factor

    // Word/character wrapping mechanism variables
    enum { MEASURE_STATE = 0, DRAW_STATE = 1 };
    int state = MEASURE_STATE;

    int startLine = -1;         // Index where to begin drawing (where a line begins)
    int endLine = -1;           // Index where to stop drawing (where a line ends)
    int lastk = -1;             // Holds last value of the character position

    for (int i = 0, k = 0; i < length; i++, k++) {
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
            glyphWidth = (font.glyphs[index].advanceX == 0) ?
                font.recs[index].width*scaleFactor :
                font.glyphs[index].advanceX*scaleFactor;

            if (i + 1 < length) glyphWidth = glyphWidth + spacing;
        }

        // We first measure how much of the text we can draw before going outside of the rec
        // container.
        // We store this info in startLine and endLine, then we change states, draw the text
        // between those two variables and change states again until the end of the text
        // (or until we get outside of the container).
        if (state == MEASURE_STATE)
        {

            // If a white space is encountered, we can break the line here
            if (is_codepoint_whtspace(codepoint)) endLine = i;
            // TODO: Add support for more unicode whitespaces
            // TODO: configure how wide are tabs
            // TODO: Add a ":set list" option to print spaces as characters
            // Ref: http://jkorpela.fi/chars/spaces.html

            if ((textOffsetX + glyphWidth) > rec.width) // Wrap the line
            {
                // If no white space to wrap on, wrap on the previous glyph
                if (endLine < 1) endLine=i;

                if (i == endLine) endLine -= codepointByteCount;
                if ((startLine + codepointByteCount) == endLine) endLine = (i - codepointByteCount);

                state = DRAW_STATE;
            }
            else if ((i + 1) == length) // End of buffer
            {
                endLine = i;
                state = DRAW_STATE;
            }
            else if (codepoint == '\n') state = DRAW_STATE; // End of line

            if (state == DRAW_STATE)
            {
                textOffsetX = 0;
                i = startLine;
                glyphWidth = 0;

                // Save glyph position when we switch states
                int tmp = lastk;
                lastk = k - 1;
                k = tmp;
            }
        }
        else // state == DRAW_STATE
        {
            if (codepoint == '\n')
            {
            }
            else
            {
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
                    DrawTextCodepoint(
                            font,
                            codepoint,
                            (Vector2){ rec.x + textOffsetX, rec.y + textOffsetY },
                            fontSize,
                            isGlyphSelected? selectTint : tint
                            );
                }
            }

            if (i == endLine)
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

        // avoid leading spaces
        if ((textOffsetX != 0) || (codepoint != ' ')) textOffsetX += glyphWidth;
    } // For each character glyph
}

static void DrawTextBoxedSelectable_refactor(Font font, const char *text, Rectangle rec,
        float fontSize, float spacing, Color tint, int
        selectStart, int selectLength, Color selectTint, Color selectBackTint) {

    // NO_WRAP: Do not wrap lines
    // WRAP_WORD: Wrap line on a whitespace (if any)
    // WRAP_CHAR: Wrap line on any character
    const enum {NO_WRAP=0, WRAP_WORD=1, WRAP_CHAR=2} wrap_mode = WRAP_WORD;

    int length_byte = TextLength(text);  // Total length in bytes of the text
    float textOffsetX = 0.0f; // Horizontal position of the next character to be drawn
    float textOffsetY = 0.0f; // Vertical position of the next character to be drawn
    float scaleFactor = fontSize/(float)font.baseSize; // Character rectangle scaling factor
    int byte_line_start = 0; // First character of the current line (Index in 'text')

    // Because it is UTF-8 a character can be more than 1-byte
    size_t byte = 0; // Index (byte) of the current glyph in 'text'
    size_t last_byte = 0; // Index in 'text' of the last byte (previous iteration)

    // For all glyphs we need first to check if we need to wrap the line before we start drawing
    // the characters one by one. Stop when there is no more vertical space
    while (byte < length_byte) {
        int byte_last_whtspc = 0; // Index of the last white space in 'text' (for the current line)
        int codepointByteCount = 0;
        int codepoint = GetCodepoint(&text[i], &codepointByteCount);
        //if (codepoint == '?' /*0x3f*/) codepointByteCount = 1;
        int index = GetGlyphIndex(font, codepoint);

        // With UTF-8 a glygh can be many byte
        last_byte = byte; // Save the index of the previous byte, if the new one does not fit
                          // in the screen we know where is the last character we should draw
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

        // If character can be used to wrap the line, store the index in wrap_position
        // We store the index of the character before the space. We want the wrapped line to
        // start with the white space
        if (is_codepoint_whtspace(codepoint)) byte_last_whtspc = last_byte;

        if (wrap_mode == WRAP_CHAR) {
            // if the current character does not fit on screen
            if (codepoint == '\n' || textOffsetX+glyphWidth > rec.width) {
                DrawNewLine(font, &textOffsetX, &textOffsetY, scaleFactor);
                byte_line_start = byte; // New line start with the character that didn't fit
                // If the next line does not fit in the rectangle, stop drawing
                // TODO
            }

            // Draw the character
            // TODO change tint
            if (codepoint != '\n') DrawChar(font, codepoint, position, fontSize, tint);
            
        } else if (wrap_mode == WRAP_WORD) {
            // We wait for the line not to fit in the screen or for the last character
            // before we draw the line (and stop at the last whitespace)
            if (codepoint == '\n' || textOffsetX+glyphWidth > rec.width || byte = length_byte) {
                int wrap_position = byte_last_whtspc;

                // If there was no white space in this line, wrap on the last character that fits.
                if (wrap_position <= byte_line_start) {
                    wrap_position = last_byte;
                }

                DrawTextLine(font,
                        /*Start of the line*/ text+byte_line_start, // First character to be drawn
                        /*End of the line*/ text+wrap_position, // Last character to be drawn
                        rec, fontSize, spacing, tint, selectStart,
                        selectLength, selectTint, selectBackTint);

                DrawNewLine(font, &textOffsetX, &textOffsetY, scaleFactor);
                byte_line_start = byte; // New line start with the character that didn't fit
                // If the next line does not fit in the rectangle, stop drawing
                // TODO
            }
        } else if (wrap_mode == NO_WRAP) {
            if (codepoint == '\n') {
                DrawNewLine(font, &textOffsetX, &textOffsetY, scaleFactor);
                byte_line_start = byte;
                // If the next line does not fit in the rectangle, stop drawing
                // TODO
            }
            else if (textOffsetX+glyphWidth > rec.width) {
                // If the character does not fit on the screen, ignore it
            } else {
                DrawChar(font, codepoint, position, fontSize, tint);
            }
        }

        // TODO draw the cursor
    }
}

static void DrawNewLine(Font font, float *textOffsetX, float *textOffsetY, const float &scaleFactor) {
    textOffsetY += (font.baseSize + font.baseSize/2)*scaleFactor;
    textOffsetX = 0;
}

static void DrawChar(Font font, const char *line_start, const char* line_end, Rectangle rec,
        float fontSize, float spacing, Color tint, int
        selectStart, int selectLength, Color selectTint, Color selectBackTint) {
    DrawTextCodepoint(
            font,
            codepoint,
            (Vector2){ rec.x + textOffsetX, rec.y + textOffsetY },
            fontSize,
            isGlyphSelected? selectTint : tint
            );
}

static void DrawTextLine(Font font, const char *line_start, const char* line_end, Rectangle rec,
        float fontSize, float spacing, Color tint, int
        selectStart, int selectLength, Color selectTint, Color selectBackTint) {
            // Draw background (highlight)
            // TODO

            // Draw underline
            DrawTextCodepoint(
                    font, // TODO select different fonts
                    codepoint,
                    (Vector2){ rec.x + textOffsetX, rec.y + textOffsetY },
                    fontSize, // TODO select different font size
                    tint // Select different tint
                    );

            // Draw overline
            // TODO

            // Update textOffsetX and textOffsetY
            textOffsetY += (font.baseSize + font.baseSize/2)*scaleFactor;
            textOffsetX = 0; // Start new line on the left

            // After drawing the line we update textOffsetY to the next line

}
