#--------------------------
# Macro definitions
	CXXFLAGS = -O1 -g -Wall -std=c++11
	OBJ = graph_server.o ./library/mongoose.o ./graph/graph.o ./library/virtual_memory.o
	TARGET = cs426_graph_server
#--------------------------
# Rules
all:$(TARGET)
$(TARGET):$(OBJ)
	$(CXX) -o $@ $(OBJ)
clean:
	rm -f $(OBJ) $(TARGET)
#---------------------------
#Dependencies
graph_server.o: graph_server.cpp ./library/mongoose.h ./graph/graph.h ./library/virtual_memory.hpp
./library/mongoose.o: ./library/mongoose.c ./library/mongoose.h
./graph/graph.o: ./graph/graph.cpp ./graph/graph.h
./library/virtual_memory.o: ./library/virtual_memory.cpp ./library/virtual_memory.hpp
