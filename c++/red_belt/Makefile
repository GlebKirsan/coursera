CXXFLAGS =	-O2 -g -Wall -fmessage-length=0

OBJS =		red_belt.o

LIBS =

TARGET =	red_belt.exe

$(TARGET):	$(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS)

all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
