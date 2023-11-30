CPP      = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -pedantic -g `sdl2-config --cflags`
LIBS     = `sdl2-config --libs`
SRC_DIR  = .
OBJ_DIR  = obj
BIN_DIR  = .
SOURCES  = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS  = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SOURCES))
TARGET   = ConwayGame

.PHONY: all clean

all: $(TARGET)

clean:
	rm -rf $(OBJ_DIR) $(TARGET)

$(TARGET): $(OBJECTS) | $(OBJ_DIR)
	$(CPP) $(OBJECTS) -o $(TARGET) $(LIBS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CPP) -c $< -o $@ $(CXXFLAGS)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)