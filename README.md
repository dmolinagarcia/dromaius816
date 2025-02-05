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

## Diario
### 04 / 02 / 2025

>> He conseguido montar un básico con reloj, reset y cpu. Tambíen he clonado la CPU6502 a una CPU65816. 
Entender el reloj, oscilador. Step clock y step cycle
Siguientes pasos son. Como pintar toda la información posible. Veo la CPU pero... quiero ver circuito? Es posible?
Al menos, pintar reset, clock, por ahora.
Crear un chip nuevo. HELIUM. FPGA. De momento... quiero que tenga como input el master clock y output un PHI partido por n (4? 16?)
Simular chips 74xx
Además, quiero entender la CPU.
También debo enganchar una memoria. Al minimal65816.
Conseguir ejecutar algo básico (teniendo en cuenta que sigo siendo un 6502 a pesar del nombre)
NOP test
LDAs, cosas muy basicas vaya
Tras esto, destripar la cpu65816 y comenzar a añadir cosas de poco a poco.


