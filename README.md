# SlateProjects
Note: This project does not work fully as the URLs mentioned for RESTful API calls are private.
1. Create a C++ Unreal project and Add a new Editor Standalone window plugin to the project by giving a name to it.
2. Modify the Build.cs file of the plugin by mentioning the public dependency modules.
3. Change the plugin type from Default to PreDefault in the .uplugin file.
4. Add your slate widget file reference (the C++ class created for the UI) in the <PluginName>.cpp file in the OnTabSpawn function.
