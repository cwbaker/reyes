
require "build";
require "build.cc";
require "build.Parser";
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
    bin = build:root( ("../%s/bin"):format(variant) );
    lib = build:root( ("../%s/lib"):format(variant) );
    obj = build:root( ("../%s/obj"):format(variant) );
    include_directories = {
        build:root(),
        build:root( 'zlib' ),
        build:root( 'png' ),
    };
    library_directories = {
        build:root( ("../%s/lib"):format(variant) ),
    };
    visual_studio = {
        sln = build:root( "../sweet_render.sln" );
    };
    xcode = {
        xcodeproj = build:root( "../sweet_render.xcodeproj" );
    };
};

build:default_targets {
    'sweet/render/render_examples',
    'sweet/render/render_test'
};

buildfile "jpeg/jpeg.build";
buildfile "png/png.build";
buildfile "sweet/assert/assert.build";
buildfile "sweet/atomic/atomic.build";
buildfile "sweet/error/error.build";
buildfile "sweet/math/math.build";
buildfile "sweet/pointer/pointer.build";
buildfile "sweet/render/render.build";
buildfile "unit/unit.build";
buildfile "zlib/zlib.build";
