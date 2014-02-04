EXECUTABLE = game

CPP = g++

CPPFLAGS = -c -Wall -O3 -std=c++98 -I ./include

LIBS = -lX11

SRCDIR = src
BUILDDIR = build

SRCS = $(wildcard $(SRCDIR)/*.cpp)
OBJS = $(patsubst $(SRCDIR)/%.cpp,$(BUILDDIR)/%.o,$(SRCS))
DEPS = $(OBJS:%.o=%.d)

all: $(EXECUTABLE)
	@echo -n "Running executable..."
	./$(EXECUTABLE)
	@echo "   ...Done."

$(EXECUTABLE): $(OBJS) 
	@echo -n "Building executable..."
	@$(CPP) -o $(EXECUTABLE) $(OBJS) $(LIBS)
	@echo "   ...Done."

-include $(DEPS)

$(BUILDDIR)/%.o : $(SRCDIR)/%.cpp
	@echo -n "Building $<..."
	@mkdir -p $(dir $@)
	@$(CPP) -MM $(CPPFLAGS) $(LIBS) $< > $(BUILDDIR)/$*.d
	@mv -f $(BUILDDIR)/$*.d $(BUILDDIR)/$*.d.tmp
	@sed -e 's|.*:|$(BUILDDIR)/$*.o:|' < $(BUILDDIR)/$*.d.tmp > $(BUILDDIR)/$*.d
	@sed -e 's/.*://' -e 's/\\$$//' < $(BUILDDIR)/$*.d.tmp | fmt -1 | \
	    sed -e 's/^ *//' -e 's/$$/:/' >> $(BUILDDIR)/$*.d
	@rm -f $(BUILDDIR)/$*.d.tmp
	@$(CPP) $(CPPFLAGS) $(LIBS) $< -o $@
	@echo "   ...Done."

clean:
	@echo -n "Removing build directory and executable..."
	@rm -rf $(BUILDDIR)
	@rm -f $(EXECUTABLE)
	@echo "   ...Done."
