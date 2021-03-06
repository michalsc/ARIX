/*
    Copyright © 2018 Michal Schulz <michal.schulz@gmx.de>
    https://github.com/michalsc

    This Source Code Form is subject to the terms of the
    Mozilla Public License, v. 2.0. If a copy of the MPL was not distributed
    with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/
#include <utility/tagitem.h>
#include "exec_intern.h"

struct TagItem *LibFindTagItem(Tag tagValue, const struct TagItem *tagList)
{
    struct TagItem *tstate = (struct TagItem *)tagList;
    struct TagItem *tag;

    while ((tag = LibNextTagItem(&tstate)))
    {
        if ((ULONG)tag->ti_Tag == (ULONG)tagValue)
            return tag;
    }

    return NULL;
}

IPTR LibGetTagData(Tag tagValue, IPTR defaultVal, const struct TagItem *tagList)
{
    struct TagItem *tag = LibFindTagItem(tagValue, tagList);

    return tag ? tag->ti_Data : defaultVal;
}

struct TagItem *LibNextTagItem(struct TagItem **tagListPtr)
{
    if (!(*tagListPtr))
        return NULL;

    while (1)
    {
        switch (((*tagListPtr)->ti_Tag))
        {
        case TAG_MORE:
            if (!((*tagListPtr) = (struct TagItem *)(*tagListPtr)->ti_Data))
                return NULL;
            continue;

        case TAG_IGNORE:
            break;

        case TAG_END:
            (*tagListPtr) = 0;
            return NULL;

        case TAG_SKIP:
            (*tagListPtr) += (*tagListPtr)->ti_Data + 1;
            continue;

        default:
            return (*tagListPtr)++;
        }

        (*tagListPtr)++;
    }
}

