#!/bin/bash

# get_robot_description.sh
# 支持的 chassis 类型
# "rabbit" 使用 four_swerve chassis
# "elephant" 使用 three_swerve chassis
# "rc_r2" 使用 four_omni chassis

ROBOT_TYPE=$1
ROBOT_NAMESPACE=$2
USE_SIM=$3
BASE_COLOR=$4
USE_RADAR=$5
IF_FOR_RADAR=$6

if [ -z "$ROBOT_TYPE" ] || [ -z "$ROBOT_NAMESPACE" ]; then
    echo "Usage: $0 <robot_type> <robot_namespace> [use_sim]"
    exit 1
fi

if [ "$ROBOT_TYPE" == "standard" ]; then
    xacro "$(rospack find rc_description)/urdf/standard/standard.urdf.xacro" robot_namespace:=${ROBOT_NAMESPACE} use_sim:=${USE_SIM} base_color:=${BASE_COLOR} load_radar:=${USE_RADAR} load_cube:=${IF_FOR_RADAR}
elif [ "$ROBOT_TYPE" == "rabbit" ]; then
    xacro "$(rospack find rc_description)/urdf/rabbit/rabbit.urdf.xacro" robot_namespace:=${ROBOT_NAMESPACE} use_sim:=${USE_SIM} base_color:=${BASE_COLOR} load_radar:=${USE_RADAR} load_cube:=${IF_FOR_RADAR}
elif [ "$ROBOT_TYPE" == "engineer" ]; then
    xacro "$(rospack find rc_description)/urdf/engineer/engineer.urdf.xacro" robot_namespace:=${ROBOT_NAMESPACE} use_sim:=${USE_SIM} base_color:=${BASE_COLOR} load_radar:=${USE_RADAR} load_cube:=${IF_FOR_RADAR}
elif [ "$ROBOT_TYPE" == "elephant" ]; then
    xacro "$(rospack find rc_description)/urdf/elephant/elephant.urdf.xacro" robot_namespace:=${ROBOT_NAMESPACE} use_sim:=${USE_SIM} base_color:=${BASE_COLOR} load_radar:=${USE_RADAR} load_cube:=${IF_FOR_RADAR}
elif [ "$ROBOT_TYPE" == "rc_r2" ]; then
    xacro "$(rospack find rc_description)/urdf/rc_r2/chassis.urdf.xacro" robot_namespace:=${ROBOT_NAMESPACE} use_sim:=${USE_SIM} base_color:=${BASE_COLOR} load_radar:=${USE_RADAR} load_cube:=${IF_FOR_RADAR}
else
    echo "Unknown robot type: $ROBOT_TYPE"
    exit 1
fi