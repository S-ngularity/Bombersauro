Bombersauro:
	g++ -o Bombersauro \
	main.cpp Pos.cpp MapTile.cpp glsupport.cpp GlCamera.cpp Scene.h Map.cpp \
	Geometry/GlObject.cpp Geometry/cube/CubeShader.cpp \
	Noises/Noise.h Noises/DiamSqNoise.cpp \
	SdlClasses/SdlWindow.cpp SdlClasses/CustomWindows/MainWindow.cpp \
	Events/EventAggregator.cpp Events/Event.h Events/CustomEvents/EventCodes.h Events/CustomEvents/EventWindowResized.h \
	-lmingw32 -lSDL2main -lSDL2 -lopengl32 -lglu32 -lglew32 -Wall -std=c++11 -I ../Bombersauro
#	-std=c++11 -lSDL2 -lGL -lGLU -lGLEW -o Bombersauro