FILES := uuid_init
OBJDIR := $(BUILD_DIR)/libs/uuid
TARGETDIR := $(BIN_DIR)/Libs
OBJS := $(foreach f,$(FILES),$(f).o)

all: pre-build main-build

includes: $(INCLUDE_DIR)/proto/uuid.h $(INCLUDE_DIR)/clib/uuid_protos.h $(INCLUDE_DIR)/inline/uuid.h $(INCLUDE_DIR)/defines/uuid.h

pre-build:
	@mkdir -p $(TARGETDIR) >/dev/null
	@mkdir -p $(OBJDIR) >/dev/null

main-build: pre-build
	@echo "Building uuid.library"
	@make --no-print-directory $(TARGETDIR)/uuid.library

$(TARGETDIR)/uuid.library: $(addprefix $(OBJDIR)/,$(OBJS)) $(OBJDIR)/__base_file.o
	@echo "Linking uuid.library"
	@$(CC) -fpic -shared -Wl,-soname,uuid.library -Wl,--version-script=version.script \
	 -fvisibility=hidden -o $@ $(foreach f,$(OBJS),$(OBJDIR)/$(f)) $(OBJDIR)/__base_file.o -lc

.PHONY: all 
.SECONDARY: main-build pre-build

$(OBJDIR)/__base_file.c: uuid.json
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

$(OBJDIR)/%.d: %.cpp
	@mkdir -p $(@D)
	@set -e; rm -f $@; \
	 $(CXX) -MM -MT $(basename $@).o $(CXXFLAGS) $< > $@.$$$$; \
	 sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	 rm -f $@.$$$$

$(OBJDIR)/%.d: %.c
	@mkdir -p $(@D)
	@set -e; rm -f $@; \
	 $(CC) -MM -MT $(basename $@).o $(CFLAGS) $< > $@.$$$$; \
	 sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	 rm -f $@.$$$$

$(INCLUDE_DIR)/proto/uuid.h: uuid.json
	@$(SFDC) header_proto $< >$@

$(INCLUDE_DIR)/clib/uuid_protos.h: uuid.json
	@$(SFDC) header_clib $< >$@

$(INCLUDE_DIR)/inline/uuid.h: uuid.json
	@$(SFDC) header_inline $< >$@

$(INCLUDE_DIR)/defines/uuid.h: uuid.json
	@$(SFDC) header_defines $< >$@

clean:
	rm -rf *.o *.d $(OBJDIR) $(TESTOBJDIR)

-include $(foreach f,$(OBJS:.o=.d),$(OBJDIR)/$(f))
