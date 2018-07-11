The graphics engine is windows only.
To use graphics engine the direct2d and directwrite libraries need to be linked:
-In Visual Studio, go to Project->ProjectName properties
-Go to configuration properties->Linker->Input
-In the "Additional dependencies" field, edit it and enter: dwrite.lib;d2d1.lib;

Now the linker should find the libraries and the engine will compile.
