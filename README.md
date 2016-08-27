## About
The main macro defined by this header is `E_ABI(s)` and it is used to export and import symbols of dynamic libraries.

The normal solution is to create a header for each library, copy & paste the contents of an existing library header and then change the macros that are being defined by search and replace. If your project only has a few libraries this header will not proove to be of much convenience. _(Lazzy people might want to disagree with that after the first copy without further modifications ;))_

## Usage

```cpp
void E_ABI(my_lib) foo();
```
Will evaluate to different statements depending on compile-time definitions.

The default behaviour is to _import_ symbols. To _export_ `foo` you'll have to pass `E_ABI_my_lib` as definition to the compiler (eg. `gcc -DE_ABI_my_lib ...`). If the library is build as _static_ version you can pass `E_ABI_STATIC_my_lib` since the linker will handle all symbols.

|      import      |      export      |         static          |
|:----------------:|:----------------:|:-----------------------:|
|                  |  E_ABI_my_lib    |  E_ABI_STATIC_my_lib    |
|  E_ABI_my_lib 0  |  E_ABI_my_lib 1  |  E_ABI_STATIC_my_lib 1  |
|  E_ABI_my_lib 2  |  --------------  |  ---------------------  |
|  E_ABI_my_lib a  |  --------------  |  ---------------------  |
|       ...        |  --------------  |  ---------------------  |

+ __Import__ means either `__declspec(dllimport)`, `__attribute__((dllimport))` or
  `__attribute__((visibility("default")))` depending on the platform / compiler.
+ __Export__ means either `__declspec(dllexport)`, `__attribute__((dllexport))` or
  `__attribute__((visibility("default")))` depending on the platform / compiler.
+ __Static__ means that the macro will be evaluated to nothing -- the linker will take care of everything.

To gain benefits from "exporting / importing" with gcc / clang the compile option `-fvisibility=hidden` has to be passed to the compiler. But if you didn't know that already you should head over to [this page](https://gcc.gnu.org/wiki/Visibility) and read up on this subject.

## Credits
[pfultz2](http://pfultz2.com/blog/) for his [Cloak](https://github.com/pfultz2/Cloak) library.
