ifndef Project
 $(error "undefined $$Project")
endif

ifndef Sources
 $(error "undefined $$Sources")
endif

BinaryDir?=bin
ObjectDir?=$(BinaryDir)

Target?=$(BinaryDir)/$(Project)

Objects=$(Sources:.cpp=.o)

.PHONY: all clean distclean

all: $(Target)

$(Target): $(Objects)
	$(MKDIR) $(BinaryDir)
	$(CXX) -o $@ $^

.cpp.o: %.cpp %.hpp
	$(CXX) -c -I include -o $@ $<

clean:
	$(RM) $(Objects)

distclean: clean
	$(RM) $(_Target)
	$(RM) -r $(BinaryDir)
	$(RM) -r $(ObjectDir)
