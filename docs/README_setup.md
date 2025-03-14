## Getting ready the VM

We clone our standby devbox VM and then

```
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
sudo apt install mingw-w64
sudo apt install wine
sudo apt install default-jre
sudo apt install python3
```

## Trying the original Dromaius Project

```
cmake . -B _build -DCMAKE_BUILD_TYPE=Release
cmake --build _build 
```

## As we made it work, let's create our own project

```
mkdir dromaius816
cd dromaius816/libs
git submodule add https://github.com/ocornut/imgui.git
git submodule add https://github.com/glfw/glfw.git

# Jump to Docking brach for IMGUI
cd imgui
git fetch
cd ../..
git add libs/imgui
git commit -m "Updated IMGUI submodule to jump to Docking branch"

git config -f .gitmodules submodule.libs/imgui.branch docking
git add .gitmodules
git commit -m "IMGUI configured to follow Docking branch"

# Revert to same commits as the original project, due to some errores
cd ~/work/dromaius816
cd libs/glfw
git fetch 
git checkout 2bac7ee8da526257d808bd026b027246c98e4f2f 
cd ../imgui
git fetch
git checkout 64519c6875fde64796aa1d098c02e1b346ac99fe
cd ../../
git add libs/glfw libs/imgui
git commit -m "GFLW and IMGUI updated to specific commits"
git push
```

## Install an emulator

### emu816
get Andrew Jacobs emu from https://github.com/andrew-jacobs/emu816

unzip emu-master
cd emu816-master

The DEV65 assembler is included in the download :)

Assembly: 
java -cp ../Dev65.jar uk.co.demon.obelisk.w65xx.As65 simple.asm

Link:
java -cp ../Dev65.jar uk.co.demon.obelisk.w65xx.Lk65 -bss '$0000-$7FFF' -code '$F000-$FFFF' -s28 -output simple.s28 simple.obj

sudo ln -s /home/devbox/work/emu816/emu816-master/emu816 /usr/sbin/emu816


### bsnes-plus
https://github.com/devinacker/bsnes-plus

sudo apt install qtbase5-dev qtbase5-dev-tools libxv-dev libsdl1.2-dev libao-dev libopenal-dev g++ libdbus-1-dev
git clone https://github.com/devinacker/bsnes-plus.git
cd bsnes-plus/bsnes
make
sudo ln -s /home/devbox/work/bsnes-plus/bsnes/out/bsnes /usr/sbin/bsnes






