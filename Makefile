#Output
TARGET=bin/Physics.exe
#TARGET=bin/Physics.out

#Compiler
CC=g++

#Opciones de compilacion
CFLAGS= -Wall -std=c++20

#Rutas de inclusion de encabezados
INCLUDES= -Iinclude/ -Isrc/imgui

#Archivos fuente del proyecto
SRC= main.cpp glad.cpp Engine/System.cpp Render/Shader.cpp Render/Texture.cpp Engine/Scene.cpp Engine/Gameobject.cpp
SRC += Engine/Components/Transform.cpp Engine/Components/Camera.cpp
SRC += imgui/imgui.cpp imgui/imgui_demo.cpp imgui/imgui_impl_glfw.cpp imgui/imgui_draw.cpp imgui/imgui_impl_opengl3.cpp imgui/imgui_tables.cpp imgui/imgui_widgets.cpp

#Archivos de librerias

BLIBS= -Llibs -lreactphysics3d -lglfw3dll -lopengl32 -lgdi32
#BLIBS= -Llibs -lreactphysics3d -lglfw

#Nombre de rutas
OBJ_DIR= obj

#Objetos generados
OBJ = $(addprefix obj/, $(SRC:.cpp=.o))

#Regla para compilar los archivos fuente
$(OBJ_DIR)/%.o: src/%.cpp | $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

#Regla para compilar el ejecutable final
$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $@ $(BLIBS)

clean:
	rm -rf $(OBJ_DIR) $(TARGET)