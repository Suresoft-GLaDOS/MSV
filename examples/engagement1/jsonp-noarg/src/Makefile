CXX := msv-profilecc

prog: reader.cpp
	$(CXX) -c reader.cpp -I rapidjson/include/rapidjson/ $(CFLAGS) -o reader.o
	$(CXX) -o prog $(CFLAGS) reader.o
