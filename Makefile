CXX = g++
CXXFLAGS = -std=c++11 -Wall
SDL_FLAGS = `sdl2-config --cflags --libs`
TARGET = game_of_life
SOURCES = main.cpp life.cpp

$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SOURCES) $(SDL_FLAGS)

clean:
	rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: clean run


