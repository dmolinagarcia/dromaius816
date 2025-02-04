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

-- Pasamos al branch Docking
cd imgui
git fetch
cd ../..
git add libs/imgui
git commit -m "Actualizado submódulo imgui para apuntar a la rama docking"

git config -f .gitmodules submodule.libs/imgui.branch docking
git add .gitmodules
git commit -m "Configurado imgui para seguir la rama docking por defecto"

-- pasamos a los mismos commits que dromiaius
cd ~/work/dromaius816
cd libs/glfw
git fetch 
git checkout 2bac7ee8da526257d808bd026b027246c98e4f2f 
cd ../imgui
git fetch
git checkout 64519c6875fde64796aa1d098c02e1b346ac99fe
cd ../../
git add libs/glfw libs/imgui
git commit -m "Actualizados submódulos glfw e imgui a commits específicos"
git push

