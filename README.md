# World to GPU - 3D real-time renderer leveraging the power of Vulkan API.
## Table of contents
* [General info](#general-info)
* [Technologies](#technologies)
* [Usage](#usage)
	* [Usage example](#usage-example)
	* [For developers](#for-developers)
* [Scene file description](#scene-file-description)
	* [Asset](#asset)
	* [Cameras](#cameras)
		* [Perspective](#perspective)
		* [Orthographic](#orthographic)
* [Settings file description](#settings-file-description)
* [Dependancies](#dependancies)
* [Development](#development)
	* [Goals](#goals)
	* [Shader compilation](#shader-compilation)
	* [Code organization and design philosophy](#code-organization-and-design-philosophy)
		* [External](#external)
		* [Application](#application)
		* [Platform](#platform)
		* [Base](#base)
		* [Renderer](#renderer)
		* [Renderable](#renderable)
		* [Scene](#scene)
	* [Module interaction scheme](#module-interaction-scheme)
* [Author](#author)
## General info
*Vulkan* is a new powerful low-level graphics API that allows developers to design high-performance rendering engines in 
a way that is different from the way it was done before. But with great power come great responsibilities.
*World2GPU* project explores how the flexibility of a low-level rendering API can be put to use when designing real-time high-performance
graphical applications. 
### Technologies
*World2GPU* is developed in C++ and compiled with C++17 compatable compiler.
The application is tested with Visual Studio 2019, however, any C++17 compiler should work. 
*Vulkan* is used as a rendering API.
### Usage
Vulkan 1.0+ compatable graphics card and drivers should be installed on a machine.  
To use *World2GPU* application, launch executable file and provide command line arguments to specify
file name of a scene to be rendered *(option -s)*.
Scene description file should be present in *'/scenes'* folder.
Application settings are stored in *'ini/settings.json'* file.
#### Usage example
To start rendering the scene described in 'scenes/scene_alpha.json' file write:
> WorldToGPU.exe -s scene_alpha.json
#### For developers
In Visual Studio: after cloning the repo go to *'Project properties'* and select *'Debugging'* section:  
- Add *'Command arguments'* as described above.
- Set *'Working directory'* to location of the application .exe file.
### Scene file description
Scenes are described in *.json* files with the following pseudo-structure:
```javascript
{
	"asset": {
		"version": "1.0"
	},

	"camera": {
		...	
	}
}
```  
Each of the items are described below.
#### Asset
Used as a meta-information of the scene file.
```javascript
"version": "1.0"
```    
Version of the file structure. Used internally to identify the file.
#### Cameras
Cameras define the vantage points of the scene. There can be only one camera per scene.  
There are two camera types:  
- Perspective
- Orthographics  
##### Perspective
To describe perspective camera use:  
- *'type'* property defines the type of the camera.
- *'translation'* describes the position of the camera in 3d space.
- *'yfov'* property defines vertical field-of-view of the camera and is defined in radians.  
- *'zfar'* far clipping plane.
- *'znear'* near clipping plane.

```javascript
"camera": {
	"type": "perspective",
	"perspective": {
		"yfov": 0.7,
		"zfar": 100,
		"znear": 0.01
	},
	"translation": [ 0.5, 0.5, 3.0 ]
}
```  
##### Orthographic
To describe orthographic camera use:  
- *'type'* property defines the type of the camera.
- *'translation'* describes the position of the camera in 3d space.
- *'zfar'* far clipping plane.
- *'znear'* near clipping plane.  
- *'xmag'* width af the viewing volume.
- *'ymag'* height af the viewing volume.

```javascript
"camera": {
	"type": "orthographic",
	"orthographic": {
		"yfov": 0.7,
		"zfar": 100,
		"xmag": 1.0,
		"ymag": 1.5
	},
	"translation": [ 0.5, 0.5, 3.0 ]
}
```  
### Settings file description
Setting describe properties of the application. The file *settings.json* is located in *'/ini'* folder
relative to projects executable file. The description of the properties is as follows:
```javascript
{
"windowSize": [ 800, 600 ],
"fullscreen": false,
"renderer": {
	"appName": "World to GPU",
	"appVersion": 1,
	"engineName": "W2GPU",
	"engineVersion": 1,
	"framesInFlight": 2
}
}
``` 
- *'windowSize'* size of the output screen in pixels.
- *'fullscreen'* whether the application runs in fullscreen mode.
- *'appName'* name of the application. Used internally as identifier and as an output window title. 
- *'appVersion'* version of the application. 
- *'engineName'* name of the engine that this application uses. Used by Vulkan drivers to optimize if it is
well known engine. Has no effect in our case.
- *'engineVersion'* version of the engine used.
- *'framesInFlight'* technical parameter that describes how many frames are processed simultaneously by the GPU. Can 
affect performance.
### Dependancies
The dependancies are:  
- Vulkan SDK.
- GLFW multi-platform library for creating window, loading Vulkan, receiving input.
- [GLM](https://github.com/g-truc) optimized linear math library.
- [JSON](https://github.com/nlohmann) library.
- [AMD](https://github.com/amd) vulkan memory allocation library.  

All libraries are included in the project.
### Development
#### Goals
The development is guided by two main goals:
- real-time performance
- photorealism  

Real-time performance is achieved using the power of modern C++ and Vulkan 3d API for optimal GPU utilization. 
Photorealism is achieved using modern rendering techniques that are real-time aproximations of physically-based approach to rendeing.
#### Shader compilation
Shaders are written in GLSL language Vulkan *dialect*. Source code for the shaders is in *'/shader'* folder
in project root directory. Shaders are compiled to *.spv* format using *glslangValidator.exe* that can be found in VulkanSDK and 
are then placed in *'/shaders'* folder in *'/product'* directory.
#### Code organization and design philosophy
Project source code is divided into the following modules. Each module is placed in separate subfolder in *'/src'* directory.
##### External
Contains external libraries that are used in the project.
##### Application
The main application container. Stores and runs main modules of the application. There can be multiple applications
defined but only can be active at a time.
##### Platform
Contains platform related functionality. Defines base class for all applications that are to be defined. This base class
provides basic OS related functionality that is common among different applications and has no need to be duplicated.
##### Base
Lowest level module that provides basic type definitions and common functionality like logging, file loading, parsing etc.
All other modules are built on top of it.
##### Renderer
Renderer module is concerned with rendering whatever it is told to. It is *blind* to the rest of the engine in a *sence* that
it knows nothing about the objects to be rendered and nothing about the properties of the scene. All objects, however, *use* the renderer
to draw themselves.  
Renderers source code, by and large, consists of Vulkan API abstractions and Vulkan concopt abstractions
that are not dependant on the underlying API. 
##### Renderable
Renderables are objects to be rendered by the application. All objects have base class that they inherit from which allows
renderables to be processed in a common way. Renderables store information about themselves and *know* how to use *'renderer'*
module to render themselves.
##### Scene
This module is concerned with organization of the scene and delegation of the tasks to various parts of the engine.
'Scene manager' can be thought as the *'king'* submodule of the engine for it tells other submodules what to *do*
and how to *act*. It is the highest level submodule in the engine.  
Scene submodule itself merely generates and stores the scene description.  
Camera definitions are also contained in the *scene* module.
#### Module interaction scheme
<img src="drawing.png" alt="Design scheme" width = "500"/> 
## Author
Designed and developed by [Ivars Rb.](https://github.com/ivarsrb)