FILES := main
OBJDIR := $(BUILD_DIR)/test/debug
TARGETDIR := $(BIN_DIR)/Test
OBJS := $(foreach f,$(FILES),$(f).o)

all: pre-build main-build

pre-build:
	@mkdir -p $(TARGETDIR) >/dev/null
	@mkdir -p $(OBJDIR) >/dev/null

main-build: pre-build
	@echo "Building DebugTest"
	@make --no-print-directory $(TARGETDIR)/DebugTest

$(TARGETDIR)/DebugTest: $(addprefix $(OBJDIR)/,$(OBJS))
	@echo "Linking DebugTest"
	@$(CC) -o $@ $(foreach f,$(OBJS),$(OBJDIR)/$(f)) -Wl,-unresolved-symbols=ignore-in-shared-libs \
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
