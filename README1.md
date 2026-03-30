# UTF-8 / UTF-16 Converter in C++

This repository contains a simple implementation of a **text encoding converter in C++**.
The program converts files between **UTF-8 and UTF-16 (Little Endian / Big Endian)** without using external libraries.

---

## Supported Conversions

### UTF-8 → UTF-16

Converts a UTF-8 encoded file into:

* UTF-16 Little Endian (`utf16le`)
* UTF-16 Big Endian (`utf16be`)

Features:

* Manual byte-level decoding
* Basic BOM handling
* Supports 1-byte, 2-byte, and 3-byte UTF-8 sequences

---

### UTF-16 → UTF-8

Supports conversion from:

* UTF-16 Little Endian → UTF-8
* UTF-16 Big Endian → UTF-8

Features:

* BOM detection
* Endianness handling (byte swapping for BE)
* Manual UTF-8 encoding

---

## Program Usage

```
./converter <from-encoding> <to-encoding> <source-file> <destination-file>
```

### Example

```
./converter utf8 utf16le input.txt output.txt
```

---

## Project Structure

```
utf8-utf16-converter/
│
├── formatConverter.cpp
└── README.md
```

---

## Compilation

Compile using `g++`:

```
g++ formatConverter.cpp -o converter
```

Run:

```
./converter utf8 utf16be input.txt output.txt
```

---

## Notes

* When using UTF-16, you must specify endian type:

  * `utf16le` – Little Endian
  * `utf16be` – Big Endian
* The program performs conversion using low-level bit manipulation

---

## Limitations

* Assumes input file is correctly encoded
* No strict UTF validation
* Does not support UTF-16 surrogate pairs
* Limited support for UTF-8 (no 4-byte sequences)
* Uses compiler-specific behavior (tested with GCC)
* Not intended for production use

---

## Purpose

This project was created to practice:

* File handling in C/C++
* Bitwise operations
* Understanding of UTF encodings
* Low-level data processing

