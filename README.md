# C++17 High Performance
This is the code repository for [C++17 High Performance](https://www.packtpub.com/application-development/c17-high-performance?utm_source=github&utm_medium=repository&utm_campaign=9781787120952), published by [Packt](https://www.packtpub.com/?utm_source=github). It contains all the supporting project files necessary to work through the book from start to finish.
## About the Book
C++ is an easy language and can be used to write complex applications and performance-critical code. It has evolved and changed over the last few years. This book will guide you through optimizing the performance of your C++ apps by allowing them to consume fewer resources on the device they’re running on.

The book begins with a gentle introduction to help you identify the bottlenecks in C++. We move on to measuring performance and you’ll see how this affects the way you write code. Next you’ll understand the importance of data structure optimization and how it can be used efficiently. After that, we’ll explain which algorithm should be used to achieve faster execution, followed by how to use STL containers.

Moving on, you’ll find out how to boost your code’s performance by executing compiled code. You’ll get hands-on making use of multiple cores to enable efficient and faster execution, including GPUs. After that, you’ll learn how to improve memory management in C++. We end with some techniques you can use to optimize the performance of your apps.

## About the Code
All of the code is organized into folders. Each folder starts with a number followed by the application name. For example, Chapter02.

There are a lot of code examples in this book. Some are taken from the real world, but most of them are artificial or vastly simplified examples to prove a concept rather than providing you with production-ready code. We have put all the code examples in source files divided by chapter so that it is fairly easy to find the examples you want to experiment with. If you open up the source code files, you will note that we have replaced most of the main() functions from the examples with test cases written with Google Test framework. We hope that this will help you rather than confuse you. It allowed us to write helpful descriptions for each example, and it also makes it easier to run all the examples from one chapter at once.

In order to execute the examples you need a recent C++ compiler and CMake version 3.8 or above.

All examples use the Google Test framework (https://github.com/google/googletest) and there are some examples using functionality from Boost (http://boost.org). Both Google Test and Boost as well as other third party dependencies are installed and configured automatically by the CMake script via the Hunter package manager (https://github.com/ruslo/hunter). Note that downloading and building third party librarias can take significant amount of time.

If you want to exclude some of the chapters when building, you can comment out some of the chapters in the file CMakeList.txt located in the root of the project.

## Build Instructions for Windows/Visual Studio
Several steps are required to build the code examples.
1. Go to the folder with the code examples and create a folder named `build`:
```
cd <cpp-high-performance-repo>
mkdir build
```

2. Run CMake to generate a Visual Studio solution:
```
cmake -H. -Bbuild -G "Visual Studio 15 2017 Win64"
```
This step will download and build all third party dependecies by using Hunter. It can take a significant amount of time if you have a slow internet connection or slow computer.

3. Open the solution in Visual Studio. There you can find one Visual Studio project for each chapter and several special projects, among them ALL_BUILD and RUN_TESTS.

4. Build ALL_BUILD project, and as a result all other projects will be built automatically.

5. Build RUN_TESTS project, and as a result all other projects will be executed. The result of running the tests will be printed to the output window.

## Build instructions for macOS
All examples have been compiled and verified on macOS High Sierra using clang 5.

We have provided a CMake script that you can use for generating build files such as makefiles or Xcode projects files. To use the CMake script you need a working installation of CMake.

This book uses a lot of new features in C++17. Not all of them are fully supported by the version of clang bundled with Xcode 9.2 (which is the latests Xcode version at the time of writing). So to be able to build all examples you need to install clang 5. 

Chapter 11 is dependent on OpenCL which already comes installed with macOS. However, not all models support OpenCL, you can find more information at https://developer.apple.com/opencl and https://support.apple.com/en-us/HT202823.

The installation instructions assumes that you have installed the package manager Homebrew, see https://brew.sh.

### Install CMake
First of all you will need CMake in order to use the provided CMake scripts. Open a terminal and install CMake using Homebrew:

```
$ brew install cmake
```

### Install LLVM/Clang
You will need a recent version of the Clang compiler in order to build all examples. _Installing LLVM will take a very long time on most computers, don't give up!_ Remember to add the `--with-toolchain` option. From a terminal window, type:

```
$ brew install --with-toolchain llvm
```

### Configure CMake to use the new Clang version
In order to have CMake use the newly installed version of Clang, we need to set two environment variables: `CC` and `CXX`. If you are using Bash, the standard shell in macOS you can set the environment variables in your ```.bashrc``` file, located under your home directory. Add the following two lines to `~/.bashrc`:

```
export CC=/usr/local/opt/llvm/bin/clang
export CXX=/usr/local/opt/llvm/bin/clang++
```

The environment variables will now be set the next time you open a terminal. Or if you source the .bashrc file immediately:

```
source ~/.bashrc
```

### Install Boost and Google Test, and generate Makefiles
You are now ready to run CMake which will download and install Boost and Google Test for you using package manager Hunter. This will take a long time as well, but will only be needed the first time you run CMake. First, make sure your current directory is the root directory for the cpp-high-performance repo.

```
$ cd <cpp-high-performance-repo>
```

Next, run the `cmake` command and tell it to place project files and build files in the directory `build`:

```
$ cmake -H. -Bbuild -G "Unix Makefiles"
```

If you want to generate an Xcode project file instead, it's also possible to do:

```
$ cmake -H. -Bxcode-build -G Xcode
```

Assuming you have Xcode correctly installed, you can open the project by this command:

```
$ open xcode-build/CppHighPerformanceCodeExamples.xcodeproj
```

### Build the source code
Now you are ready to compile the example codes using the makefiles (or Xcode) generated in the `build` directory. First `cd` into the build directory:

```
$ cd build
```

Then compile the code:

```
$ make
```


### Run the example code

It's now possible to execute each chapter on its own, like this:
```
$ Chapter01/Chapter01-A_Brief_Introduction_to_C++
```

Or you can run all the examples using the command:

```
$ ctest
```



## Related Products
* [C++ Reactive Programming](https://www.packtpub.com/application-development/c-reactive-programming?utm_source=github&utm_medium=repository&utm_campaign=9781788629775)

* [C++ Data Structures and Algorithms](https://www.packtpub.com/application-development/c-data-structures-and-algorithms?utm_source=github&utm_medium=repository&utm_campaign=9781788835213)

* [C++ By Example](https://www.packtpub.com/application-development/c-example?utm_source=github&utm_medium=repository&utm_campaign=9781788391818)

### Suggestions and Feedback
[Click here](https://docs.google.com/forms/d/e/1FAIpQLSe5qwunkGf6PUvzPirPDtuy1Du5Rlzew23UBp2S-P3wB-GcwQ/viewform) if you have any feedback or suggestions.
