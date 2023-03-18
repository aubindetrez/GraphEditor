# Diagram Editor GUI in Rust

## Features
- Nothing

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
