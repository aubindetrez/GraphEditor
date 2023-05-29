use gtk4::prelude::*;
use gtk4::{glib, Application, ApplicationWindow};
use gtk4::Button;

const APP_ID: &str = "org.gtk_rs.HelloWorld2";

fn main() -> glib::ExitCode {
    // Create a new application
    let app = Application::builder().application_id(APP_ID).build();

    // Connect a handler to "activate" signal of `app`
    app.connect_activate(build_ui);

    app.run()
}

fn build_ui(app: &Application) {
    let button = Button::builder()
        .label("Button label")
        .margin_top(12)
        .margin_bottom(12)
        .margin_start(12)
        .margin_end(12)
        .build();

    button.connect_clicked(|button| {
        // Executed when button is clicked
        button.set_label("Button pressed");
    });

    let window = ApplicationWindow::builder()
        .application(app)
        .title("Window Title")
        .child(&button)
        .build();

    window.present();
}
