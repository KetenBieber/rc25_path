# rc25 gazebo & rviz & ros_controller
本项目使用 catkin build 构建
这里是3个包，请先在你的工作空间目录下创建src目录
然后cd src/
git clone 本项目
然后使用 rosdepc install --from-paths . --ignore-src
来自动完成对本项目的依赖下载

# package introduction
rc_control 是基本的机器人组成文件
rc_controller 是控制器文件
rc_description 主要是存放了机器人的urdf文件

```shell
cd src/
git clone git@github.com:KetenBieber/rc25_description.git
cd ..
rosdepc install --from-paths . --ignore-src
catkin build
mon launch rc_gazebo rc2025_gazebo.launch
mon launch load_tf_tree.launch
mon launch load_basketball_field_map.launch

```