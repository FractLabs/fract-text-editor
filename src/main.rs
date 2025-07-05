use std::io;
use std::io::Write;
use std::fs::OpenOptions;
fn main() {
    let file_to_write = "file.txt";
    let mut write_options = OpenOptions::new()
    .append(true)
    .create(true)
    .write(true)
    .open(file_to_write)
    .expect("Error");
    let mut to_write = "teste breaaaaaaaaaad";
    let mut line = 1;
    println!("Fract Text Editor");
    println!("Press CTRL + C to exit.");
    // {}
    // ^ ela vai guardar tudo o que tem dentro dela
    // {"ola", "ola1", "ola2"}
    let mut texts: Vec<String> = Vec::new();
    loop {
        print!("{line}. ");
        io::stdout().flush().unwrap();
        let mut text = String::new();   

        io::stdin().read_line(&mut text);
        line += 1;

        texts.push(format!("{text}"));
        writeln!(write_options, "{}", &text.trim()).expect("fail in writing");
        //write_options.write_all("{to_write}".as_bytes())?;
    }
}
