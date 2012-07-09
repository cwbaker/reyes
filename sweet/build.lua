
command  = command or "build";
platform = platform or "msvc";
source = source or "";
goal = goal or "";
variant = variant or "debug";
version = version or os.date( "%Y.%m.%d %H:%M:%S "..platform.." "..variant );
jobs = jobs or 4;

package.path = root("build/lua/?.lua")..";"..root("build/lua/?/init.lua");
require "build";

setup {
    bin = root( "../bin" );
    lib = root( "../lib" );
    obj = root( "../obj" );
    include_directories = {
        root( ".." )
    };
    variants = {
        "debug", "release", "shipping"
    };
};

buildfile "assert/assert.build"; 
buildfile "atomic/atomic.build";
buildfile "error/error.build";
buildfile "jpeg/jpeg.build";
buildfile "math/math.build";
buildfile "png/png.build";
buildfile "pointer/pointer.build";
buildfile "render/render.build";
buildfile "unit/unit.build";
buildfile "zlib/zlib.build";

AsciiDoc {
    id = "render.html";
    "render.txt"
}

build {};
