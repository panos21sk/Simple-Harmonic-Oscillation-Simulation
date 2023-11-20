default:
	gcc ../src/*.c -o SHO.exe -O2 -Wall -Wno-missing-braces -I ../include/ -L ../lib/desktop/ -lraylib -lopengl32 -lgdi32 -lwinmm -Wno-return-type -Wno-maybe-uninitialized -static-libgcc -static-libstdc++ -static
	#Wno suppresses 2 warnings

web:
	emcc -o SHO.html ../src/main.c -O2  -I C:/!PhysicsSimulations/SimpleHarmonicOscillations/include/ -Wall -D_DEFAULT_SOURCE -Wno-missing-braces -Wno-gnu-designator -Wno-unused-result -Os -I. -I C:/raylib/raylib/src -I C:/raylib/raylib/src/external -L. -L C:/raylib/raylib/src -s USE_GLFW=3 -s ASYNCIFY -s TOTAL_MEMORY=67108864 -s FORCE_FILESYSTEM=1 --shell-file C:/raylib/raylib/src/shell.html C:/raylib/raylib/src/web/libraylib.a -DPLATFORM_WEB -s 'EXPORTED_FUNCTIONS=["_free","_malloc","_main"]' -s EXPORTED_RUNTIME_METHODS=ccall