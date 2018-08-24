LIBS := exec debug
OBJDIR := $(BUILD_DIR)/$(CURDIR)

all: $(LIBS)
$(LIBS):
	@$(MAKE) -C $@

.PHONY: all $(LIBS)