# Makefile for Bombersauro

# *****************************************************
# Variables to control Makefile operation

CXX = g++
#flags linux:
#CXXFLAGS = -Wall -std=c++11 -lSDL2 -lGL -lGLU -lGLEW -I .
#flags windows:
CXXFLAGS = -lmingw32 -lSDL2main -lSDL2 -lopengl32 -lglu32 -lglew32 -Wall -std=c++11 -I .


# *****************************************************
# Targets needed to bring the executable up to date

main: main.o glsupport.o Pos.o Player.o MapTile.o Map.o GlCamera.o SdlWindow.o \
		MainWindow.o DiamSqNoise.o  GlObject.o CubeShader.o EventAggregator.o
	$(CXX) -o Bombersauro main.o glsupport.o Pos.o Player.o MapTile.o \
		Map.o GlCamera.o SdlWindow.o MainWindow.o DiamSqNoise.o  GlObject.o \
		CubeShader.o EventAggregator.o $(CXXFLAGS)

main.o: main.cpp Map.h Noises/DiamSqNoise.h SdlClasses/CustomWindows/MainWindow.h \
		Events/EventAggregator.h Events/CustomEvents/Tick.h
	$(CXX) $(CXXFLAGS) -c main.cpp

glsupport.o: glsupport.cpp glsupport.h
	$(CXX) $(CXXFLAGS) -c glsupport.cpp

Pos.o: Pos.cpp Pos.h
	$(CXX) $(CXXFLAGS) -c Pos.cpp

Player.o: Player.cpp Player.h glsupport.h Map.h GlCamera.h Events/EventAggregator.h \
			Events/CustomEvents/Tick.h Geometry/GlObject.h
	$(CXX) $(CXXFLAGS) -c Player.cpp

MapTile.o: MapTile.cpp MapTile.h
	$(CXX) $(CXXFLAGS) -c MapTile.cpp

Map.o: Map.cpp Map.h Pos.h MapTile.h
	$(CXX) $(CXXFLAGS) -c Map.cpp

GlCamera.o: GlCamera.cpp GlCamera.h glsupport.h Events/EventAggregator.h \
			Events/CustomEvents/WindowResized.h
	$(CXX) $(CXXFLAGS) -c GlCamera.cpp

SdlWindow.o: SdlClasses/SdlWindow.cpp SdlClasses/SdlWindow.h \
	Events/CustomEvents/WindowResized.h glsupport.h
	$(CXX) $(CXXFLAGS) -c SdlClasses/SdlWindow.cpp

MainWindow.o: SdlClasses/CustomWindows/MainWindow.cpp SdlClasses/CustomWindows/MainWindow.h \
		SdlClasses/SdlWindow.h Map.h Events/EventAggregator.h Events/CustomEvents/EventCode.h \
		GlCamera.h Geometry/GlObject.h Player.h
	$(CXX) $(CXXFLAGS) -c SdlClasses/CustomWindows/MainWindow.cpp

DiamSqNoise.o: Noises/DiamSqNoise.cpp Noises/DiamSqNoise.h Noises/Noise.h Map.h
	$(CXX) $(CXXFLAGS) -c Noises/DiamSqNoise.cpp

GlObject.o: Geometry/GlObject.cpp Geometry/GlObject.h glsupport.h Geometry/cube/CubeShader.h
	$(CXX) $(CXXFLAGS) -c Geometry/GlObject.cpp

CubeShader.o: Geometry/cube/CubeShader.cpp Geometry/cube/CubeShader.h glsupport.h
	$(CXX) $(CXXFLAGS) -c Geometry/cube/CubeShader.cpp

#UiObject.o: Events/UiObject.cpp Events/UiObject.h SdlClasses/SdlTexture.h
#	$(CXX) $(CXXFLAGS) -c Events/UiObject.cpp

EventAggregator.o: Events/EventAggregator.cpp Events/EventAggregator.h Events/Event.h
	$(CXX) $(CXXFLAGS) -c Events/EventAggregator.cpp

clean:
		rm *.o *.h.gch
#main.o glsupport.o Pos.o Player.o MapTile.o Map.o GlCamera.o SdlWindow.o \
#MainWindow.o DiamSqNoise.o  GlObject.o CubeShader.o EventAggregator.o
 
uninstall:
		rm Bombersauro

# *****************************************************
# antigo Make

#Bombersauro:
#	g++ -o Bombersauro \
#	main.cpp Pos.cpp MapTile.cpp glsupport.cpp GlCamera.cpp Scene.h Map.cpp Player.cpp \
#	Geometry/GlObject.cpp Geometry/cube/CubeShader.cpp \
#	Noises/Noise.h Noises/DiamSqNoise.cpp \
#	SdlClasses/SdlWindow.cpp SdlClasses/CustomWindows/MainWindow.cpp \
#	Events/EventAggregator.cpp Events/Event.h Events/CustomEvents/EventCode.h \
#	Events/CustomEvents/WindowResized.h Events/CustomEvents/Tick.h \
#	-lmingw32 -lSDL2main -lSDL2 -lopengl32 -lglu32 -lglew32 -Wall -std=c++11 -I ../Bombersauro
#	-std=c++11 -lSDL2 -lGL -lGLU -lGLEW -o Bombersauro