CXX = g++
# C++ 컴파일러를 정의하며, GNU g++ 사용

WX_CXXFLAGS := $(shell wx-config --cxxflags)
# wxWidgets 라이브러리 컴파일 플래그 저장, 'wx-config --cxxflags' 명령을 사용
WX_LIBS     := $(shell wx-config --libs)
# wxWidgets 라이브러리 저장, 'wx-config --libs' 명령을 사용

PNG_CFLAGS := $(shell pkg-config --cflags libpng)
# libpng 라이브러리 컴파일 플래그 저장, 'pkg-config --cflags libpng' 명령을 사용
PNG_LIBS   := $(shell pkg-config --libs   libpng)
# libpng 라이브러리 저장, 'pkg-config --libs   libpng' 명령을 사용

CXXFLAGS := -std=c++17 -g -Wall $(WX_CXXFLAGS) $(PNG_CFLAGS)
# compile시 사용할 flag전체 저장
# -std=c++17: C++17 표준을 사용
# -g: 디버깅 정보표시
# -Wall: 모든 경고를 활성화
LDFLAGS  := $(WX_LIBS) $(PNG_LIBS) -pthread
# link 단계에서 사용할 flag저장
# $(WX_LIBS): wxWidgets library 연결 flag, 위에서 정의함
# $(PNG_LIBS): libpng library 연결 flag, 위에서 정의함
# -pthread: multi thread 사용을 위한 pthread library


TARGET = brick-breaker
# 실행파일명 정의
SRCS   = imageutil.cpp ingameobj/ImageRenderable.cpp ingameobj/R2Point.cpp ingameobj/Ball.cpp ingameobj/Block.cpp ingameobj/ArrowHelper.cpp Game.cpp MyFrame.cpp MyApp.cpp main.cpp
# compile에 포함될 source file 정의
OBJS   = $(SRCS:.cpp=.o)
# src의 .cpp file들의 object code를 저장할 규칙 정의

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) -o $@ $^ $(LDFLAGS)
# $(TARGET)을 build하기 위해 모든 object file들 $(OBJS)를 link
# $(LDFLAGS) (위에서 선언함)을 포함하여 link 단계에서 사용할 명령인수로 사용

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@
# .cpp file들을 compile하여 object code .o file을 생성
# $(CXXFLAGS) (위에서 선언함)을 명령인수로 사용

clean:
	rm -f $(TARGET) $(OBJS)
# build과정에서 생성된 file들을 모두 제거
# $(TARGET) 최종 만들어진 실행 file
# $(OBJS) compile결과로 만들어진 object code file
