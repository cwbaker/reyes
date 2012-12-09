
package.path = root("build/lua/?.lua")..";"..root("build/lua/?/init.lua");
require "build";
require "build/llvmgcc";
require "build/mingw";
require "build/msvc";
require "build/Parser";
require "build/visual_studio";
require "build/xcode";

function initialize()
    platform = platform or build.switch { operating_system(); windows = "msvc"; macosx = "llvmgcc"; };
    variant = variant or "debug";
    version = version or os.date( "%Y.%m.%d %H:%M:%S "..platform.." "..variant );
    goal = goal or "";
    jobs = jobs or 4;

    local settings = build.initialize {
        bin = root( "../bin" );
        lib = root( "../lib" );
        obj = root( "../obj" );
        include_directories = {
            root( ".." )
        };
        variants = {
            "debug", "release", "shipping"
        };
        sln = root( "../sweet_render.sln" );
        xcodeproj = root( "../sweet_render.xcodeproj" );
    };

    if operating_system() == "windows" then    
        mingw.initialize( settings );
        msvc.initialize( settings );
        visual_studio.initialize( settings );
    elseif operating_system() == "macosx" then
        llvmgcc.initialize( settings );
        xcode.initialize( settings );
    end
    parser.initialize( settings );
end

function buildfiles()
    buildfile( "assert/assert.build" ); 
    buildfile( "atomic/atomic.build" );
    buildfile( "error/error.build" );
    buildfile( "jpeg/jpeg.build" );
    buildfile( "math/math.build" );
    buildfile( "png/png.build" );
    buildfile( "pointer/pointer.build" );
    buildfile( "render/render.build" );
    buildfile( "unit/unit.build" );
    buildfile( "zlib/zlib.build" );
end
