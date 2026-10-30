<div align="center">

# inicpp

**Simple, convenient and intuitive INI file handling for Modern C++**

[![License: MIT](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)
[![C++11](https://img.shields.io/badge/C%2B%2B-11%20or%20later-00599C.svg?logo=c%2B%2B&logoColor=white)](#installation)
[![Header only](https://img.shields.io/badge/header--only-1%20file-success.svg)](inicpp.hpp)
[![GitHub stars](https://img.shields.io/github/stars/dujingning/inicpp?style=flat&logo=github)](https://github.com/dujingning/inicpp/stargazers)

**English** | [简体中文](README_CN.md)

[Quick start](#quick-start) · [Install](#installation) · [Usage](#usage) · [Error handling](#error-handling) · [Syntax](#supported-ini-syntax) · [Testing](#testing)

</div>

> 1. Make INI file processing **simple**, **convenient**, and **intuitive**.
> 2. Advance the enduring excellence of **C++** through collaborative innovation.
> 3. Let C++ stay **simple**, **easy** and **free**.

---

## Quick start

Drop `inicpp.hpp` into your project and you are done:

```cpp
#include "inicpp.hpp"
#include <iostream>

int main()
{
    inicpp::IniManager ini("config.ini");   // load and parse (the file may not exist yet)

    ini["server"]["port"] = 8080;           // write (saved to the file immediately)
    int port = ini["server"]["port"];       // read
    std::cout << port << std::endl;
}
```

---

## Features

- **Single header**: copy `inicpp.hpp` into your project. C++11 or later, no third-party dependencies.
- **Intuitive access**: `_ini["server"]["port"]` reads and assigns like a map, with automatic type conversion.
- **Read / write / comment**: write values and comments back to the file, keeping existing comments and line endings.
- **Safe writes**: updates go through a backup-based replacement; deferred writes via `flush()` let you batch changes and handle failures.
- **Predictable errors**: conversion operators throw `std::runtime_error`; `toString()` / `toInt()` / `toDouble()` never throw. See [Error handling](#error-handling).
- **New**: [bind a config straight into your own structs](#bind-to-your-own-struct-read).

---

## Installation

```bash
git clone https://github.com/dujingning/inicpp.git
```

Copy `inicpp.hpp` into your project, or add the repository directory to your include path. C++11 or later is required:

```bash
g++ -std=c++11 -I/path/to/inicpp main.cpp -o app
```

With CMake, declare the header as an interface library:

```cmake
add_library(inicpp INTERFACE)
target_include_directories(inicpp INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/third_party/inicpp)
target_compile_features(inicpp INTERFACE cxx_std_11)

target_link_libraries(my_app PRIVATE inicpp)
```

---

## API cheat sheet

| Task | API |
| --- | --- |
| Read (throws if missing / invalid) | `int v = ini["sec"]["key"];`, `ini["sec"]["key"].get<T>()` |
| Read (never throws) | `ini["sec"].toString("key")`, `.toInt("key")`, `.toDouble("key")` |
| Write | `ini["sec"]["key"] = v;`, `ini.set("sec", "key", v[, "comment"])` |
| Comment | `ini.setComment("sec", "key", "text")` |
| Query | `ini.sectionsList()`, `ini.sectionMap("sec")`, `ini["sec"].isKeyExists("key")`, `ini.isSectionExists("sec")` |
| Deferred write | `ini.setAutoFlush(false)`, `ini.isDirty()`, `ini.flush()` |
| Reload | `ini.parse()` (discards unflushed changes) |

---

## Usage

Every example below is self-contained: include `inicpp.hpp`, create an `inicpp::IniManager`, and go.

### Read values

Convert: From string to typed values. `operator[]` value conversions, including `std::string`, `bool`, and `get<T>()`, throw `std::runtime_error` when the section/key is missing or conversion fails. Existing empty values are valid and convert to an empty `std::string`; use `toString()`, `toInt()`, and `toDouble()` for non-throwing default-value reads. `bool` conversion treats only `"0"`, `"false"`, and `"no"` as false.

```cpp
#include "inicpp.hpp"
#include <iostream>

int main()
{
    inicpp::IniManager _ini("config.ini"); // Load and parse the INI file.

    int         port = _ini["server"]["port"];
    std::string ip   = _ini["server"]["ip"];
    std::cout << ip << ":"<< port << std::endl;

    // or get any
    ip   = _ini["server"]["ip"].get<std::string>();
    port = _ini["server"]["port"].get<int>();
    std::cout << ip << ":"<< port << std::endl;
}
```

### Write values

Write: Set directly to the file.

```cpp
#include "inicpp.hpp"
#include <iostream>

int main()
{
    inicpp::IniManager _ini("config.ini"); // Load and parse the INI file.

    _ini["server"]["ip"] = "192.168.3.35";
    _ini["server"]["port"] = 554;
    std::cout << _ini["server"]["ip"] << ":"<< _ini["server"]["port"] << std::endl;

    // or set any
    _ini.set("server","ip","127.0.0.1");
    _ini.set("server","port",8080);
    std::cout << _ini["server"]["ip"] << ":"<< _ini["server"]["port"] << std::endl;
}
```

### Deferred writes

By default, `set()` and `operator[]` assignment write to the file immediately. If you need to update many values, disable auto flush, update values in memory, and call `flush()` once.

```cpp
#include "inicpp.hpp"
#include <iostream>

int main()
{
    inicpp::IniManager _ini("config.ini"); // Load and parse the INI file.

    if (!_ini.setAutoFlush(false))
    {
        return 1;
    }

    _ini.set("server", "ip", "127.0.0.1"); // Memory only.
    _ini.set("server", "port", 8080);       // Memory only.
    _ini["server"]["name"] = "main";        // Memory only.

    if (_ini.isDirty() && !_ini.flush())
    {
        std::cerr << "failed to write config.ini" << std::endl;
        return 1;
    }
}
```

`flush()` writes all pending changes to disk through the same backup-based replacement path as immediate writes. The destructor does not call `flush()`, so callers can handle write failures explicitly. Calling `parse()` discards unflushed changes and reloads the file. Calling `setAutoFlush(true)` while changes are pending first attempts to `flush()` them; it returns `false` if that write fails.

### Comments

Comment: Write comments for key-value pairs.

```cpp
#include "inicpp.hpp"
#include <iostream>

int main()
{
    inicpp::IniManager _ini("config.ini"); // Load and parse the INI file.

    // comment section/key
    _ini.set("math"/*section*/, "PI"/*key*/, "3.1415926535897932"/*key*/, "This is PI in mathematics."/*comment*/);
    _ini.setComment("server"/*section*/, "port"/*key*/, "this is the listen ip for server."/*comment*/);
}
```

Comments written through `set()` or `setComment()` are prefixed with `;` by default. If the comment string already starts with `;` or `#`, that marker is kept.

### Non-throwing reads: `toString()`, `toInt()`, `toDouble()`

Convert: From string to type without throwing. `toString()` returns an empty string for missing keys, while `toInt()` and `toDouble()` return `0` or `0.0` when the key is missing or conversion fails.

```cpp
#include "inicpp.hpp"
#include <iostream>

int main()
{
    inicpp::IniManager _ini("config.ini"); // Load and parse the INI file.
    
    _ini.set("server","port","554","this is the listen port for server");
    std::cout << _ini["server"]["port"] << std::endl;

    // toString
    std::string http_port_s = _ini["server"].toString("port");
    std::cout << "to string:\tserver.port = " << http_port_s << std::endl;

    // toDouble
    double http_port_d = _ini["server"].toDouble("port");
    std::cout << "to double:\tserver.port = " << http_port_d << std::endl;

    // toInt
    int http_port_i = _ini["server"].toInt("port");
    std::cout << "to int:\t\tserver.port = " << http_port_i << std::endl;
}
```

### Enumerate: `isKeyExists()`, `sectionsList()`, `sectionMap()`

May contain unnamed sections: when keys are at the head of the file.

```cpp
#include "inicpp.hpp"
#include <iostream>
#include <iomanip>

// ANSI escape code for green text
#define GREEN_TEXT "\033[1;32m"
#define RESET_COLOR "\033[0m"

int main()
{
    inicpp::IniManager _ini("config.ini"); // Load and parse the INI file.

    for (auto &sectionName : _ini.sectionsList())
    {
        std::cout << GREEN_TEXT << "\nSection: " << RESET_COLOR << sectionName << "\n";

        for (auto &kv : _ini.sectionMap(sectionName))
        {
            std::cout << std::setw(10) << std::left << kv.first
                        << " -------> "
                        << kv.second << std::endl;
        }
    }
}
```

### `std::wstring`

`std::wstring` support is optional because not every C++11 standard library ships `<codecvt>`. Define `_ENABLE_INICPP_STD_WSTRING_` before including the header to enable it (the legacy misspelled `_ENBABLE_INICPP_STD_WSTRING_` is still accepted).

```cpp
#ifndef _ENABLE_INICPP_STD_WSTRING_
#define _ENABLE_INICPP_STD_WSTRING_ // std::wstring support
#endif

#include "inicpp.hpp"

#include <iostream>
#include <string>

int main()
{
    {
        inicpp::IniManager _ini(L"config.ini"); // Load and parse the INI file.
        std::wstring ws = _ini["server"].toWString("info");
        int port = _ini["server"]["port"];
        std::string ip = _ini["server"]["ip"];

        std::cout << ip << ":" << port << std::endl;
    }
    // or
    {
        inicpp::IniManager _ini;
        _ini.setFileName(L"config.ini");
        _ini.parse();
        std::wstring ws = _ini["server"].toWString("info");
        int port = _ini["server"]["port"];
        std::string ip = _ini["server"]["ip"];

        std::cout << ip << ":" << port << std::endl;
    }
}
```

### Bind to your own struct (read)

config.ini :
```
title=config.ini
[server]
isKeepalived=true
;this is the listen ip for server.
port=8080
ip=127.0.0.1


[math]
;Comment: This is pi in mathematics.
PI=3.141592653589793238462643383279502884
```
config.cpp

```cpp
#include "inicpp.hpp"
#include <iostream>
#include <iomanip>

#define CONFIG_FILE "config.ini"

class app
{
public:
	typedef struct Config
	{
		typedef struct Server
		{
			std::string ip;
			unsigned short port;
			bool isKeepalived;
		} Server;

		std::string title;
		Server server;
		double PI;
	} Config;

	static const Config readConfig()
	{
		inicpp::IniManager _ini(CONFIG_FILE);

		return Config{
			_ini[""]["title"],
			Config::Server{
				_ini["server"]["ip"],
				_ini["server"]["port"],
				_ini["server"]["isKeepalived"]},
			_ini["math"]["PI"],
		};
	}
};

int main()
{
	/** easy read for app as struct */
	app::Config config = app::readConfig();

	std::cout << config.server.ip << std::endl;

	return 0;
}
```

---

## Error handling

| Operation | When something goes wrong |
| --- | --- |
| `ini["sec"]["key"]` converted to `int`, `std::string`, `bool`, ..., `get<T>()`, `<<` | Throws `std::runtime_error` (missing key, or the value cannot be converted) |
| `toString()`, `toInt()`, `toDouble()` | Never throws; returns `""`, `0` or `0.0` |
| `set()`, `setComment()`, `flush()`, `setAutoFlush(true)` | Returns `false` when the file cannot be written |
| `ini["sec"]["key"] = value` | Cannot report a write failure. Use `set()` when you need to detect one |

```cpp
#include "inicpp.hpp"
#include <iostream>
#include <stdexcept>

int main()
{
    inicpp::IniManager ini("config.ini");

    try
    {
        int port = ini["server"]["port"]; // throws if the key is missing or not a number
        std::cout << port << std::endl;
    }
    catch (const std::runtime_error &e)
    {
        std::cerr << "read failed: " << e.what() << std::endl;
    }

    int timeout = ini["server"].toInt("timeout"); // missing key -> 0, never throws
    std::cout << "timeout = " << timeout << std::endl;

    if (!ini.set("server", "ip", "127.0.0.1")) // set() reports write failures
    {
        std::cerr << "failed to write config.ini" << std::endl;
        return 1;
    }
}
```

Thread safety: `IniManager` does no internal locking. Guard an instance with your own mutex if several threads share it.

---

## Supported INI syntax

When reading INI files, inicpp supports these common forms:

- UTF-8 BOM at the beginning of the file.
- Section headers such as `[server]`, `  [server]`, and `[ server ]`.
- Key-value lines using `=` or `:`, such as `port=8080` and `host: localhost`.
- Empty values in existing files and values written with `set(..., "")`, such as `key=`.
- Full-line comments using `;` or `#`, including lines with leading whitespace.
- Inline comments after whitespace, such as `key=value ; comment` and `key=value # comment`.
- Comment markers inside quoted or escaped values are preserved, such as `text="a ; b"` and `path=C:\tmp;cache`.
- Duplicate keys in the same section, and duplicate section headers, use the last parsed value for the repeated key.

Quoted values are returned as written; inicpp does not automatically remove quotes or unescape values. Write operations preserve the existing LF or CRLF line-ending style, but multiline values, bare keys without `=` or `:`, and full formatting-preserving round trips are not supported.

---

## Running the example

Compile it with `example/Makefile`, or without make: `g++ -I../ -std=c++11 main.cpp -o iniExample`.

- Compile `example/main.cpp`

```bash
$ cd example/
$ ls
main.cpp  Makefile
$ make
g++ -I../ -std=c++11 main.cpp -o iniExample
$ ls
iniExample  main.cpp  Makefile
```

- Run example app `iniExample`

```bash
$ ./iniExample

Section:
title      -------> config.ini

Section: math
PI         -------> 3.141592653589793238462643383279502884

Section: server
info       -------> the server socket info.
ip         -------> 127.0.0.1
keepalived -------> true
number     -------> 1
port       -------> 8080
$
```

- Configuration file `config.ini` has been created.

```bash
$ ls
config.ini  iniExample  main.cpp  Makefile
$ cat config.ini
;This is the title.
title=config.ini
[server]
number=1
info=the server socket info.
keepalived=true
;this is the listen ip for server.
port=8080
ip=127.0.0.1


[math]
;Comment: This is pi in mathematics.
PI=3.141592653589793238462643383279502884
$
```

---

## Testing

The tests live in `tests/` and are built with `g++ -std=c++11 -Wall -Wextra -pedantic`. The suite also checks that `example/main.cpp` compiles and that invalid usage is rejected at compile time.

```bash
cd tests
make test
```

Contributing: please read the [development rules](tests/Dev_Rules_en.md) (English) first. Issues and pull requests are welcome.

---

## Project

- GitHub: [dujingning/inicpp](https://github.com/dujingning/inicpp)
- Gitee: [dujingning/inicpp](https://gitee.com/dujingning/inicpp)

If inicpp helps you, a ⭐ star is much appreciated. Created by **DuJingning**.

<a href="https://star-history.com/#dujingning/inicpp">
 <picture>
   <source media="(prefers-color-scheme: dark)" srcset="https://api.star-history.com/svg?repos=dujingning/inicpp&type=Date&theme=dark" />
   <source media="(prefers-color-scheme: light)" srcset="https://api.star-history.com/svg?repos=dujingning/inicpp&type=Date" />
   <img alt="Star History Chart" src="https://api.star-history.com/svg?repos=dujingning/inicpp&type=Date" />
 </picture>
</a>

---

## License

Released under the [MIT License](LICENSE).
