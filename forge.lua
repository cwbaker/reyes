
local paths = {
    package.path;
    forge:root( 'src/lalr/lalr/?.lua' );
    forge:root( 'src/lalr/lalr/?/init.lua' );
};
package.path = table.concat( paths, ';' );

variant = forge:lower( variant or 'debug' );
version = version or ('%s %s %s'):format( os.date('%Y.%m.%d %H:%M:%S'), platform, variant );

local forge = require 'forge.cc' {
    identifier = 'cc_${platform}_${architecture}';
    platform = forge:operating_system();
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

    architecture = 'x86_64';
    assertions = variant ~= 'shipping';
    debug = variant ~= 'shipping';
    debuggable = variant ~= 'shipping';
    exceptions = true;
    fast_floating_point = variant ~= 'debug';
    incremental_linking = variant == 'debug';
    link_time_code_generation = variant == 'shipping';
    minimal_rebuild = variant == 'debug';
    optimization = variant ~= 'debug';
    run_time_checks = variant == 'debug';
    runtime_library = variant == 'debug' and 'static_debug' or 'static_release';
    run_time_type_info = true;
    stack_size = 1048576;
    standard = 'c++14';
    string_pooling = variant == 'shipping';
    strip = false;
    warning_level = 3;
    warnings_as_errors = true;
};

local lalr = require 'forge.lalr';
lalr.initialize( forge );

buildfile 'src/jpeg/jpeg.forge';
buildfile 'src/lalr/lalr/lalr.forge';
buildfile 'src/libpng/libpng.forge';
buildfile 'src/math/math.forge';
buildfile 'src/reyes/reyes.forge';
buildfile 'src/unittest-cpp/unittest-cpp.forge';
buildfile 'src/zlib/zlib.forge';

forge:all {
    'src/lalr/lalr/lalrc/all',
    'src/reyes/all',
    'src/reyes/reyes_examples/all',
    'src/reyes/reyes_test/all'
};
