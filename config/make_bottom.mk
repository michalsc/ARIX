$(_M_OBJDIR)/%.o: %.cpp
	@mkdir -p $(@D)
	@echo "Compiling: $*.cpp"
	@$(CXX) -c -fpic $(CXXFLAGS) $< -o $@

$(_M_OBJDIR)/%.o: %.c
	@mkdir -p $(@D)
	@echo "Compiling: $*.c"
	@$(CC) -c -fpic $(CFLAGS) $< -o $@

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
