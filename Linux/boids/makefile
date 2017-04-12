LIBS=-ldl -lglfw -lGL
INC=-Ilibraries/GLM -Ilibraries/GLAD_V4.5/include -Ilibraries/GLFW_x32/inlcude

Boids:
	g++ libraries/GLAD_V4.5/src/glad.c Header.h ../Boid.h ShaderBuilder.h Tools.h ShaderBuilder.cpp Utilities.cpp Tools.cpp Boid.cpp Main.cpp -fopenmp -o  Boids $(LIBS) $(INC)

clean:
	rm Boids
	g++ libraries/GLAD_V4.5/src/glad.c Header.h ../Boid.h ShaderBuilder.h Tools.h ShaderBuilder.cpp Utilities.cpp Tools.cpp Boid.cpp Main.cpp -fopenmp -o  Boids $(LIBS) $(INC)

run:
	g++ libraries/GLAD_V4.5/src/glad.c Header.h ../Boid.h ShaderBuilder.h Tools.h ShaderBuilder.cpp Utilities.cpp Tools.cpp Boid.cpp Main.cpp -fopenmp -o  Boids $(LIBS) $(INC)
	./Boids
