FILES := execbase openlibrary
OBJDIR := $(BUILD_DIR)/libs/exec
TARGETDIR := $(BIN_DIR)/Libs
OBJS := $(foreach f,$(FILES),$(f).o)

all: pre-build main-build

pre-build:
	@mkdir -p $(TARGETDIR) >/dev/null
	@mkdir -p $(OBJDIR) >/dev/null

main-build: pre-build
	@echo "Building exec.library"
	@make --no-print-directory $(TARGETDIR)/exec.library

$(TARGETDIR)/exec.library: $(addprefix $(OBJDIR)/,$(OBJS))
	@echo "Linking exec.library"
	@$(CC) -fpic -shared -Wl,-soname,exec.library -Wl,--version-script=version.script \
	 -fvisibility=hidden -o $@ $(foreach f,$(OBJS),$(OBJDIR)/$(f)) -lc -ldl

.PHONY: all
.SECONDARY: main-build pre-build

$(OBJDIR)/%.o: %.cpp
	@mkdir -p $(@D)
	@echo "Compiling: $*.cpp"
	@$(CXX) -c -fpic $(CXXFLAGS) $< -o $@

$(OBJDIR)/%.o: %.c
	@mkdir -p $(@D)
	@echo "Compiling: $*.c"
	@$(CC) -c -fpic $(CFLAGS) $< -o $@

$(OBJDIR)/%.d: %.cpp
	@mkdir -p $(@D)
	@set -e; rm -f $@; \
	 $(CXX) -MM -MT $(basename $@).o $(CXXFLAGS) $< > $@.$$$$; \
	 sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	 rm -f $@.$$$$

clean:
	rm -rf *.o *.d $(OBJDIR) $(TESTOBJDIR)

-include $(foreach f,$(OBJS:.o=.d),$(OBJDIR)/$(f))
