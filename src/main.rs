use std::io;
use std::io::Write;
use std::fs;
use std::fs::OpenOptions;
use slint::slint;
use rfd::FileDialog;
use std::path::PathBuf;

slint! {
import { VerticalBox, HorizontalBox, Button, TextEdit } from "std-widgets.slint";
export component FractTextEditor inherits Window {
        in-out property <bool> editing: false;
        title: "Fract Text Editor";
        max-height: 1080px;
        max-width: 1920px;
        preferred-height: 256px;
        preferred-width: 456px;

        VerticalBox {

            if (root.editing == true):
                TextEdit {
                    font-size: 8px;
                    width: parent.width;
                    height: 300px;
                    wrap: word-wrap;
                    enabled: true;
                    read-only: false;
                    horizontal-alignment: left;
                }


            Text {
                text: "Welcome to Fract Text Editor!";
                font-family: "bold";
                font-size: 24px;
                horizontal-alignment: center;
                vertical-alignment: center;

            }
            Text {
                text: "Press New to create a new text file or press Open to open an existing text file.";
                font-size: 12px;
                horizontal-alignment: center;
                vertical-alignment: center;
            }
        }

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
                clicked => {
                    root.editing = true;
                }
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

    select_file_ui();
}
fn print_from_rs() {
    println!("Hello from Rust!")
}

fn select_file_ui() -> Option<PathBuf> {
    if let Some(selected_file) = FileDialog::new().pick_file() {
        println!("File: {:?}", &selected_file.display());
        Some(selected_file)
    } else {
        println!("None selected");
        None
    }
}