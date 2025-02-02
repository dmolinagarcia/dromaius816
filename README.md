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



## Intentando compilar el original

cmake . -B _build -DCMAKE_BUILD_TYPE=Release
cmake --build _build 

