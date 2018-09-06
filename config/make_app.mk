EXTRA_CFLAGS := $(EXTRA_CFLAGS) -DVERSION_STRING_DATE='$(VERSION_STRING_DATE)'
EXTRA_CXXFLAGS := $(EXTRA_CXXFLAGS) -DVERSION_STRING_DATE='$(VERSION_STRING_DATE)'

_M_LDLIBS := $(foreach l,$(LDLIBS),-l$(l))
_M_AUTOLIBS := $(foreach l,exec $(AUTOLIBS),$(BIN_DIR)/Libs/$(l).library)
_M_OBJDIR := $(BUILD_DIR)/$(TARGETDIR)/$(OBJNAME)
_M_TARGETDIR := $(BIN_DIR)/$(TARGETDIR)
_M_OBJS := $(foreach f,$(FILES),$(f).o)

all: pre-build main-build

pre-build:
	@echo "Building $(TARGETDIR)/$(OBJNAME)"
	@mkdir -p $(_M_TARGETDIR) >/dev/null
	@mkdir -p $(_M_OBJDIR) >/dev/null

main-build: pre-build
	@make --no-print-directory $(_M_TARGETDIR)/$(OBJNAME)

$(_M_TARGETDIR)/$(OBJNAME): $(addprefix $(_M_OBJDIR)/,$(_M_OBJS))
	@echo "* Linking $(OBJNAME)"
	@$(CC) -o $@ $(foreach f,$(_M_OBJS),$(_M_OBJDIR)/$(f)) -Wl,-unresolved-symbols=ignore-in-shared-libs \
	 $(_M_LDLIBS) $(_M_AUTOLIBS)

.PHONY: all
.SECONDARY: main-build pre-build
