TARGET_DIR := $(BIN_DIR)/Development/include

includes:
	@mkdir -p $(TARGET_DIR)
	@rsync -a --exclude=Makefile . $(TARGET_DIR)/

.PHONY: includes
