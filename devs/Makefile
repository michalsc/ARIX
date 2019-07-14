DEVS := timer
OBJDIR := $(BUILD_DIR)/$(CURDIR)

includes:
	@$(foreach dir, $(DEVS), make -C $(dir) includes;)

all: $(DEVS)
$(DEVS):
	@$(MAKE) -C $@

.PHONY: all includes $(DEVS)