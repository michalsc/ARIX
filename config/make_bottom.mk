$(_M_OBJDIR)/%.o: %.cpp
	@mkdir -p $(@D)
	@echo "  Compiling: $*.cpp"
	@$(CXX) -c $(EXTRA_CXXFLAGS) $(CXXFLAGS) $< -o $@
	touch $(_M_OBJDIR)/.compiled

$(_M_OBJDIR)/%.o: %.c
	@mkdir -p $(@D)
	@echo "  Compiling: $*.c"
	@$(CC) -c $(EXTRA_CFLAGS) $(CFLAGS) $< -o $@
	@touch $(_M_OBJDIR)/.compiled

# Special case for c files automatically generated in _M_OBJDIR
$(_M_OBJDIR)/%.o: $(_M_OBJDIR)/%.c
	@mkdir -p $(@D)
	@echo "  Compiling: $*.c"
	@$(CC) -c -fpic $(EXTRA_CFLAGS) $(CFLAGS) $< -o $@
	@touch $(_M_OBJDIR)/.compiled

$(_M_OBJDIR)/%.o: $(_M_OBJDIR)/%.cpp
	@mkdir -p $(@D)
	@echo "  Compiling: $*.cpp"
	@$(CXX) -c -fpic $(EXTRA_CXXFLAGS) $(CXXFLAGS) $< -o $@
	@touch $(_M_OBJDIR)/.compiled

$(_M_OBJDIR)/%.d: %.c
	@mkdir -p $(@D)
	@set -e; rm -f $@; \
	 $(CC) -MM -MT $(basename $@).o $(CFLAGS) $< > $@.$$$$; \
	 sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	 rm -f $@.$$$$

$(_M_OBJDIR)/%.d: %.cpp
	@mkdir -p $(@D)
	@set -e; rm -f $@; \
	 $(CXX) -MM -MT $(basename $@).o $(CXXFLAGS) $< > $@.$$$$; \
	 sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	 rm -f $@.$$$$

clean:
	rm -rf *.o *.d $(_M_OBJDIR) $(TESTOBJDIR)

-include $(foreach f,$(_M_OBJS:.o=.d),$(_M_OBJDIR)/$(f))
