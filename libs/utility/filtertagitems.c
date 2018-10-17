/*
    Copyright © 1995-2001, The AROS Development Team. All rights reserved.
    $Id$

    Desc: FilterTagItems() - filter an array of TagItems.
    Lang: english
*/

#include <clib/utility_protos.h>
#include <exec/memory.h>
#include <proto/exec.h>

/**
 *  NAME
 *      FilterTagItems
 * 
 *  SYNOPSIS
 *      ULONG FilterTagItems(struct TagItem * tagList, Tag * filterArray, ULONG logic)
 * 
 *  FUNCTION
 *      Scans a tag list and removes tag items from the list depending
 *      upon whether the tag's Tag value is found in an array of tag
 *      values.
 *
 *      If 'logic' is TAGFILTER_AND, then all the tags that are NOT
 *      in the array filterArray will be removed from the tagList.
 *
 *      If 'logic' is TAGFILTER_NOT, then all the tags that ARE in
 *      the array filterArray will be removed from the tagList.
 *
 *      Tags are removed by setting their ti_Tag value to TAG_IGNORE.
 *
 *  INPUTS
 *      tagList         -   A TagList to filter items from.
 *      filterArray     -   An array (as described by TagInArray())
 *                          to determine which tag items are to be
 *                          removed.
 *      logic           -   Whether the tags in filterArray are to be
 *                          included or excluded from the tag list.
 *
 *  RESULT
 *      The number of valid items left in the resulting filtered list.
 *
 *  NOTES
 *
 *  EXAMPLE
 *
 *  BUGS
 *
 *  SEE ALSO
 *      TagInArray()
 *
 *  INTERNALS
 *
 *  HISTORY
 *      29-10-95    digulla automatically created from
 *                          utility_lib.fd and clib/utility_protos.h
 */
ULONG FilterTagItems(struct TagItem *tagList, Tag *filterArray, ULONG logic)
{
    ULONG valid = 0;
    if(tagList && filterArray)
    {
        struct TagItem *ti;

        while((ti = NextTagItem(&tagList)))
        {
            if(logic == TAGFILTER_AND)
            {
                if(TagInArray(ti->ti_Tag, filterArray))
                    valid++;
                else
                    ti->ti_Tag = TAG_IGNORE;
            }
            else if(logic == TAGFILTER_NOT)
            {
                if(TagInArray(ti->ti_Tag, filterArray))
                    ti->ti_Tag = TAG_IGNORE;
                else
                    valid++;
            }
        }
    }
    return valid;
} /* FilterTagItems */