/*
    Copyright © 1995-2017, The AROS Development Team. All rights reserved.
    $Id$

    Desc: RefreshTagItemClones()
    Lang: english
*/

#include <clib/utility_protos.h>
#include <exec/memory.h>
#include <proto/exec.h>

/**
 *  NAME
 *      RefreshTagItemClones
 * 
 *  SYNOPSIS
 *      void RefreshTagItemClones(struct TagItem * clone, const struct TagItem * original)
 * 
 *  FUNCTION
 *      If (and only if) the Tag list 'clone' was created by calling
 *      CloneTagItems on the Tag list 'original', and the list original
 *      has NOT been changed in any way, then this function will change
 *      the list 'clone' back to its original state.
 *
 *  INPUTS
 *      original    - The source TagList (unaltered)
 *      clone       - The destination TagList (MUST be allocated by
 *                    CloneTagItems())
 *
 *  RESULT
 *      The second TagList now has the same values as the first.
 *
 *  NOTES
 *      If either of the inputs is NULL, then the function will not do
 *      anything.
 *
 *  EXAMPLE
 *      struct TagItem *orig, clone;
 *
 *      \* TagList orig has some values already *\
 *      clone = CloneTagList( orig );
 *
 *      \* In between here we do something to the TagItems in clone,
 *      but we need to have them restored.
 *      *\
 *
 *      RefreshTagItemClones( clone, orig );
 *
 *  BUGS
 *      None, however if either of the two pre-conditions is not fulfilled
 *      then this function will probably be unreliable, or trash memory.
 *
 *      We warned you...
 *
 *  SEE ALSO
 *      CloneTagItems()
 *
 *  INTERNALS
 */
void RefreshTagItemClones(struct TagItem *clone, const struct TagItem *original)
{
    struct TagItem *current;

    if( !clone )
        return;

    /* If we don't have an original, but do have a clone, set the
       clone to have a definite end of TagList.

       This is because CloneTagItems(NULL) is valid.
    */
    if(!original)
    {
        clone->ti_Tag = TAG_DONE;
        return;
    }

    /* Remember, the clone list is a straight memory block, however
       the original list may not be.
    */
    while ((current = NextTagItem ((struct TagItem **)&original)))
    {
        *clone = *current; /* Copies both tag and data */
        clone++;
    }
} /* RefreshTagItemClones */
