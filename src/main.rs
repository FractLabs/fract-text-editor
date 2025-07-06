use std::io;
use std::io::Write;
use std::fs;
use std::fs::OpenOptions;
use slint::slint;

slint! {
    import { HorizontalBox, Button } from "std-widgets.slint";
    export component FractTextEditor inherits Window {
        title: "Fract Text Editor";
        min-height: 300px;
        min-width: 500px;
        max-height: 600px;
        max-width: 800px;

        HorizontalBox {
            Button {
                text: "New";
                width: 60px;
                height: 40px;
            } 
            
            Button {
                text: "Open";
                width: 60px;
                height: 40px;
            } 
            padding: 5px;
            spacing: 30px;
        }
    }
}

fn main() {
    FractTextEditor::new().unwrap().run().unwrap();
    let file_to_write = "file.txt";
    let mut write_options = OpenOptions::new()
    .append(true)
    .create(true)
    .write(true)
    .open(file_to_write)
    .expect("Error");
}
