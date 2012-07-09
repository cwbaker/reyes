
CcPrototype = TargetPrototype {
    "Cc", BIND_PHONY,

    static_depend = function( self )
        if built_for_platform_and_variant(self) then
            local directory = Directory( obj_directory(self) );
            self:add_dependency( directory );

            local pch = nil;
            if self.pch then
                precompiled_header = PrecompiledHeader( obj_directory(self)..obj_name(self.pch) );
                precompiled_header.header = self.pch;
                precompiled_header.unit = self;
                precompiled_header:add_dependency( directory );
                self:add_dependency( precompiled_header );
                self.precompiled_header = precompiled_header;
            end
            
            for _, value in ipairs(self) do
                local source = SourceFile( value );
                source:set_required_to_exist( true );
                source.unit = self;
                local object = File( obj_directory(self)..obj_name(value) );
                object.source = value;
                source.object = object;
                object:add_dependency( source );
                object:add_dependency( directory );
                object:add_dependency( pch );           
                self:add_dependency( object );
            end
        end
    end;

    depend = function( self )
        if built_for_platform_and_variant(self) then
            for _, value in ipairs(self) do
                local source = SourceFile( value );
                assert( source, "Failed to find source file '%s' for scanning" % tostring(value) );
                scan( source, CcScanner );
            end
        end
    end;

    build = function( self )
        if self:is_outdated() and built_for_platform_and_variant(self) then
            cc( self );
        end
    end;

    clean = function( self )
        if built_for_platform_and_variant(self) then
            for dependency in self:get_dependencies() do
                if dependency:prototype() == FilePrototype then
                    rm( dependency:path() );
                end
            end
        end
    end;
}

function Cc( cc )
    assert( cc == nil or type(cc) == "table" );
    return target( "", CcPrototype, cc );
end
