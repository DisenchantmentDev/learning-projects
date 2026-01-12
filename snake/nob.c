#define NOB_IMPLEMENTATION

#include "./src_build/nob.h"

#define BUILD_FOLDER "build/"
#define SRC_FOLDER "src/"
#define RAYLIB_SRC_FOLDER "raylib/src/"

int
main (int argc, char **argv)
{
    NOB_GO_REBUILD_URSELF (argc, argv);

    Nob_Cmd cmd = { 0 };

    nob_cmd_append (&cmd, "gcc", "src/snake.c", "-Iraylib/src",
                    "raylib/src/libraylib.a", "-o", "snake", "-lm",
                    "-lpthread", "-ldl", "-lrt", "-lX11");

    if (!nob_cmd_run (&cmd))
        return 1;
    return 0;

    // nob_cmd_append (&cmd, "gcc", "-Wall", "-Wextra", "-ggdb", "-I.",
    //                 "-I" RAYLIB_SRC_FOLDER, "-o", "./build/snake",
    //                 "./src/snake.c", "-l:libraylib.a");
}
