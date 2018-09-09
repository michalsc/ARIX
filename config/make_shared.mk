EXTRA_CFLAGS := $(EXTRA_CFLAGS) -fpic -DVERSION_STRING_DATE='$(VERSION_STRING_DATE)'
EXTRA_CXXFLAGS := $(EXTRA_CXXFLAGS) -fpic -DVERSION_STRING_DATE='$(VERSION_STRING_DATE)'

_M_LDLIBS := $(foreach l,$(LDLIBS),-l$(l))
_M_AUTOLIBS := $(foreach l,$(AUTOLIBS),$(BIN_DIR)/Libs/$(l).library)
_M_OBJDIR := $(BUILD_DIR)/$(TARGETDIR)/$(OBJNAME)
_M_TARGETDIR := $(BIN_DIR)/$(TARGETDIR)
_M_OBJS := $(foreach f,$(FILES),$(f).o)

all:
	@make --no-print-directory pre-build
	@make --no-print-directory build
	@make --no-print-directory main-build

includes: 
	@echo "  Generating headers for $(OBJNAME).$(OBJTYPE)"
	@make --no-print-directory $(INCLUDE_DIR)/proto/$(OBJNAME).h \
          $(INCLUDE_DIR)/clib/$(OBJNAME)_protos.h \
          $(INCLUDE_DIR)/inline/$(OBJNAME).h \
          $(INCLUDE_DIR)/defines/$(OBJNAME).h

pre-build:
	@echo "Building $(TARGETDIR)/$(OBJNAME).$(OBJTYPE)"
	@mkdir -p $(_M_TARGETDIR) >/dev/null
	@mkdir -p $(_M_OBJDIR) >/dev/null

main-build: $(addprefix $(_M_OBJDIR)/,$(_M_OBJS) __base_file.o) $(_M_OBJDIR)/version.script
	@make --no-print-directory $(_M_TARGETDIR)/$(OBJNAME).$(OBJTYPE)

build: $(addprefix $(_M_OBJDIR)/,$(_M_OBJS)) $(_M_OBJDIR)/version.script

$(_M_TARGETDIR)/$(OBJNAME).$(OBJTYPE): $(addprefix $(_M_OBJDIR)/,$(_M_OBJS) __base_file.o) $(_M_OBJDIR)/version.script
	@echo "* Linking $(OBJNAME).$(OBJTYPE)"
	@$(CC) -fpic -shared -Wl,-soname,$(OBJNAME).$(OBJTYPE) -Wl,--version-script=$(_M_OBJDIR)/version.script \
	 -fvisibility=hidden -o $@ $(foreach f,$(_M_OBJS) __base_file.o,$(_M_OBJDIR)/$(f)) $(_M_LDLIBS) $(_M_AUTOLIBS)
	@rm -rf $(_M_OBJDIR)/.compiled

.PHONY: all
.SECONDARY: main-build pre-build build build_with_version $(_M_OBJDIR)/.compiled

$(_M_OBJDIR)/version.script:
	@echo "{ global: $(foreach g,$(GLOBAL),$(g);) local: $(foreach l,$(LOCAL),$(l);) };" >$@

$(_M_OBJDIR)/__base_file.c: $(OBJNAME).json $(_M_OBJDIR)/.compiled
	@$(SFDC) basefile $< >$@

$(INCLUDE_DIR)/proto/$(OBJNAME).h: $(OBJNAME).json
	@$(SFDC) header_proto $< >$@

$(INCLUDE_DIR)/clib/$(OBJNAME)_protos.h: $(OBJNAME).json
	@$(SFDC) header_clib $< >$@

$(INCLUDE_DIR)/inline/$(OBJNAME).h: $(OBJNAME).json
	@$(SFDC) header_inline $< >$@

$(INCLUDE_DIR)/defines/$(OBJNAME).h: $(OBJNAME).json
	@$(SFDC) header_defines $< >$@
