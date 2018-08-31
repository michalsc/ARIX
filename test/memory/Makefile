FILES := main
OBJDIR := $(BUILD_DIR)/test/memory
TARGETDIR := $(BIN_DIR)/Test
OBJS := $(foreach f,$(FILES),$(f).o)

all: pre-build main-build

pre-build:
	@mkdir -p $(TARGETDIR) >/dev/null
	@mkdir -p $(OBJDIR) >/dev/null

main-build: pre-build
	@echo "Building AllocTest"
	@make --no-print-directory $(TARGETDIR)/AllocTest

$(TARGETDIR)/AllocTest: $(addprefix $(OBJDIR)/,$(OBJS))
	@echo "Linking AllocTest"
	@$(CC) -o $@ $(foreach f,$(OBJS),$(OBJDIR)/$(f)) -lrt -Wl,-unresolved-symbols=ignore-in-shared-libs \
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
