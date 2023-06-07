# Review of Rust UI framework
 - `gtk-rs/gtk3-rs`, 87 forks and 475 stars
 - `gtk-rs/gtk4-rs`, 130 forks and 1.3k stars
 - `antoyo/relm`, 76 forks and 2.3k stars, not stable enough
 - `fltk-rs/fltk-rs`, niche, small community
 - `Relm4/Relm4`, nice, small nice, based on gtk4
 - `iced`, still very new and not stable
 - `skulpin` mentionned by Helix
 - `gfx-rs/wgpu` mentionned by Helix 612 forks, 7.9k, very active, stable?
 Conclusion: All libraries are rather small, not sure if they are going to be maintained in the future
 or stable enough for my usecase.

 # Review of C/C++ Gui libraries
 - `SDL2`, stable, portable, lightweigth
 - `SFML`, better (C++) APIs than SDL
 - `raylib`, accelerated, 2D/3D. Already buggy unicode and input method support. Only good for small projects
 - `GTK` needs hacks to optimize for my usecase (I do not want the default widgets)
 - `Qt` bloat
 - `GLFW`, (rust binding glfw-rs)M 4,4k forks 11k stars. Harder for just a text editor
