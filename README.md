# World to GPU - 3D real-time renderer leveraging the power of Vulkan API.

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
#### asset
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
##### Orthographics
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
		"xmag": 0.01,
		"ymag": 0.05
	},
	"translation": [ 0.5, 0.5, 3.0 ]
}
```  
### Development
#### Goals
The development is guided by two main goals:
- real-time performance
- photorealism
Real-time performance is achieved using the power of modern C++ and Vulkan 3d API for optimal GPU utilization. 
Photorealism is achieved using modern rendering techniques that are real-time aproximations of physically-based approach to rendeing.
