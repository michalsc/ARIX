/*
    Copyright © 1995-2001, The AROS Development Team. All rights reserved.
    $Id$

    Desc: FilterTagChanges() - filter unchanged tags from a list.
    Lang: english
*/

#include <clib/utility_protos.h>
#include <exec/memory.h>
#include <proto/exec.h>

/**
 *  NAME
 *      FilterTagChanges
 * 
 *  SYNOPSIS
 *      void FilterTagChanges(struct TagItem * changeList, const struct TagItem * originalList, BOOL apply)
 * 
 *  FUNCTION
 *      This function will scan through changeList, and if an item in
 *      changeList exists in originalList, but both items data values
 *      are equal, then the item in changeList will be removed from the
 *      list.
 *
 *      If the value of apply is TRUE, then if the datas are different
 *      then the values in originalList will be updated to match those
 *      in changeList.
 *
 *  INPUTS
 *      changeList      -   List of new tags (may be NULL).
 *      originalList    -   List of existing tags (may be NULL).
 *      apply           -   Boolean flag as to whether the values in
 *                          originalList should be updated to match
 *                          those in changeList.
 *
 *  RESULT
 *      The changeList will be modified to show altered items, and if
 *      requested, the originalList will be updated.
 *
 *  NOTES
 *
 *  EXAMPLE
 *
 *  BUGS
 *
 *  SEE ALSO
 *      ApplyTagChanges()
 *
 *  INTERNALS
 *
 *  HISTORY
 *      29-10-95    digulla automatically created from
 *                          utility_lib.fd and clib/utility_protos.h
 */
void FilterTagChanges(struct TagItem *changeList, const struct TagItem *originalList, BOOL apply)
{
    if (originalList && changeList)
    {
        struct TagItem *change, *orig;

        while ((change = NextTagItem(&changeList)))
        {
            if ((orig = FindTagItem(change->ti_Tag, (struct TagItem *)originalList)))
            {
                if (change->ti_Data == orig->ti_Data)
                {
                    change->ti_Tag = TAG_IGNORE;
                }
                else
                {
                    if (apply)
                        orig->ti_Data = change->ti_Data;
                }
            } /* if (FindTagItem()) */
        } /* while (changeList++) */
    } /* if (lists are both valid) */
} /* FilterTagChanges */