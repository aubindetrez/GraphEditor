# Diagram editor

## Introduction
The GUI part of the application is isolated from the rest so it can change in the future.
[Raylib](https://www.raylib.com/) is being used for the GUI rendering since it is lightweight
and multiplatform.

## Internal format
The diagrams are saved in a plain text format because it has to be easy to review changes and
track versions of a draph.

```

// This is a comment
// Using a subset of the C syntax (Because the engineers who may be using it are used to C)
// Multiple instances can be named annon
TextBlock anon {
    position = { .x = 10, .y = 100};
    size = { .width = 100, .height = 100 };
    font = exampleFont; // See below
    text_processor = MARKDOWN; // The Markdown processor will Set the font to 'large' for the title...
    on_click_script = "gvim somefile.C"; // This command will be executed we the user clicks on the block
    border = { .color = BLACK, .width = 4, .type = DEFAULT }; // can be: rounded
    background = WHITE;
%%%
#Title#
This special block can contain anything you may want in you text block.
If you click on this block, it will open somefile.c in gvim.
%%%
    anchors = {
        {.x = 0, .y = 50, .conn = NULL}, // Position of an anchor without any connection (center left)
        {.x = 100, .y = 50, .conn = {conn_001, conn_002}}, // center right anchor with 2 connections
    };
}

Font exampleFont {
    size = { .small = 6, .medium = 12, .large = 16 }; // Font size for small, medium and large text
}

Connection conn_001 {
    points = {
        {.x = 110, .y = 150}, // Same position as the anchor of the textblock
        {.x = 210, .y = 150},
    };
    width = 5;
    color = BLACK;
    annotation = {
        {.text = "Signal name"}, // can also use .x, .y or .index to set the position
        {.text = "64", option=SIZE},
    };
}
```

## Dependencies
```
sudo apt install build-essential git cmake libasound2-dev mesa-common-dev libx11-dev libxrandr-dev libxi-dev xorg-dev libgl1-mesa-dev libglu1-mesa-dev
#sudo dnf install alsa-lib-devel mesa-libGL-devel libX11-devel libXrandr-devel libXi-devel libXcursor-devel libXinerama-devel libatomic
#sudo pacman -S alsa-lib mesa libx11 libxrandr libxi libxcursor libxinerama

# For SFML
```

## Compiling
```
git clone --recurse-submodules  https://github.com/aubindetrez/GraphEditor.git GraphEditor
cd GraphEditor
make
```

## Q&A

### Rust vs C++
Short version: I need a low-maintenance Language+Library


Long version: I initially started this project using Rust, but it became obvious that the API
wasn't stable enough. Rust's userbase is growing and the language changed a lot since I started
using it (~2018), making my projects obsolete.
This is just a toy project, and it requires a language (/set of library) that would allow me to
stop working on the project for months/years without having to refactor it when I get back to it.
