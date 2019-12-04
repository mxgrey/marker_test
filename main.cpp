/*
 * Copyright (C) 2019 Open Source Robotics Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
*/

#include <visualization_msgs/msg/marker_array.hpp>
#include <rclcpp/rclcpp.hpp>
#include <cmath>

double wrap_to_2pi(double theta)
{
  const double divisor = std::floor(theta/(2*M_PI));
  return theta - 2*M_PI*divisor;
}

class MarkerTest : public rclcpp::Node
{
public:

  using Marker = visualization_msgs::msg::Marker;
  using Markers = visualization_msgs::msg::MarkerArray;
  using Point = geometry_msgs::msg::Point;

  MarkerTest()
    : rclcpp::Node("marker_test")
  {
    _publisher = create_publisher<Markers>(
          "/markers", rclcpp::SystemDefaultsQoS());

    using namespace std::chrono_literals;
    _timer = create_wall_timer(50ms, [&](){ make_markers(); });
  }

private:

  void make_markers()
  {
    Markers msg;
    msg.markers.push_back(make_circle());
    msg.markers.push_back(make_spot());

    _publisher->publish(msg);
  }

  Marker make_circle()
  {
    const auto now = get_clock()->now();

    Marker circle;

    circle.header.frame_id = "/map";
    circle.header.stamp = get_clock()->now();

    circle.id = 0;
    circle.ns = "marker_test";
    circle.type = Marker::LINE_STRIP;
    circle.action = Marker::ADD;

    circle.pose.position.x = 0.0;
    circle.pose.position.y = 0.0;
    circle.pose.position.z = 0.0;

    circle.pose.orientation.w = 1.0;
    circle.pose.orientation.x = 0.0;
    circle.pose.orientation.y = 0.0;
    circle.pose.orientation.z = 0.0;

    const double resolution = 5.0*M_PI/180.0;
    const double initial_theta = wrap_to_2pi(2.0*M_PI*now.seconds()/period);
    for (double theta = initial_theta; theta <= 2*M_PI; theta += resolution)
    {
      Point p;
      p.x = radius*std::cos(theta);
      p.y = radius*std::sin(theta);
      p.z = 0.0;
      circle.points.push_back(p);
    }
    Point p;
    p.x = radius;
    p.y = 0.0;
    p.z = 0.0;
    circle.points.push_back(p);

    const double thick = 0.1;
    circle.scale.x = thick;
    circle.scale.y = thick;
    circle.scale.z = thick;

    circle.color.r = 0.0;
    circle.color.g = 1.0;
    circle.color.b = 0.0;
    circle.color.a = 0.5;

    circle.lifetime.sec = 3.0;

    return circle;
  }

  Marker make_spot()
  {
    const auto now = get_clock()->now();

    Marker spot;

    spot.header.frame_id = "/map";
    spot.header.stamp = now;

    spot.id = 1;
    spot.ns = "marker_test";
    spot.type = Marker::CYLINDER;
    spot.action = Marker::ADD;

    const double height = 1.0;

    const double theta = wrap_to_2pi(2*M_PI*now.seconds()/period);
    spot.pose.position.x = radius*std::cos(theta);
    spot.pose.position.y = radius*std::sin(theta);
    spot.pose.position.z = height/2.0;

    spot.pose.orientation.w = 1.0;
    spot.pose.orientation.x = 0.0;
    spot.pose.orientation.y = 0.0;
    spot.pose.orientation.z = 0.0;

    spot.scale.x = 0.5;
    spot.scale.y = 0.5;
    spot.scale.z = height;

    spot.color.r = 1.0;
    spot.color.g = 1.0;
    spot.color.b = 0.0;
    spot.color.a = 1.0;

    spot.lifetime.sec = 3.0;

    return spot;
  }

  rclcpp::TimerBase::SharedPtr _timer;
  rclcpp::Publisher<Markers>::SharedPtr _publisher;
  const double radius = 5.0;
  const double period = 50.0;

};

int main(int argc, char* argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<MarkerTest>());
  rclcpp::shutdown();
}
