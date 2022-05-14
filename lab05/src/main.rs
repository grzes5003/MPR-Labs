use std::collections::HashMap;
use std::env;
use std::fs::File;
use std::io::{BufRead, BufReader};

fn count_words(path: String, dict: &mut HashMap<String, u64>) -> Result<(), std::io::Error> {
    let file = File::open(path)?;
    let mut reader = BufReader::new(file);
    let mut buf = vec![];

    while let Ok(_) = reader.read_until(b'\n', &mut buf) {
        if buf.is_empty() {
            break;
        }
        String::from_utf8_lossy(&buf)
            .split_whitespace().for_each(|word| {
            dict.entry(word
                .chars()
                .filter(|c| c.is_alphabetic() || *c == '\'')
                .collect::<String>()
                .to_ascii_lowercase())
                .and_modify(|count| *count += 1)
                .or_insert(1);
        });
        buf.clear();
    }

    Ok(())
}


fn main() -> Result<(), std::io::Error> {
    let mut dict = HashMap::new();
    let result: Result<Vec<_>, _> = env::args().skip(1)
        .map(|path|
            count_words(path, &mut dict)
        )
        .collect();

    match result {
        Ok(_) => Ok(()),
        Err(e) => {
            println!("{:?}", e);
            Err(e)
        }
    }
}
