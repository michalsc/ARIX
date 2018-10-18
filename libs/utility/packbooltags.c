/*
    Copyright © 1995-2001, The AROS Development Team. All rights reserved.
    $Id$

    Desc:
    Lang: english
*/

#include <clib/utility_protos.h>
#include <exec/memory.h>
#include <proto/exec.h>

/**
 *  NAME
 *      PackBoolTags
 * 
 *  SYNOPSIS
 *      ULONG PackBoolTags(ULONG initialFlags, struct TagItem * tagList, struct TagItem * boolMap)
 * 
 *  FUNCTION
 *      Scans through the list tagList to find the tags which are contained
 *      in the list boolMap which are then converted to a bit-flag
 *      representation as defined in boolMap.
 *
 *      If the value of the Tag's data is 0, then the boolean value of the
 *      tag is defined as false, otherwise it is true.
 *
 *  INPUTS
 *      initialFlags -  an initial set of bit-flags which will be changed
 *                      by this function.
 *
 *      tagList      -  A TagItem list which contains some tags which are
 *                      defined as boolean by having a corresponding tag
 *                      in boolMap. The boolean value of tag->ti_Data
 *                      determines whether the bits in the flag are
 *                      TRUE or FALSE.
 *
 *      boolMap      -  A TagItem list containing a series of tags which
 *                      are to be considered Boolean.
 *
 *  RESULT
 *      flags        -  The value of initialFlags modified by the values
 *                      of the boolean tags defined in boolMap.
 *
 *  NOTES
 *      If there is more than one Tag in tagList of a single type. The
 *      last of these tags will determine the value of that bit-flag.
 *
 *  EXAMPLE
 *
 *  BUGS
 *
 *  SEE ALSO
 *      GetTagData(), FindTagItem(), NextTagItem()
 *
 *  INTERNALS
 *
 *  HISTORY
 *      29-10-95    digulla automatically created from
 *      utility_lib.fd and clib/utility_protos.h
 *      18-08-96    iaint   Created. But still needs some testing.
 */
ULONG PackBoolTags(ULONG initialFlags, struct TagItem *tagList, struct TagItem *boolMap)
{
    struct TagItem *current, *found, *tstate = tagList;

    while ((current = NextTagItem(&tstate)))
    {
        if ((found = FindTagItem (current->ti_Tag, boolMap)))
        {
            if (current->ti_Data == 0)
                initialFlags &= ~(found->ti_Data);
            else
                initialFlags |= found->ti_Data;
        }
    }

    return initialFlags;
} /* PackBoolTags */
