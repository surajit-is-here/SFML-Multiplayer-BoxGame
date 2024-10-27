SRCS = $(wildcard *.cpp)
OBJS = $(patsubst %.cpp, %.o, $(SRCS))
CXX = g++
COMPILER-FLAGS = -c
LINKER-FLAGS = -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
TARGET = game


default: $(TARGET)

# Display rules before compilation
%.o: %.cpp %.hpp
	$(RULES_MESSAGE)
	$(CXX) $(COMPILER-FLAGS) $< -o $@

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET) $(LINKER-FLAGS)

clean:
	rm -f $(OBJS) $(TARGET) *~
