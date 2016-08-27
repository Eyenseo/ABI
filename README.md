## About
The main macro defined by this header is `E_ABI(s)` and it is used to export and import symbols of dynamic libraries.

The normal solution is to create a header for each library, copy & paste the contents of an existing library and then change the macros that are being defined by search and replace.

This header is the only header that you'll need from now on because
```cpp
void E_ABI(mylib) foo();
```
will evaluate to different statements depending on definitions.

## Usage

The default behaviour is to import symbols. To export `foo` you'll have to pass `E_ABI_mylib` (eg. `gcc -DE_ABI_mylib ...`). If the library is build as a static version you can pass `E_ABI_STATIC_mylib` since the linker will handle all symbols.

|      import     |      export     |         static         |
|:---------------:|:---------------:|:----------------------:|
|                 |  E_ABI_mylib    |  E_ABI_STATIC_mylib    |
|  E_ABI_mylib 0  |  E_ABI_mylib 1  |  E_ABI_STATIC_mylib 1  |
|  E_ABI_mylib 2  |  -------------  |  --------------------  |
|  E_ABI_mylib a  |  -------------  |  --------------------  |
|       ...       |  -------------  |  --------------------  |

+ __Import__ means either `__declspec(dllimport)` or
  `__attribute__((visibility("default")))` depending on the platform / compiler.
+ __Export__ means either `__declspec(dllexport)` or
  `__attribute__((visibility("default")))` depending on the platform / compiler.
+ __Static__ means that the macro will be evaluated to nothing -- the linker will take care of everything.

To gain benefits from exporting / importing with gcc / clang the compile option `-fvisibility=hidden` has to be passed to the compiler.

## Credits
+ [pfultz2](http://pfultz2.com/blog/) for his [Cloak](https://github.com/pfultz2/Cloak) library.
