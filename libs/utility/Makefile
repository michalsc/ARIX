OBJNAME   := utility
OBJTYPE   := library
TARGETDIR := Libs
AUTOLIBS  := exec
LDLIBS    := c
GLOBAL    := LibBase UtilityBase
LOCAL     := *
FILES     := utility_init callhookpkt allocatetagitems freetagitems applytagchanges clonetagitems \
             filtertagchanges filtertagitems findtagitem gettagdata maptags nexttagitem packbooltags \
             packstructuretags refreshtagitemclones taginarray unpackstructuretags

include $(ROOT_DIR)/config/make_shared.mk
include $(ROOT_DIR)/config/make_bottom.mk
