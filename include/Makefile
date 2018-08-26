TARGET_DIR := $(BIN_DIR)/Development/include

includes:
	@mkdir -p $(foreach f,proto defines inline clib, $(TARGET_DIR)/$(f))
	@rsync -am --exclude=Makefile . $(TARGET_DIR)/

.PHONY: includes
