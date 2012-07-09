
StaticLibraryPrototype = TargetPrototype {
    "StaticLibrary", BIND_GENERATED_FILE,

    load = function( self )
        load_module( self );
        self:set_filename( self.settings.lib.."/"..lib_name(self:id()) );
    end;

    static_depend = function( self )
        if built_for_platform_and_variant(self) then
            self:add_dependency( Directory(self.settings.lib) );
        end
    end;

    build = function( self )
        if self:is_outdated() and built_for_platform_and_variant(self) then
            build_library( self );
        end    
    end;

    clean = function( self )
        if built_for_platform_and_variant(self) then
            clean_library( self );
        end
    end;
}

function StaticLibrary( static_library )
    assert( type(static_library) == "table" );
    local id = static_library.id;
    static_library.id = nil;
    return target( id, StaticLibraryPrototype, static_library );
end
