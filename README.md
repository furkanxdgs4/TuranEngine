# TuranEngine
My toy engine. First development target is the Renderer and UI, then Game Object and ECS-Fiber Task systems. Only tested for Windows, but I think it is easy to port to other platforms using CMake or Premake. If you want to run the TuranEditor without compiling, open bin/TuranEditor/TuranEditor.exe.

# What Is This Branch For?
The engine's master branch will use source code across multiple DLLs, my own Memory system and STL library. All of these systems have some bugs and find-fixing all the bugs is very hard. That's why I created this branch. This branch uses my own Memory Manager and C++ Standard Library STD instead of mine. Source code is only in .exe, instead of DLLs. When I think I fixed all the bugs in Refactoring branch, I will implement the changes I made in this brach to Refactoring branch. If there isn't any bug, I will merge Refactoring branch with Master branch. 
