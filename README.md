# rc25_path

> Robocon 2025 — Path planning, Gazebo simulation & multi-robot coordination

[中文文档](./README_CN.md)

## Overview

`rc25_path` is a robot path planning and simulation system for the Robocon 2025 season. Built on **ROS Noetic** + **Gazebo**, it covers multi-robot control, state estimation, trajectory planning, and Move Base navigation. The **Nodelet** architecture minimizes inter-process communication overhead, supporting single-remote or multi-remote control of multiple simulated robots.

## Package Structure

```
rc25_path/
└── rc_control/
    ├── rc_common/         # Common utilities & algorithms
    ├── rc_hw/             # Hardware interface (CAN / GPIO / IMU)
    ├── rc_gazebo/         # Gazebo simulation environment
    ├── air_joy_control/   # Joystick control (Nodelet)
    ├── move_control/      # Move Base chassis control
    ├── tf_broadcast/      # TF coordinate broadcasting
    └── odom_sensor/       # Odometry via position differentiation
```

## Modules

### rc_common — Algorithm Library

| Component | Description |
|-----------|-------------|
| **LQR** | Linear Quadratic Regulator |
| **Kalman Filter** | Standard Kalman filter |
| **Complementary Filter** | IMU attitude fusion |
| **One Euro Filter** | Low-latency adaptive filter |
| **S-curve Planner** | S-curve trajectory interpolation |
| **Eigen Types** | Linear algebra type wrappers |
| **Trajectory Generator** | Polynomial trajectory generation |
| **TF Broadcaster** | Real-time TF transform publishing |

### rc_hw — Hardware Interface

- SocketCAN-based CAN bus management
- Actuator control interfaces
- GPIO digital I/O management
- IMU sensor data acquisition & filtering

### rc_gazebo — Simulation Environment

- **Maps**: Competition field maps (basketball court + playing field)
- **Models**: Task model (SDF) + Robot URDF
- **Multi-robot**: Simultaneous spawn of multiple robots
- **Action Simulation**: Actuator hardware simulation plugins for Gazebo

### air_joy_control — Joystick Control

- **Single-remote mode**: Toggle between multiple robots via a switch
- **Multi-remote mode**: Dedicated remotes for individual robots
- **Dual coordinate frames**: World-frame velocity / robot-frame velocity

### move_control — Navigation

- Integration with `move_base` framework
- Dynamic loading of chassis controller plugins for multiple robots

### odom_sensor — Odometry

- Chassis velocity derived from position differentiation
- Outputs standard `nav_msgs/Odometry` messages
- Fills the gap where action controllers lack direct velocity output

## Nodelet Architecture

Nodelets within the same process pass data via shared pointers, eliminating ROS serialization/deserialization overhead between nodes. This significantly reduces CPU load and transport latency.

## Dependencies

- ROS Noetic
- Gazebo 11
- `rc25_description` (URDF model repo)
- `move_base` (navigation framework)

## Quick Start

```shell
# 1. Set robot type environment variables
export ROBOT_TYPE_1=rc_r2
export ROBOT_TYPE_2=rabbit
export ROBOT_TYPE_3=rc_r2
export ROBOT_TYPE_4=rabbit

# 2. Clone dependencies
cd <workspace>/src
git clone git@github.com:KetenBieber/rc25_description.git
git clone git@github.com:KetenBieber/rc25_path.git

# 3. Install dependencies & build
cd <workspace>
rosdepc install --from-paths . --ignore-src
catkin build
source devel/setup.bash

# 4. Launch simulation
mon launch rc_gazebo rc2025_gazebo.launch
mon launch load_tf_tree.launch
mon launch load_basketball_field_map.launch
```

## Related Repos

- [rc_ecat_controls](https://github.com/KetenBieber/rc_ecat_controls) — Physical robot EtherCAT controllers
- [gdut_rc_embedded_fw](https://github.com/KetenBieber/gdut_rc_embedded_fw) — Embedded firmware (formerly General_Framework)
- [easy_ecatslave_hw](https://github.com/KetenBieber/easy_ecatslave_hw) — EtherCAT slave hardware
