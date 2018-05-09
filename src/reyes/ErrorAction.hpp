//
// ErrorAction.hpp
// Copyright (c) 2012 Charles Baker.  All rights reserved.
//

#ifndef SWEET_RENDER_ERRORACTION_HPP_INCLUDED
#define SWEET_RENDER_ERRORACTION_HPP_INCLUDED

namespace sweet
{

namespace render
{

/**
// A mask of actions that can be taken when an error occurs or when the 
// renderer finishes a block of processing and clears the accumulated error
// count.
*/
enum ErrorAction
{
    ERROR_ACTION_IGNORE = 0x00, ///< Ignore errors.
    ERROR_ACTION_PRINT = 0x01, ///< Print errors.
    ERROR_ACTION_THROW = 0x02, ///< Throw errors.
    ERROR_ACTION_COUNT
};

}

}

#endif
