################################################################################
### CUSTOMIZE BELOW HERE #######################################################

TARGET=runConfTest # define the name of the executable


SOURCES=$(wildcard *.cpp ../src/*.cpp   )

CXXFLAGS   += -g -uClibc++ -std=c++11 

CPPFLAGS   += -I../../json/include/  \
              

LDLIBS    += -lpthread -ldl


################################################################################
### DO NOT EDIT THE FOLLOWING LINES ############################################


# define list of objects
OBJSC=$(SOURCES:.cpp=.o)
OBJS=$(OBJSC:.cpp=.o)

# the target is obtained linking all .o files
all: $(SOURCES) $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(LDFLAGS) $(OBJS) $(LDLIBS) $(SLIBS) -o $(TARGET)

purge: clean
	rm -f $(TARGET)

clean:
	rm -f  $(OBJS)

################################################################################
################################################################################
