# TuranEngine
My toy engine. First development target is the Renderer and UI, then Game Object and ECS-Fiber Task systems. Only tested for Windows, but I think it is easy to port to other platforms using CMake or Premake. If you want to run the TuranEditor without compiling, open bin/TuranEditor/TuranEditor.exe.

# How to build
You can download the project and build it without any issue. Project runs well, but content pipeline uses exact file directories so you can't use it for now. 

# What Is This Branch For?
The engine's master branch will use source code across multiple DLLs, my own Memory system and STL library. All of these systems have some bugs and find-fixing all the bugs is very hard. That's why I created this branch. This branch uses my own Memory Manager and C++ Standard Library STD instead of mine. Source code is only in .exe, instead of DLLs. When I think I fixed all the bugs in Refactoring branch, I will implement the changes I made in this brach to Refactoring branch. If there isn't any bug, I will merge Refactoring branch with Master branch. 


# Changelog:
13/07/2020: Memory Manager gives an already allocated empty memory block when requested size is greater than %60 of the memory block's size. Texture loading and Content Browser had some basic problems, fixed. Asset system has completely changed, Resource_Identifier objects stores everything about the asset (ID, File Path, Asset's Data if loaded, Type of the object) and all proccesses uses these identifiers to reference assets. With this new Asset system, asset's data itself doesn't know if it's an asset, I moved all save-load-verify code to TuranEditor's FileSystem folder. With that way, any asset type can be added or modified without compiling the whole project. And I'm modifying Material Type creation proccess.

14/07/2020: Material Type is changed. It's just a link of vertex and fragment shader. Shader Sources can be seen in Content Browser and functionalities like creating-compiling-saving-verifying-seeing properties are coded. Because Material Type is changed, it's creation editor is changed too. Also Vertex Attribute Layout stores VertexAttributes as objects instead of pointers. IMGUI supports Tabs and Seperators. GFXContentManager had problems about Material Type, fixed.

17/07/2020: After a break. Assimp loads all of the Vertex Attributes now (All texture and vertex color sets, tangents etc.). Unloading resources from GPU works well. Texture Loader had been importing textures with 3 channels, fixed. Texture's channel type is changable in Properties window. Camera components' properties can be seen in Scene Editor and are editable but you should save the changes or changes are gone after editor's termination. Static Model's Vertex Attribute info is seenable in Properties window.