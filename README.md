### Ⅰ、Project
You can view the project at [https://github.com/dujingning/inicpp.git](https://github.com/dujingning/inicpp.git) or [https://gitee.com/dujingning/inicpp](https://gitee.com/dujingning/inicpp).

#### * To support open source, please give us a star. Thank you very much!


---

### Ⅱ、Description

The INI header-only library for Modern C++ supports reading, writing, and even commenting. It is easy to use and simplifies working with INI files.


---
### Ⅲ、Usage 

#### * 0.Simple to use with C++11 or later.
```
git clone https://github.com/dujingning/inicpp.git
```

Include `inicpp.hpp`, declare the `inicpp::IniManager` class, and you're all set.

#### * 1.read example
Read: Load file to memory, used directly by the user.
```cpp
#include "inicpp.hpp"
#include <iostream>

int main()
{
    inicpp::IniManager _ini("config.ini"); // Load and parse the INI file.

    std::cout << _ini["rtsp"]["port"] << std::endl;
}
```

#### * 2.write example
Write: Modify directly to the file.
```cpp
#include "inicpp.hpp"
#include <iostream>

int main()
{
    inicpp::IniManager _ini("config.ini"); // Load and parse the INI file.

    _ini.modify("rtsp","port","554");
    std::cout << _ini["rtsp"]["port"] << std::endl;
}
```
#### * 3.or comment
Comment: Write comments for key-value pairs.
```cpp
#include "inicpp.hpp"
#include <iostream>

int main()
{
    inicpp::iniReader _ini("config.ini"); // Load and parse the INI file.

    _ini.modify("rtsp","port","554","this is the listen port for rtsp server");
    std::cout << _ini["rtsp"]["port"] << std::endl;
}
```
#### * 4.toString()、toInt()、toDouble()
Convert: From string to type.
```cpp
#include "inicpp.hpp"
#include <iostream>

int main()
{
    inicpp::IniManager _ini("config.ini"); // Load and parse the INI file.
    _ini.modify("rtsp","port","554","this is the listen port for rtsp server");
    std::cout << _ini["rtsp"]["port"] << std::endl;

    // Convert to string, default is string
    std::string http_port_s = _ini["http"].toString("port");
    std::cout << "to string:\thttp.port = " << http_port_s << std::endl;

    // Convert to double
    double http_port_d = _ini["http"].toDouble("port");
    std::cout << "to double:\thttp.port = " << http_port_d << std::endl;

    // Convert to int
    int http_port_i = _ini["http"].toInt("port");
    std::cout << "to int:\t\thttp.port = " << http_port_i << std::endl;
}
```
#### * 5.isKeyExists()
Check: If the key exists.
```cpp
#include "inicpp.hpp"
#include <iostream>

int main()
{
    inicpp::IniManager _ini("config.ini");

    if (!_ini["rtsp"].isKeyExist("port"))
    {
        std::cout << "rtsp.port: not exist!" << "\n";
    }
    else
    {
        std::cout << "rtsp.port: not exist!" << "\n";
    }

    return 0;
}
```
#### * 6.isSectionExists()、getSectionsList()
May contain unnamed sections: when keys are at the head of the file.
```bash
#include "inicpp.hpp"
#include <iostream>

int main()
{
    inicpp::IniManager _ini("config.ini");

    // isSectionExists
    if (!_ini.isSectionExists("math"))
    {
        std::cout << "section of math: not exist" << "\n\n";
    }

    // getSectionsList
    std::list<std::string> sectionList = _ini.getSectionsList();

    for(auto data:sectionList){ // print
        std::cout << data << std::endl;
    }
}
```

#### * 7.For a full example, see `example/main.cpp`.

You can compile it using `example/Makefile` or any other method you prefer.

If make is not available, use the following command: `g++ -I../ -std=c++11 main.cpp -o iniExample`.


#### * 8.how to compile example/main.cpp
- Compile `example/main.cpp`
```bash
[jn@jn inicpp]$ ls
example  inicpp.hpp  LICENSE  README.md
[jn@jn inicpp]$ cd example/
[jn@jn example]$ make
g++ -I../ -std=c++11 main.cpp -o iniExample
[jn@jn example]$ ls
iniExample  main.cpp  Makefile
```

- Run example app `iniExample`
```bash
[jn@jn example]$ ./iniExample
get rtsp port:555
to string:      rtsp.port = 554
to string:      math.PI   = 3.1415926
to string:      math.PI   = 3.1415926
to double:      math.PI   = 3.1415926
to int:         math.PI   = 3
to wstring:     other.desc= 你好，世界
[jn@jn example]$
```

- Then you will get the config file `config.ini`.
```bash
[jn@jn example]$ cat config.ini
;no section test:add comment later.
noSection=yes
key0=noSectionAndComment
key1=noSectionAndComment
key2=noSectionAndComment
[head]
;thanks for your using inicpp project.
title=inicpp
;Permissive license for open-source software distribution.
license=MIT


[rtsp]
;this is the listen ip for rtsp server.
port=554
ip=127.0.0.1


[math]
;This is pi in mathematics.
PI=3.1415926


[other]
;this test for std::wstring. comment it.
desc=你好，世界
[jn@jn example]$
```

---
### Ⅳ、End
 The project was created by DuJingning.
