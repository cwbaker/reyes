
AsciiDocPrototype = TargetPrototype {
    "AsciiDoc", BIND_GENERATED_FILE,

    load = function( self )
        if self.settings then
            setmetatable( self.settings, {__index = settings} );
        else
            self.settings = settings;
        end
    end;

    static_depend = function( self )
        local source = SourceFile( self[1] );
        source:set_required_to_exist( true );
        self.source = self[1];
        self:add_dependency( source );
    end;

    document = function( self )
        if self:is_outdated() and built_for_platform_and_variant(self) then
            local python = self.settings.python.executable;
            local asciidoc = self.settings.asciidoc.executable;
            local conf_file = self.settings.asciidoc.conf_file;
            local attributes = " ";
            if self.attributes then
                for attribute, value in pairs(self.attributes) do
                    if type(value) == "boolean" then
                        attributes = "%s-a %s " % {attributes, attribute};
                    else
                        attributes = "%s-a %s=%s " % {attributes, attribute, value};
                    end
                end
            end
            print( self.source );
            local arguments = "python %s -f %s%s%s" % {asciidoc, conf_file, attributes, self.source};
            if execute(python, arguments) ~= 0 then
                error( arguments.." failed" );
            end
        end
    end;
}

function AsciiDoc( asciidoc )
    assert( type(asciidoc) == "table" );
    local id = asciidoc.id;
    asciidoc.id = nil;
    return target( id, AsciiDocPrototype, asciidoc );
end
