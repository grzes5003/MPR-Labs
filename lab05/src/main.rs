use std::collections::HashMap;
use std::env;
use std::fs::File;
use std::io::{BufRead, BufReader, BufWriter, Write};

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

fn save_results(path: String, dict: &HashMap<String, u64>) -> Result<(), std::io::Error> {
    let file = File::create(path)?;
    let mut f = BufWriter::new(file);
    dict.iter().map(|record| {
        f.write_all(format!("{:?}:{:?}\n", record.0, record.1).as_bytes())
    }).collect()
}

fn main() -> Result<(), std::io::Error> {
    let mut dict = HashMap::new();
    let result: Result<Vec<_>, _> = env::args().skip(1)
        .map(|path|
            count_words(path, &mut dict)
        )
        .collect();

    match result {
        Ok(_) => save_results("output.log".to_string(), &dict),
        Err(e) => {
            println!("{:?}", e);
            Err(e)
        }
    }
}
