# ATOS 
<img align="left" width="100" height="100" src="./doc/ATOS_icon.svg">

ATOS, the communication hub for all test objects, controls and monitors iso22133-compliant vehicles and equipment. To build ATOS follow the guide below.

<br />
<br />


# Table of contents
- [ATOS](#atos)
- [Table of contents](#table-of-contents)
- [ Building ATOS with colcon](#-building-atos-with-colcon)
  - [ Dependencies \& external libraries](#-dependencies--external-libraries)
    - [ Installing OpenSimulationInterface v3.4.0](#-installing-opensimulationinterface-v340)
    - [ Installing atos-interfaces](#-installing-atos-interfaces)
    - [ Installing esmini](#-installing-esmini)
  - [ Installing ROS2 and building for the first time with colcon](#-installing-ros2-and-building-for-the-first-time-with-colcon)
    - [ Ubuntu 20.04](#-ubuntu-2004)
- [ Optional builds \& installations](#-optional-builds--installations)
  - [ How to build with RelativeKinematics instead of ObjectControl](#-how-to-build-with-relativekinematics-instead-of-objectcontrol)
- [User manual](#user-manual)
  - [The test directory](#the-test-directory)
  - [Changing ROS parameters](#changing-ros-parameters)
  - [Using ATOS with a Graphical User Interface (GUI)](#using-atos-with-a-graphical-user-interface-gui)
    - [Simple Control](#simple-control)
    - [Foxglove Studio](#foxglove-studio)
      - [Connect to ATOS](#connect-to-atos)
      - [Extensions](#extensions)
      - [Changing layout](#changing-layout)
      - [Panels in Foxglove Studio](#panels-in-foxglove-studio)

# <a name="ATOS"></a> Building ATOS with colcon
Below are the steps for building ATOS for the first time with colcon.

Prerequisites: C/C++ compiler, CMake (minimum version 3.10.2)

## <a name="dependencies"></a> Dependencies & external libraries
In order to build ATOS, dependencies and exernal libraries need to be installed. First install the necessary development packages:
```
sudo apt install libsystemd-dev libprotobuf-dev protobuf-compiler libeigen3-dev ros-foxy-paho-mqtt-c nlohmann-json3-dev npm nodejs libpcl-dev
```

Then, the following external libraries need to be installed:
- [OpenSimulationInterface v3.4.0](https://github.com/OpenSimulationInterface/open-simulation-interface)
- [atos-interfaces](https://github.com/RI-SE/atos-interfaces)
- [esmini](https://github.com/esmini/esmini)

### <a name="osi"></a> Installing OpenSimulationInterface v3.4.0
```
git clone https://github.com/OpenSimulationInterface/open-simulation-interface.git -b v3.4.0
cd open-simulation-interface
mkdir build && cd build
cmake .. && make
sudo make install
```
Make sure that the linker knows where OpenSimulationInterface is located:
```
sudo sh -c "echo '/usr/local/lib/osi3' > /etc/ld.so.conf.d/osi3.conf"
sudo ldconfig
```

### <a name="atos-interfaces"></a> Installing atos-interfaces
```
git submodule update --init
```

### <a name="esmini"></a> Installing esmini
Begin by installing esmini dependencies listed under section 2.3 on the page https://esmini.github.io/
```
git clone https://github.com/esmini/esmini
cd esmini
mkdir build && cd build
cmake .. && make
sudo make install
cp ../bin/libesminiLib.so /usr/local/lib
cp ../bin/libesminiRMLib.so /usr/local/lib
sudo mkdir -p /usr/local/include/esmini/
cp ../EnvironmentSimulator/Libraries/esminiLib/esminiLib.hpp /usr/local/include/esmini/
cp ../EnvironmentSimulator/Libraries/esminiRMLib/esminiRMLib.hpp /usr/local/include/esmini/
sudo ldconfig
```


## <a name="ros2"></a> Installing ROS2 and building for the first time with colcon
### <a name="ubuntu-20.04"></a> Ubuntu 20.04
clone ATOS in your git folder, and make sure that all submodules are present and up to date:
```
git clone https://github.com/RI-SE/ATOS.git
cd ATOS
git submodule update --init --recursive
```

Download prerequisites:
```
sudo apt update && sudo apt install curl gnupg2 lsb-release
```
Authorize the ros2 gpg key with apt:
```sudo apt update && sudo apt install curl gnupg2 lsb-release
sudo curl -sSL https://raw.githubusercontent.com/ros/rosdistro/master/ros.key  -o /usr/share/keyrings/ros-archive-keyring.gpg
```
Add the ROS2 repo to sources list:
```
echo "deb [arch=$(dpkg --print-architecture) signed-by=/usr/share/keyrings/ros-archive-keyring.gpg] http://packages.ros.org/ros2/ubuntu $(source /etc/os-release && echo $UBUNTU_CODENAME) main" | sudo tee /etc/apt/sources.list.d/ros2.list > /dev/null
```
Install ros foxy for desktop and colcon
```
sudo apt update
sudo apt install ros-foxy-desktop python3-colcon-common-extensions ros-foxy-nav-msgs ros-foxy-geographic-msgs ros-foxy-foxglove-msgs ros-foxy-sensor-msgs  ros-foxy-rosbridge-suite ros-foxy-pcl-conversions
```

source the setup script:
```
source /opt/ros/foxy/setup.bash
```
Add the above line to ~/.bashrc or similar startup script to automate this process.

Create a workspace:
```
mkdir -p ~/atos_ws/src
```

Create symlinks to atos and atos_interfaces
```
ln -s path/to/ATOS ~/atos_ws/src/atos
ln -s path/to/ATOS/atos_interfaces ~/atos_ws/src/atos_interfaces
```

Change directory into the workspace and build
```
cd ~/atos_ws
colcon build
```

Source the project setup file:
```
source ~/atos_ws/install/setup.bash
```
Also add the above line to ~/.bashrc or similar.

Launch ATOS
```
ros2 launch atos launch_basic.py
```

# <a name="optional-builds--installations"></a> Optional builds & installations
ATOS can be installed in alternative ways, and built with support for various optional modules, described here.

## <a name="relativekinematics"></a> How to build with RelativeKinematics instead of ObjectControl

The server will build the ObjectControl with AbsolutKinematics by default. It's possible to build with RelativeKinematics support by rebuilding with the argument -DWITH_RELATIVE_KINEMATICS=ON, see following command
```sh
colcon build --cmake-args -DWITH_RELATIVE_KINEMATICS=ON
```
To include ObjectControl in the build again run the same command with OFF, as follows
```sh
colcon build --cmake-args -DWITH_RELATIVE_KINEMATICS=OFF
```

# User manual

## The test directory
After starting ATOS the first time, you will find the test directory in `~/.astazero/ATOS`. This directory will contain all test settings and will contain the following directories:
- **Catalogs**
  - Explanation here: 
  - Supported file types:
- **certs**
  - Explanation here: 
  - Supported file types:
- **conf**
  - Explanation here: 
  - Supported file types:
- **geofence**
  - Explanation here: 
  - Supported file types:
- **journal**
  - Explanation here: 
  - Supported file types:
- **objects**
  - Explanation here: Directory containing all objects that should be used in a test.
  - Supported file types: `.opro`
- **odr**
  - Explanation here: Directory containing OpenDRIVE-files.
  - Supported file types: `.xodr`
- **osc**
  - Explanation here: Directory containing the OpenSCENARIO-files.
  - Supported file types: `.xosc`
- **pointclouds**
  - Explanation here: Directory containing site scans as pointclouds.
  - Supported file types: `.pcd`
- **traj**
  - Explanation here: 
  - Supported file types:


## Changing ROS parameters
In the `conf`-directory, `params.yaml` is located and sets ROS-parameters for the ATOS modules, which are read when launching ATOS. The parameters are described in their respective module:
- [ATOSBase](./modules/ATOSBase/README.md)
- [EsminiAdapter](./modules/EsminiAdapter/README.md)
- [MQTTBridge](./modules/MQTTBridge/README.md)
- [ObjectControl](./modules/ObjectControl/README.md)
- [OSIAdapter](./modules/OSIAdapter/README.md)
- [PointcloudPublisher](./modules/PointcloudPublisher/README.md)
- [SystemControl](./modules/SystemControl/README.md)
- [TrajectoryletStreamer](./modules/TrajectoryletStreamer/README.md)


## Using ATOS with a Graphical User Interface (GUI)
### Simple Control
Simple Control is used to run ATOS without any further visualisation. In order to start Simple Control, start ATOS and then open your web browser and go to `ATOS_host_address:3000`, where `ATOS_host_address` is the address to the server that ATOS is hosted on. If you run ATOS on your own computer and want to use Simple Control on the same computer, you would go to `localhost:3000`.

When opening Simple Control there will be a set of buttons, [ObjectControl](./modules/ObjectControl/README.md) state, and connection status to ATOS. The buttons are:
- **Reset Test** - Reset the loaded test
- **Init** - Initialise a test, loads things such as the scenario and which objects to use.
- **Connect** - Connects to the objects that were initialised.
- **Arm** - Arms the objects, meaning that they are ready to start the test.
- **Disarm** - Disarms the objects, meaning that they are no longer ready to start the test.
- **Start** - Starts the test.
- **Abort** - Stops the test, for instance if something goes wrong or the test needs to be stopped for some reason.
- **All clear** - Clears the test.


### Foxglove Studio
If you want to run ATOS with more visualisation you can use [Foxglove Studio](https://github.com/foxglove/studio). To get started, visit [studio.foxglove.dev](https://studio.foxglove.dev).

#### Connect to ATOS
In order to connect to ATOS in Foxglove Studio, ATOS first needs to be running. Then, in Foxglove Studio, go to `Data source -> New connection -> Open connection -> Rosbridge` and enter `wss://ATOS_host_address:9090`.

#### Extensions
In order for everything to be displayed correctly, some extensions needs to be added. The extensions for ATOS are found [here](./plugins/foxglove/) with the file type `.foxe`. In Foxglove Studio, go to `Extensions` and then drag and drop the extensions.

#### Changing layout
The premade layouts for ATOS are located [here](./plugins/foxglove/). To add a layout in Foxglove Studio, go to `Layouts -> Import layout` and select one of the layouts for ATOS.

#### Panels in Foxglove Studio
The panels used in layouts for ATOS are Simple Control, Map, and 3D. To change settings for the panels, press `Settings` that are located at the top right of each panel.

Simple Control - This is a panel for Simple Control as described previously. It has the following settings:
- `ATOS hostname` - The host address of ATOS.
- `ATOS port` - The port to connect to.
- `Protocol` - Either `https` or `http`

Map - This is a panel that displays a map with objects and their paths. If you want a custom map, in settings change `Tile layer` to custom and enter the address to your tile server. In the settings you can also turn on or off which objects and paths that should be displayed, and also change their colours.

3D - This is a panel for 3D representation of a scenario. Objects, paths, and site scans can be visualised here. In the settings menu, the objects, paths, and site scans can be turned on or off. They can also change size, colour, and shape.
