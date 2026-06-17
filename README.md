# rc25_path

> Robocon 2025 赛季 — 路径规划、Gazebo 仿真与多机器人协同控制

## 项目简介

`rc25_path` 是面向 Robocon 2025 赛季的机器人路径规划与仿真系统。项目基于 **ROS Noetic** + **Gazebo** 搭建，涵盖多机器人控制、状态估计、轨迹规划、Move Base 导航等模块。采用 **Nodelet** 架构减少进程间通信开销，支持单遥控/多遥控控制多台仿真机器人。

## 包结构

```
rc25_path/
└── rc_control/
    ├── rc_common/         # 通用工具库
    ├── rc_hw/             # 硬件接口（CAN / GPIO / IMU）
    ├── rc_gazebo/         # Gazebo 仿真环境
    ├── air_joy_control/   # 遥控器控制（Nodelet）
    ├── move_control/      # Move Base 底盘控制
    ├── tf_broadcast/      # TF 坐标广播
    └── odom_sensor/       # 里程计传感器（位置微分）
```

## 模块详解

### rc_common — 通用算法库

| 组件 | 说明 |
|------|------|
| **LQR** | 线性二次型调节器 |
| **Kalman Filter** | 标准卡尔曼滤波器 |
| **Complementary Filter** | IMU 互补滤波（姿态融合） |
| **One Euro Filter** | 低延迟自适应滤波 |
| **S-curve Planner** | S曲线轨迹插值 |
| **Eigen Types** | 矩阵运算类型封装 |
| **Trajectory Generator** | 多项式轨迹生成 |
| **TF Broadcaster** | 实时 TF 变换发布 |

### rc_hw — 硬件接口层

- CAN 总线通信管理（SocketCAN）
- 执行器控制接口
- GPIO 数字 IO 管理
- IMU 传感器数据采集与滤波

### rc_gazebo — 仿真环境

- **地图**: 篮球场地图 + 比赛场地地图
- **模型**: 任务模型 (SDF) + Robot URDF
- **多车加载**: 支持同时 spawn 多台机器人
- **Action 仿真**: 硬件执行器 Gazebo 仿真插件

### air_joy_control — 遥控控制

- **单遥控模式**: 通过拨杆切换控制多台车中的任意一台
- **多遥控模式**: 多台遥控器分别控制多台车
- **双坐标系支持**: 世界坐标系速度 / 机器人坐标系速度

### move_control — 导航接入

- 接入 `move_base` 框架
- 多台机器人底盘 controller plugin 动态加载

### odom_sensor — 里程计

- 通过位置微分获取底盘速度
- 输出标准 `nav_msgs/Odometry` 类型
- 弥补 Action 控制器无法直接提供速度数据的空缺

## Nodelet 架构

采用 `nodelet` 结构编程，同一进程内的多个 Nodelet 之间通过共享指针直接传递数据，避免了传统多进程 ROS 节点间的序列化/反序列化开销，显著降低 CPU 负载和传输延迟。

## 依赖

- ROS Noetic
- Gazebo 11
- `rc25_description`（URDF 模型仓库）
- `move_base`（导航框架）

## 快速开始

```shell
# 1. 设置机器人类型环境变量
export ROBOT_TYPE_1=rc_r2
export ROBOT_TYPE_2=rabbit
export ROBOT_TYPE_3=rc_r2
export ROBOT_TYPE_4=rabbit

# 2. 克隆依赖
cd <workspace>/src
git clone git@github.com:KetenBieber/rc25_description.git
git clone git@github.com:KetenBieber/rc25_path.git

# 3. 安装依赖 & 构建
cd <workspace>
rosdepc install --from-paths . --ignore-src
catkin build
source devel/setup.bash

# 4. 启动仿真
mon launch rc_gazebo rc2025_gazebo.launch
mon launch load_tf_tree.launch
mon launch load_basketball_field_map.launch
```

## 相关仓库

- [rc_ecat_controls](https://github.com/KetenBieber/rc_ecat_controls) — 实机 EtherCAT 控制器
- [gdut_rc_embedded_fw](https://github.com/KetenBieber/gdut_rc_embedded_fw) — 机器人嵌入式固件（原 General_Framework）
- [easy_ecatslave_hw](https://github.com/KetenBieber/easy_ecatslave_hw) — EtherCAT 从站硬件
