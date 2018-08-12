
local paths = {
    package.path;
    forge:root( 'src/lalr/lalr/?.lua' );
    forge:root( 'src/lalr/lalr/?/init.lua' );
};
package.path = table.concat( paths, ';' );

require 'forge';
require 'forge.cc';
require 'forge.lalr';
require 'forge.macos';
require 'forge.windows';
require 'forge.xcode';
require 'forge.visual_studio';

platform = platform or forge:operating_system();
variant = forge:lower( variant or 'debug' );
version = version or ('%s %s %s'):format( os.date('%Y.%m.%d %H:%M:%S'), platform, variant );
goal = goal or "";
jobs = jobs or 8;

local settings = forge:initialize {
    variants = { "debug", "release", "shipping" };
    bin = forge:root( ('%s/bin'):format(variant) );
    lib = forge:root( ('%s/lib'):format(variant) );
    obj = forge:root( ('%s/obj'):format(variant) );
    include_directories = {
        forge:root( 'src' ),
        forge:root( 'src/zlib' ),
        forge:root( 'src/libpng' ),
        forge:root( 'src/unittest-cpp' );
        forge:root( 'src/lalr/lalr' );
    };
    library_directories = {
        forge:root( ("%s/lib"):format(variant) ),
    };
    visual_studio = {
        sln = forge:root( "reyes.sln" );
    };
    xcode = {
        xcodeproj = forge:root( "reyes.xcodeproj" );
    };
    lalr = {
        lalrc = forge:switch {
            forge:operating_system();
            linux = forge:root( ('%s/bin/lalrc'):format(variant) );
            macos = forge:root( ('%s/bin/lalrc'):format(variant) );
            windows = forge:root( ('%s/bin/lalrc.exe'):format(variant) );
        };
    };
};

forge:default_targets {
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
