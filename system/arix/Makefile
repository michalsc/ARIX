FILES := arix_start publicport
OBJDIR := $(BUILD_DIR)/system/arix
TARGETDIR := $(BIN_DIR)/System
OBJS := $(foreach f,$(FILES),$(f).o)
CFLAGS := $(CFLAGS) $(shell TAG=v60.0 ./get_git_version.sh) 
CXXFLAGS := $(CXXFLAGS) $(shell TAG=v60.0 ./get_git_version.sh) 
all: pre-build main-build

pre-build:
	@mkdir -p $(TARGETDIR) >/dev/null
	@mkdir -p $(OBJDIR) >/dev/null

main-build: pre-build
	@echo "Building ARIX"
	@make --no-print-directory $(TARGETDIR)/ARIX

$(TARGETDIR)/ARIX: $(addprefix $(OBJDIR)/,$(OBJS))
	@echo "Linking ARIX"
	@$(CXX) -o $@ $(foreach f,$(OBJS),$(OBJDIR)/$(f)) -Wl,-unresolved-symbols=ignore-in-shared-libs \
	 $(BIN_DIR)/Libs/exec.library

.PHONY: all
.SECONDARY: main-build pre-build

$(OBJDIR)/%.o: %.cpp
	@mkdir -p $(@D)
	@echo "Compiling: $*.cpp"
	@$(CXX) -c $(CXXFLAGS) $< -o $@

$(OBJDIR)/%.o: %.c
	@mkdir -p $(@D)
	@echo "Compiling: $*.c"
	@$(CC) -c $(CFLAGS) $< -o $@

$(OBJDIR)/%.d: %.c
	@mkdir -p $(@D)
	@set -e; rm -f $@; \
	 $(CC) -MM -MT $(basename $@).o $(CFLAGS) $< > $@.$$$$; \
	 sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	 rm -f $@.$$$$

$(OBJDIR)/%.d: %.cpp
	@mkdir -p $(@D)
	@set -e; rm -f $@; \
	 $(CXX) -MM -MT $(basename $@).o $(CXXFLAGS) $< > $@.$$$$; \
	 sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	 rm -f $@.$$$$

clean:
	rm -rf *.o *.d $(OBJDIR) $(TESTOBJDIR)

-include $(foreach f,$(OBJS:.o=.d),$(OBJDIR)/$(f))
