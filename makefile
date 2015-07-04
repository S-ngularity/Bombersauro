# Makefile for Bombersauro

# *****************************************************
# Variables to control Makefile operation

CXX = g++
#flags Linux:
#CXXFLAGS = -Wall -std=c++11 -lSDL2 -lGL -lGLU -lGLEW -MMD -I .
#flags Windows:
CXXFLAGS = -lmingw32 -lSDL2main -lSDL2 -lopengl32 -lglu32 -lglew32 -Wall -std=c++11 -MMD -I .

SRC =	main.cpp Bomba.cpp Player.cpp Game.cpp \
		Map/Map.cpp Map/Noises/DiamSqNoise.cpp \
		SdlClasses/SdlWindow.cpp SdlClasses/CustomWindows/MainWindow.cpp \
		Events/EventAggregator.cpp \
		GlClasses/GlObject.cpp GlClasses/glsupport.cpp GlClasses/GlCamera.cpp\
		Geometry/NormalShader.cpp Geometry/objloader.cpp
		
OUT = Bombersauro

# *****************************************************
# Targets needed to bring the executable up to date

all: ${SRC:.cpp=.o}
	${CXX} ${SRC:.cpp=.o} ${CXXFLAGS} -o ${OUT}

clean:
		rm ${SRC:.cpp=.o} ${SRC:.cpp=.d} ${SRC:.cpp=.h.gch} -f
 
uninstall:
		rm Bombersauro -f

-include ${SRC:.cpp=.d}
