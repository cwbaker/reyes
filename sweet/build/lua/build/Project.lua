
ProjectPrototype = TargetPrototype {
    "Project", BIND_GENERATED_FILE,

    load = function( self )
        local filename = string.format( "%s.vcproj", basename(self:id()) );
        self:set_filename( absolute(filename) );
        
        local working_directory = self:get_working_directory();
        for target in working_directory:get_targets() do
            if string.match(target:id(), ".*%.build")  then
                self:add_dependency( target );
            end            
        end

        self:add_dependency( SourceFile(root("build/lua/build/visual_studio.lua")) );
    end;

    generate = function( self )
        if self:is_outdated() then
            generate_visual_studio_project( self:get_filename(), self.module );
        end
    end;
}

function Project( project )
    assert( type(project) == "string" );
    return target( project, ProjectPrototype );
end
