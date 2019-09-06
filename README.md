World to GPU
'World to GPU' is an open-ended exploration project for Real-time visualisation of a procedurally generated terrain using C++ and Vulkan API.

Development is guided by two main goals:
- real-time performance
- photorealism
Real-time performance is achieved using the power of modern C++ and Vulkan 3d API for optimal GPU utilization. 
Photorealism is achieved using modern rendering techniques that are real-time aproximations of physically-based approach to rendeing.

How to use WorldToGPU?
* Configure settings.json file.
* Create json scene file with Scene(link) structure. Save it in 'scenes' folder.
* Run application executable providing scene file through command line: '-s filename.json'


The product folder is the final application product which consists of:
- executable
- scenes filder
- shaders folder
- ini folder
scenes filder stores various scenes that are to be rendered
shaders folders stores shader code in spir-v format
ini folder stores various application settings files

settings.json file structure.
windowSize: initial size of the application window
fullscreen: weather application should be displayed in fulscreen mode
renderer:
appName: name of an application
appVersion: version of and application
engineName: name of an engine
engineVersion: version of an engine
framesInFlight: number of frame processed in paralel by the GPU


Scene
Scene contents include camera,  .

Scene json file structure.
asset:
version: version of the file for interpreter

camera:
Camera is defined by it's type - 'perspective' or 'ortographic', parameters and location.
The location of a camera is set with 'translation' parameter.
Common parameters: zfar, znear.
Perspective parameters: yfov.
Orthographic parameters: xmag, ymag.
Camera viewing direction is along z-axis.


The project source code consists of the following modules:
- application
- base
- platform
- renderer
- renderable
- scene
- shaders
- external