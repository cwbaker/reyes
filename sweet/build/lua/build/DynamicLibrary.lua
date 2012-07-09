
DynamicLibraryPrototype = TargetPrototype {
    "DynamicLibrary", BIND_GENERATED_FILE,

    load = function( self )
        load_module( self );
        self:set_filename( self.settings.bin.."/"..dll_name(self:id()) );
    end;

    static_depend = function( self )
        if built_for_platform_and_variant(self) then
            self:add_dependency( Directory(self.settings.lib) );
            self:add_dependency( Directory(self.settings.bin) );

            local libraries = {};
            if self.libraries then
                for _, value in ipairs(self.libraries) do
                    local library = find_target( root(value) );
                    assert( library, "Failed to find library '"..value.."'" );
                    if built_for_platform_and_variant(library) then
                        table.insert( libraries, library );
                        self:add_dependency( library );
                    end
                end
            end
            self.libraries = libraries;
        end
    end;

    build = function( self )
        if self:is_outdated() and built_for_platform_and_variant(self) then
            build_executable( self );
        end    
    end;

    clean = function( self )
        if built_for_platform_and_variant(self) then
            clean_executable( self );
        end
    end;
}

function DynamicLibrary( dynamic_library )
    assert( type(dynamic_library) == "table" );
    local id = dynamic_library.id;
    dynamic_library.id = nil;
    return target( id, DynamicLibraryPrototype, dynamic_library );
end
