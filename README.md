# dromaius816
Nova 64 emulator based on Dromaius

## Preparando la vbox

Clonamos la VM standar

sudo apt-get update
sudo apt install cmake
sudo apt install libx11-dev
sudo apt install libxrandr-dev
sudo apt install libxinerama-dev
sudo apt install libxcursor-dev
sudo apt install libxi-dev
sudo apt install libgl1-mesa-dev libglu1-mesa-dev freeglut3-dev
sudo apt install wayland-protocols libwayland-dev
sudo apt install pkg-config
sudo apt install libxkbcommon-dev


## Intentando compilar el original

cmake . -B _build -DCMAKE_BUILD_TYPE=Release
cmake --build _build 

## Funciona, voy creando dentro de dromaius816 el mio
Carga de librerias

cd dromaius816/libs
git submodule add https://github.com/ocornut/imgui.git
git submodule add https://github.com/glfw/glfw.git

cd imgui
