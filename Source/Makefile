COMPILE_FLAGS=-m32

CRage: CConfigParser.o CControl.o CMaster.o CSocket.o Main.o StringUtils.o StdInc.o
	g++ $(COMPILE_FLAGS) -o ../Release/CRage CConfigParser.o CControl.o CMaster.o CSocket.o Main.o StringUtils.o StdInc.o

CConfigParser.o:
	g++ $(COMPILE_FLAGS) -c CConfigParser.cpp

CControl.o:
	g++ $(COMPILE_FLAGS) -c CControl.cpp

CMaster.o:
	g++ $(COMPILE_FLAGS) -c CMaster.cpp

CSocket.o:
	g++ $(COMPILE_FLAGS) -c CSocket.cpp

Main.o:
	g++ $(COMPILE_FLAGS) -c Main.cpp

StringUtils.o:
	g++ $(COMPILE_FLAGS) -c StringUtils.cpp

StdInc.o:
	g++ $(COMPILE_FLAGS) -c StdInc.cpp

clean:
	rm -rf *.o
