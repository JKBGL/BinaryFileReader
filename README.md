# BinaryFileReader
Here we have a basic c++ binary reader I wrote for an unrelated project that required it.<br>
I decided to post in on github in case some people needed it. ♥

#### Features:
- Reading:
  - int (Any bit size, with endian adjustability)
  - char Array
  - string (very unstable)
- Converting:
  - bytes to int (any size with little_endian support)
  - bytes to char array
  - bytes to std::string
- Misc:
  - Displaying byte array to console in hex format

#### Contributions:
> If you find this library useful and have extensive knowledge in c++<br>
and want to make it better, please do create a pull request<br>
with proposed features, fixes or overall better implementations of<br>
the methods this library uses.

## Docs:
----
#### Example program featuring everything in the library:
```cpp
#include "BinaryReader.h"
#define byte unsigned char

void main() {
  // Opening the file we will read from
  BinaryFileReader a("test.bin");

  // This will read a string and store it in the variable "name"
  // and will also store its size in the "name_size" variable
  size_t name_size = 0;
  byte *name = a.ReadBytesUntilBreak(name_size);

  // convert bytes to a usable char array
  char* str_name = BinaryFileReader::ConvertBytesToCharArray(name, name_size);

  // Reading the next 32 bytes from the file
  byte *t_hash = a.ReadBytes(32);

  // This will display the hex of the "t_hash" variable (useful for comparison of boundaries
  // or checking if the reader is reading correctly)
  BinaryFileReader::DisplayBytesHex(t_hash, 32);

  // Read a 3 byte int value
  byte* key_id_bytes = a.ReadBytes(3);

  // Convert a 3 byte int value to usable int variable
  int key_id = BinaryFileReader::ConvertBytesToAnyInt(key_id_bytes, 3);

  // display key_id_bytes
  BinaryFileReader::DisplayBytesHex(key_id_bytes, 3);

  // Read a 16bit int
  int16_t key_flags = a.ReadInt16();

  // Read a 32bit int
  int name_flags = a.ReadInt32();
  
  // Closing the binary file
  a.close();
  
  ...
  
  return;
}
```
----
## Reading 

### Opening a file:
- Directly in the constructor:
  ```cpp
  BinaryFileReader a("test.bin");
  ```
- With the open() method:
  ```cpp
  BinaryFileReader a;
  a.open("test.bin");
  ```

---

### Closing an opened file:
```cpp
a.close();
```
Note: the object will automatically close the file if out of scope

---

### Read a 16bit int:
```cpp
a.ReadInt16();
```
returns a usable `int16_t`

---

### Read a 32bit int:
```cpp
a.ReadInt32();
```
returns a usable `int`

---

### Read a single byte:
```cpp
a.ReadByte();
```
returns an `unsigned char`

---

### Read bytes:
```cpp
a.ReadBytes(40);
```
returns `unsigned char*`<br>
Note: limited to 500 bytes per call

---

### Read string with unknown size (char*):
```cpp
size_t size_variable;
a.ReadBytesUntilBreak(size_variable);
```
returns `unsigned char*`<br>
writes the size of the string to the size variable

---

### Read string with unknown size (std::string):
```cpp
a.ReadString();
```
returns `std::string`<br>
Note: This function may show unwanted/unstable behaviour, use `ReadBytesUntilBreak()`

---
<br><br>

## Converting

### Convert byte array to an integer:
```cpp
int result = BinaryFileReader::ConvertBytesToAnyInt(byte_arr, 4);
```
returns an `int`

---

### Convert byte array to an integer (endian toggle):
```cpp
int result = BinaryFileReader::ConvertBytesToAnyInt2(byte_arr, 4, true);
```
returns an `int`

---

### Convert byte array to char* array:
```cpp
char *result = BinaryFileReader::ConvertBytesToCharArray(byte_arr, byte_arr_size);
```
returns `char*`

---
<br><br>

## Misc

### View byte array in console:
```cpp
BinaryFileReader::DisplayBytesHex(byte_arr, byte_arr_size);
```
returns `char*`


