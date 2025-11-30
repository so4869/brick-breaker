CXX = g++

WX_CXXFLAGS := $(shell wx-config --cxxflags)
WX_LIBS     := $(shell wx-config --libs)

PNG_CFLAGS := $(shell pkg-config --cflags libpng)
PNG_LIBS   := $(shell pkg-config --libs   libpng)

CXXFLAGS := -std=c++17 -g -Wall $(WX_CXXFLAGS) $(PNG_CFLAGS)
LDFLAGS  := $(WX_LIBS) $(PNG_LIBS) -pthread

TARGET = sg-prj-aie2051
SRCS   = imageutil.cpp ingameobj/ImageRenderable.cpp ingameobj/R2Point.cpp ingameobj/Ball.cpp ingameobj/ArrowHelper.cpp Game.cpp MyFrame.cpp MyApp.cpp main.cpp
OBJS   = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJS)
