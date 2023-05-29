# Diagram Editor GUI in Rust

## Features
 - Nothing

## To dos
 - Add help menu
     - `CTRL+]` to open a link
 - Have smooth cursor movements (optional)
 - Have smooth scrolling (optional)
    - [How GNVIM is doing it](https://github.com/vhakulinen/gnvim/blob/1594850acdd0b39fdf4ab2d7188178fc5f832460/ui/src/components/grid-buffer/mod.rs#L152-L201)
 - Add option to draw in a Drawing Area of a GL Area
 - Add Color picker
 - Add Calendar picker
 - Add Emogy picker
 - Add option for a zoom-out minimap
 - Add Images in the middle of text files
    - Store information in a hidden companion file
 - Add Videos in the middle of text files
    - Store information in a hidden companion file
 - Have a separate thread to run builds...
 - Add GUI to select fonts on `:set guifont=*`
 - Add GUI to set the application for `gx`
 - Add "Print to pdf" option

## Build/Install

### Fedora
```
Install cargo (e.g. the rust toolchain)

$ sudo dnf install gtk4-devel
$ make build

No installation yet (in developement)
```
### Ubuntu
```
Install cargo (e.g. the rust toolchain)

$ sudo apt install libgtk-4-dev
$ make build

No installation yet (in developement)
```


## Developer's resources
 - GTK4 book: https://gtk-rs.org/gtk4-rs/stable/latest/book/
 - GTK4 Documentation: https://gtk-rs.org/gtk4-rs/stable/latest/docs/
 - GTK4 Examples: https://github.com/gtk-rs/gtk4-rs/tree/master/examples

## Troubleshooting

### template_child error
When running the program, if it does not start and prints:
```
thread 'main' panicked at 'Failed to retrieve template child. Please check that it has been bound and has a #[template_child] attribute.' [...]
```
You coudl try to update `Cargo.lock` with: `cargo update -p gtk4`.
