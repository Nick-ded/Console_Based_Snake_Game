CXX      = g++
CXXFLAGS = -std=c++17 -O2 -Wall -Wno-unused-variable
TARGET   = snake_ultra.exe
SRCDIR   = src
SRCS     = $(SRCDIR)/main.cpp     \
           $(SRCDIR)/game.cpp     \
           $(SRCDIR)/console.cpp  \
           $(SRCDIR)/sound.cpp    \
           $(SRCDIR)/particles.cpp\
           $(SRCDIR)/maze.cpp     \
           $(SRCDIR)/leaderboard.cpp \
           $(SRCDIR)/ui.cpp

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS)
	@echo Build complete: $(TARGET)

clean:
	del /f $(TARGET) scores.dat

run: $(TARGET)
	./$(TARGET)
