FILES := openlibrary allocmem freemem reschedule addhead addtail insert remhead remtail enqueue \
         findname remove closelibrary exec_init allocvec freevec availmem spawn copymem tlsf \
         createpool deletepool allocpooled freepooled reallocmem allocmemaligned allocvecaligned

OBJDIR := $(BUILD_DIR)/libs/exec
TARGETDIR := $(BIN_DIR)/Libs
OBJS := $(foreach f,$(FILES),$(f).o)

all: pre-build main-build

includes: $(INCLUDE_DIR)/proto/exec.h $(INCLUDE_DIR)/clib/exec_protos.h $(INCLUDE_DIR)/inline/exec.h $(INCLUDE_DIR)/defines/exec.h

pre-build:
	@mkdir -p $(TARGETDIR) >/dev/null
	@mkdir -p $(OBJDIR) >/dev/null

main-build: pre-build
	@echo "Building exec.library"
	@make --no-print-directory $(TARGETDIR)/exec.library

$(TARGETDIR)/exec.library: $(addprefix $(OBJDIR)/,$(OBJS)) $(OBJDIR)/__base_file.o
	@echo "Linking exec.library"
	@$(CC) -fpic -shared -Wl,-soname,exec.library -Wl,--version-script=version.script \
	 -fvisibility=hidden -o $@ $(foreach f,$(OBJS),$(OBJDIR)/$(f)) $(OBJDIR)/__base_file.o -lc -ldl

.PHONY: all
.SECONDARY: main-build pre-build

$(OBJDIR)/__base_file.c: exec.json
	$(SFDC) basefile $< >$@

$(OBJDIR)/__base_file.o: $(OBJDIR)/__base_file.c

$(OBJDIR)/%.o: %.cpp
	@mkdir -p $(@D)
	@echo "Compiling: $*.cpp"
	@$(CXX) -c -fpic $(CXXFLAGS) $< -o $@

$(OBJDIR)/%.o: %.c
	@mkdir -p $(@D)
	@echo "Compiling: $*.c"
	@$(CC) -c -fpic $(CFLAGS) $< -o $@

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

$(INCLUDE_DIR)/proto/exec.h: exec.json
	@$(SFDC) header_proto $< >$@

$(INCLUDE_DIR)/clib/exec_protos.h: exec.json
	@$(SFDC) header_clib $< >$@

$(INCLUDE_DIR)/inline/exec.h: exec.json
	@$(SFDC) header_inline $< >$@

$(INCLUDE_DIR)/defines/exec.h: exec.json
	@$(SFDC) header_defines $< >$@

clean:
	rm -rf *.o *.d $(OBJDIR) $(TESTOBJDIR)

-include $(foreach f,$(OBJS:.o=.d),$(OBJDIR)/$(f))
