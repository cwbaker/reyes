
DirectoryPrototype = TargetPrototype {
	"Directory", BIND_DIRECTORY,

	build = function( self )
	    if self:is_outdated() then
	        mkdir( self:get_filename() );
	    end
	end
}

function Directory( directory )
	return target( directory, DirectoryPrototype );
end
