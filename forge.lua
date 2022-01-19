
local paths = {
    package.path;
    root( 'src/lalr/src/lalr/?.lua' );
    root( 'src/lalr/src/lalr/?/init.lua' );
};
package.path = table.concat( paths, ';' );

variant = lower( variant or 'debug' );
version = version or ('%s %s %s'):format( os.date('%Y.%m.%d %H:%M:%S'), platform, variant );

local cc = require 'forge.cc' {
    identifier = 'cc_${platform}';
    platform = operating_system();
    bin = root( ('%s/bin'):format(variant) );
    lib = root( ('%s/lib'):format(variant) );
    obj = root( ('%s/obj'):format(variant) );
    include_directories = {
        root( 'src' );
        root( 'src/zlib' );
        root( 'src/libpng' );
        root( 'src/unittest-cpp' );
        root( 'src/lalr/src' );
    };
    library_directories = {
        root( ("%s/lib"):format(variant) ),
    };
    visual_studio = {
        sln = root( "reyes.sln" );
    };
    xcode = {
        xcodeproj = root( "reyes.xcodeproj" );
    };

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
    standard = 'c++17';
    string_pooling = variant == 'shipping';
    strip = false;
    warning_level = 3;
    warnings_as_errors = true;
};

local lalr = require 'forge.lalr';
cc:install( lalr );

buildfile 'src/jpeg/jpeg.forge';
buildfile 'src/lalr/lalr.forge';
buildfile 'src/libpng/libpng.forge';
buildfile 'src/math/math.forge';
buildfile 'src/reyes/reyes.forge';
buildfile 'src/unittest-cpp/unittest-cpp.forge';
buildfile 'src/zlib/zlib.forge';

cc:all {
    'src/lalr/all',
    'src/reyes/all',
    'src/reyes/reyes_examples/all',
    'src/reyes/reyes_test/all'
};
