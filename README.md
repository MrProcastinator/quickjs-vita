# `quickjs-vita` - QuickJS Javascript Engine port for Playstation Vita

`QuickJS` is a small and embeddable Javascript engine. It supports the ES2020 specification including modules, asynchronous generators, proxies and BigInt.
It optionally supports mathematical extensions such as big decimal floating point numbers (BigDecimal), big binary floating point numbers (BigFloat) and operator overloading.

## Main Features

* Small and easily embeddable: just a few C files, no external dependency, 210 KiB of x86 code for a simple hello world program.
* Fast interpreter with very low startup time: runs the 75000 tests of the ECMAScript Test Suite in about 100 seconds on a single core of a desktop PC. The complete life cycle of a runtime instance completes in less than 300 microseconds.
* Almost complete ES2020 support including modules, asynchronous generators and full Annex B support (legacy web compatibility).
* Passes nearly 100% of the ECMAScript Test Suite tests when selecting the ES2020 features. A summary is available at Test262 Report.
* Can compile Javascript sources to executables with no external dependency.
* Garbage collection using reference counting (to reduce memory usage and have deterministic behavior) with cycle removal.
* Mathematical extensions: BigDecimal, BigFloat, operator overloading, bigint mode, math mode.
* Command line interpreter with contextual colorization implemented in Javascript.
* Small built-in standard library with C library wrappers.

Alongside this port, a [C++ binding header](https://github.com/ftk/quickjspp) is provided in this repository, as quickjspp.hpp

## Installation

To compile modules for PSVita, quickjs must be compiled natively and also as a vita static library:

### Compile natively

Run the following commands:

**For Linux**

``` bash
make
sudo make install
```

**For Windows (crosscompiling with MinGW)**

``` bash
make CONFIG_WIN32=y # Use CONFIG_M32=y to compile to 32-bit binaries
sudo make install CONFIG_WIN32=y
```

This should install the following elements:
```
/usr
    /local
        /bin
            qjs
            qjsc
        /lib
            /quickjs
                libquickjs.a
                libquickjs.lto.a
        /include
            /quickjs
                quickjs.h
                quickjs-libc.h
                quickjspp.hpp
                
```

### Compile for Vita

Run the following commands:

``` bash
# Use make clean if you've already compiled for other architecture
make CONFIG_VITA=y # Use CONFIG_VITA=y to compile to PSVita static library
sudo make install CONFIG_VITA=y VITASDK=$VITASDK # Make sure you have set your $VITASDK bash variable correctly
```

This should install the following elements:
```
/usr
    /local
        /vitasdk
            /arm-vita-eabi
                /lib
                    /quickjs
                        libquickjs.a
                /include
                    /quickjs
                        quickjs.h
                        quickjs-libc.h
                        quickjspp.hpp
                
```

Supported compilation flags for PSVita

| **Command**        | **Description**
|:-------------------|:------------------------------------------
|CONFIG_VITA         | Compile for PSVita when defined (only generate static libraries, without examples)
|VITA_USE_DEBUGSCREEN | Redirects standard output and standard error to the screen using debugscreen from [samples](https://github.com/vitasdk/samples) (requires debugScreen.c)
|VITASDK | Path to the vitasdk toolchain (normally /usr/local/vitasdk), needed for automated installation

## Compiling Javascript to native code

In order to run Javascript code through QuickJS:

1. Write your Javascript in a .js file:
``` javascript
var number = 10;
console.log("This is a test, with number = " + number)
```

2. Run qjsc to generate a JS bytecode file, this should generate a .c file with a byte array:
``` bash
qjsc -c -o qjs_test.c -m test.js
```
3. Create a main.c file to initialize and run your script:
``` C
#include <quickjs/quickjs-libc.h>

extern uint32_t qjsc_test_size;
extern uint8_t* qjsc_test;

static JSContext *JS_NewCustomContext(JSRuntime *rt)
{
  JSContext *ctx = JS_NewContextRaw(rt);
  if (!ctx)
    return NULL;
  JS_AddIntrinsicBaseObjects(ctx);
  return ctx;
}

int main(int argc, char **argv)
{
  JSRuntime *rt;
  JSContext *ctx;

  rt = JS_NewRuntime();
  js_std_set_worker_new_context_func(JS_NewCustomContext);
  js_std_init_handlers(rt);

  ctx = JS_NewCustomContext(rt);

  js_std_add_helpers(ctx, argc, argv);
  js_std_eval_binary(ctx, qjsc_test, qjsc_test_size, JS_EVAL_TYPE_GLOBAL);
  js_std_loop(ctx);
  JS_FreeContext(ctx);
  JS_FreeRuntime(rt);
  return 0;
}
```

## How to use

Inside your CMakeLists.txt, add the following declarations:
``` cmake
# Add link directory
link_directories(
  ${VITASDK}/arm-vita-eabi/lib/quickjs
)

# Add quickjs library
target_link_libraries(${PROJECT_NAME}
  # QuickJS Javascript
  quickjs
  # Dependencies
  c m dl
  -Wl,--whole-archive -lpthread -Wl,--no-whole-archive
  taihen_stub
  SceSblSsMgr_stub
)
```

## PSVita differences

* Standard output (stdout) and standard error (stderr) are redirected to ux0:data/quickjs_stdout.txt and quickjs_stderr.txt respectively, unless the VITA_USE_DEBUGSCREEN flag is specified.
* Only precompiled binaries are supported.

## TODO

* Allow custom stdout and stderr file redirection.
* Add vitasdk module with vita native call bindings.
* Add more vita related samples.
* Fix setTimeout interval, as it always 10 seconds.

## Documentation

QuickJS documentation: [HTML version](https://bellard.org/quickjs/quickjs.html), [PDF version](https://bellard.org/quickjs/quickjs.pdf).

Specification of the JS Bignum Extensions: [HTML version](https://bellard.org/quickjs/jsbignum.html), [PDF version](https://bellard.org/quickjs/jsbignum.pdf).
