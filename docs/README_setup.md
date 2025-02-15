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
sudo apt intsall wine
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