
_M_LDLIBS := $(foreach l,$(LDLIBS),-l$(l))
_M_OBJDIR := $(BUILD_DIR)/$(TARGETDIR)/$(OBJNAME)
_M_TARGETDIR := $(BIN_DIR)/$(TARGETDIR)
_M_OBJS := $(foreach f,$(FILES),$(f).o) __base_file.o

all: pre-build main-build

includes: $(INCLUDE_DIR)/proto/$(OBJNAME).h \
          $(INCLUDE_DIR)/clib/$(OBJNAME)_protos.h \
          $(INCLUDE_DIR)/inline/$(OBJNAME).h \
          $(INCLUDE_DIR)/defines/$(OBJNAME).h

pre-build:
	@mkdir -p $(_M_TARGETDIR) >/dev/null
	@mkdir -p $(_M_OBJDIR) >/dev/null

main-build: pre-build
	@echo "Building $(OBJNAME).$(OBJTYPE)"
	@make --no-print-directory $(_M_TARGETDIR)/$(OBJNAME).$(OBJTYPE)

$(_M_TARGETDIR)/$(OBJNAME).$(OBJTYPE): $(addprefix $(_M_OBJDIR)/,$(_M_OBJS)) $(_M_OBJDIR)/version.script
	@echo "Linking $(OBJNAME).$(OBJTYPE)"
	@$(CC) -fpic -shared -Wl,-soname,$(OBJNAME).$(OBJTYPE) -Wl,--version-script=$(_M_OBJDIR)/version.script \
	 -fvisibility=hidden -o $@ $(foreach f,$(_M_OBJS),$(_M_OBJDIR)/$(f)) $(_M_LDLIBS)

.PHONY: all
.SECONDARY: main-build pre-build

$(_M_OBJDIR)/version.script:
	@echo "{ global: $(foreach g,$(GLOBAL),$(g);) local: $(foreach l,$(LOCAL),$(l);) };" >$@

$(_M_OBJDIR)/__base_file.c: $(OBJNAME).json
	@$(SFDC) basefile $< >$@

$(_M_OBJDIR)/__base_file.o: $(_M_OBJDIR)/__base_file.c

$(INCLUDE_DIR)/proto/$(OBJNAME).h: $(OBJNAME).json
	@$(SFDC) header_proto $< >$@

$(INCLUDE_DIR)/clib/$(OBJNAME)_protos.h: $(OBJNAME).json
	@$(SFDC) header_clib $< >$@

$(INCLUDE_DIR)/inline/$(OBJNAME).h: $(OBJNAME).json
	@$(SFDC) header_inline $< >$@

$(INCLUDE_DIR)/defines/$(OBJNAME).h: $(OBJNAME).json
	@$(SFDC) header_defines $< >$@
