#CConfigParser.cpp  CControl.h   CRage        Definitions.h  StdInc.h
#CConfigParser.h    CMaster.cpp  CSocket.cpp  Main.cpp       StringUtils.cpp
#CControl.cpp       CMaster.h    CSocket.h    StdInc.cpp     StringUtils.h

CRage: CConfigParser.o CControl.o CMaster.o CSocket.o Main.o StringUtils.o StdInc.o
	g++ -o ../Release/CRage CConfigParser.o CControl.o CMaster.o CSocket.o Main.o StringUtils.o StdInc.o

CConfigParser.o:
	g++ -c CConfigParser.cpp

CControl.o:
	g++ -c CControl.cpp

CMaster.o:
	g++ -c CMaster.cpp

CSocket.o:
	g++ -c CSocket.cpp

Main.o:
	g++ -c Main.cpp

StringUtils.o:
	g++ -c StringUtils.cpp

StdInc.o:
	g++ -c StdInc.cpp

clean:
	rm -rf ../Release/*.o
