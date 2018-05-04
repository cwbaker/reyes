
package.path = ('%s;%s;%s'):format( package.path, build:root('src/lalr/lalr/?.lua'), build:root('src/lalr/lalr/?/init.lua') );

require "build";
require "build.cc";
require "build.lalr";
require "build.macos";
require "build.windows";
require "build.xcode";
require "build.visual_studio";

platform = platform or build:operating_system();
variant = build:lower( variant or 'debug' );
version = version or ('%s %s %s'):format( os.date('%Y.%m.%d %H:%M:%S'), platform, variant );
goal = goal or "";
jobs = jobs or 8;

local settings = build:initialize {
    variants = { "debug", "release", "shipping" };
    bin = build:root( ('%s/bin'):format(variant) );
    lib = build:root( ('%s/lib'):format(variant) );
    obj = build:root( ('%s/obj'):format(variant) );
    include_directories = {
        build:root( 'src' ),
        build:root( 'src/zlib' ),
        build:root( 'src/png' ),
        build:root( 'src/unittest-cpp' );
        build:root( 'src/lalr/lalr' );
    };
    library_directories = {
        build:root( ("%s/lib"):format(variant) ),
    };
    visual_studio = {
        sln = build:root( "sweet_render.sln" );
    };
    xcode = {
        xcodeproj = build:root( "sweet_render.xcodeproj" );
    };
    lalr = {
        lalrc = build:switch {
            build:operating_system();
            linux = build:root( '%s/bin/lalrc' );
            macos = build:root( '%s/bin/lalrc' );
            windows = build:root( '%s/bin/lalrc.exe' );
        };
    };
};

build:default_targets {
    'src/lalr/lalr/lalrc',
    'src/sweet/render',
    'src/sweet/render/render_examples',
    'src/sweet/render/render_test'
};

buildfile 'src/lalr/lalr/lalr.build';
buildfile 'src/jpeg/jpeg.build';
buildfile 'src/png/png.build';
buildfile 'src/sweet/assert/assert.build';
buildfile 'src/sweet/math/math.build';
buildfile 'src/sweet/render/render.build';
buildfile 'src/unittest-cpp/unittest-cpp.build';
buildfile 'src/zlib/zlib.build';
