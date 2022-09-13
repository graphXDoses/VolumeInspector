# Mobius Engine

List of contents:
- [Overview](#overview)
- [External dependencies](#external-dependencies)
- [Features](#features)
- [Plans for the future](#plans-for-the-future)

## Overview
The Mobius engine is primarily an early-stage interactive application and rendering engine for Windows (64bit). While still lacking some of the desirable features a complete engine might have on today's standards, such as compatibility with many systems or a plethora of integrated tools, the Mobius engine is still usable for small projects and its potential for further development is very high.

## External dependencies
The engine depends on the following:
- [Glad](https://glad.dav1d.de/) - Multi-Language Vulkan/GL/GLES/EGL/GLX/WGL Loader-Generator based on the official specs.
- [GLFW](https://github.com/glfw/glfw) - Multi-platform library for OpenGL application development.
- [GLM](https://github.com/g-truc/glm) - C++ mathematics library for graphics software based on the OpenGL Shading Language (GLSL) specifications.
- [Dear ImGui](https://github.com/ocornut/imgui/tree/docking) - Bloat-free graphical user interface library for C++. (Docking Branch)
- [spdlog](https://github.com/gabime/spdlog) - Very fast, header only, C++ logging library.
- [stb_image](https://github.com/nothings/stb) - Image loader library.

## Features
Originally developed to support the VolumeInspector project as its backend, the engine has integrated its own rendering API. The API is currently wrapping around OpenGL functions and constants, but it is designed with the capability of incorporating both DirectX and OpenGL graphics, as a unified interface. Furthermore, it utilizes the GLFW library to provide basic window control and customization, while synergizing with the Dear ImGui library for easy access and itegration of UI graphical elements to the window. User interaction and general input handling is also done via a dedicated, layering system for events. Last but not least, memory efficiency is maintained and management of resources is done automatically through the use of smart pointers.

## Plans for the future
There are plenty of features to enhance the engine. Some of them are:
- DirectX functions and constants. integration with current API.
- DDS support for 2D and 3D textures.
- Recording Logs.
- Material system. Basic integrated shaders, lighting, shadow maps, skybox.
- Introduce audio API.
