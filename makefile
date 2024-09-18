
# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -std=c++17 -fPIC
QTFLAGS = $(shell pkg-config --cflags --libs Qt5Widgets)
# Source files
SRC = main.cpp gamehandler.cpp customwidget.cpp Board.cpp Player.cpp Game.cpp Square.cpp \
      GoSquare.cpp Jail.cpp FreeParking.cpp GoToJail.cpp Tax.cpp Street.cpp Property.cpp \
      UtilityProperty.cpp ChanceSquare.cpp CommunityChestSquare.cpp \
      CommunityChestCard.cpp Card.cpp Dice.cpp
# Object files
OBJ = $(SRC:.cpp=.o)
# Executable name
TARGET = monopoly_app
# Qt MOC (Meta-Object Compiler) for handling signals and slots
MOC = moc_customwidget.cpp
MOC_SOURCES = customwidget.h
MOC_OUTPUT = moc_customwidget.o
# Default target
all: $(TARGET)
# Linking the final executable
$(TARGET): $(OBJ) $(MOC_OUTPUT)
	$(CXX) -o $@ $(OBJ) $(MOC_OUTPUT) $(QTFLAGS)
# Compile source files into object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(QTFLAGS) -c $< -o $@
# Run moc on customwidget.h
$(MOC): $(MOC_SOURCES)
	moc $< -o $@
# Compile moc output
$(MOC_OUTPUT): $(MOC)
	$(CXX) $(CXXFLAGS) $(QTFLAGS) -c $< -o $@ 
# Clean up the compiled files
clean:
	rm -f $(OBJ) $(MOC_OUTPUT) $(TARGET) $(MOC)
# Phony target to prevent conflicts
.PHONY: clean