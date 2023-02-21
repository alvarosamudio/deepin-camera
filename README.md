# deepin-camera

Camera application for Linux Deepin, built with Qt5 and DTK5.

## Build

```sh
git clone https://github.com/alvarosamudio/deepin-camera.git
cd deepin-camera
./build.sh
```

The `build.sh` script sets up the required environment (custom g++-12, DTK5 headers/libs, C++ headers) and runs qmake + make.

## Run

```sh
./deepin-camera
```

Dependencies: `qtbase5-dev`, `qt5-qmake`, `qtmultimedia5-dev`, DTK5 runtime libraries.

## Mockup

![mockup 1](doc/mockup/deepincamera1.png)
![mockup 2](doc/mockup/deepincamera2.png)
