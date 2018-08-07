
local paths = {
    package.path;
    build:root( 'src/lalr/lalr/?.lua' );
    build:root( 'src/lalr/lalr/?/init.lua' );
};
package.path = table.concat( paths, ';' );

require 'forge';
require 'forge.cc';
require 'forge.lalr';
require 'forge.macos';
require 'forge.windows';
require 'forge.xcode';
require 'forge.visual_studio';

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
        build:root( 'src/libpng' ),
        build:root( 'src/unittest-cpp' );
        build:root( 'src/lalr/lalr' );
    };
    library_directories = {
        build:root( ("%s/lib"):format(variant) ),
    };
    visual_studio = {
        sln = build:root( "reyes.sln" );
    };
    xcode = {
        xcodeproj = build:root( "reyes.xcodeproj" );
    };
    lalr = {
        lalrc = build:switch {
            build:operating_system();
            linux = build:root( ('%s/bin/lalrc'):format(variant) );
            macos = build:root( ('%s/bin/lalrc'):format(variant) );
            windows = build:root( ('%s/bin/lalrc.exe'):format(variant) );
        };
    };
};

build:default_targets {
    'src/lalr/lalr/lalrc',
    'src/reyes',
    'src/reyes/reyes_examples',
    'src/reyes/reyes_test'
};

buildfile 'src/jpeg/jpeg.build';
buildfile 'src/lalr/lalr/lalr.build';
buildfile 'src/libpng/libpng.build';
buildfile 'src/math/math.build';
buildfile 'src/reyes/reyes.build';
buildfile 'src/unittest-cpp/unittest-cpp.build';
buildfile 'src/zlib/zlib.build';
