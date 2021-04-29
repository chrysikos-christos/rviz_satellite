/* Copyright 2014 Gareth Cross

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License. */

#pragma once

#include <memory>
#include <unordered_map>
#include <map>
#include <utility>

#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/nav_sat_fix.hpp>
#include <geographic_msgs/msg/geo_point.hpp>

#include "rviz_common/properties/float_property.hpp"
#include "rviz_common/properties/int_property.hpp"
#include "rviz_common/properties/string_property.hpp"
#include <rviz_common/ros_topic_display.hpp>

#include "tile.hpp"
#include "tile_object.hpp"
#include "tile_client.hpp"

namespace rviz_satellite
{

/**
 * @brief Display a satellite map on the XY plane.
 */
class AerialMapDisplay : public rviz_common::RosTopicDisplay<sensor_msgs::msg::NavSatFix>
{
  Q_OBJECT
public:
  AerialMapDisplay();
  ~AerialMapDisplay() override;

  void onInitialize() override;
  void reset() override;
  void update(float, float) override;

protected Q_SLOTS:
  void updateAlpha();
  void updateDrawUnder();
  void updateTileUrl();
  void updateZoom();
  void updateBlocks();

protected:
  void onEnable() override;
  void onDisable() override;

  void processMessage(const sensor_msgs::msg::NavSatFix::ConstSharedPtr msg) override;

  bool validateMessage(const sensor_msgs::msg::NavSatFix::ConstSharedPtr msg);

  void buildObjects(TileCoordinate center_tile, double size, int zoom);
  
  void resetMap();

  TileCoordinate centerTile() const;

  rviz_common::properties::StringProperty* tile_url_property_ = nullptr;
  rviz_common::properties::IntProperty* zoom_property_ = nullptr;
  rviz_common::properties::IntProperty* blocks_property_ = nullptr;
  rviz_common::properties::FloatProperty* alpha_property_ = nullptr;
  rviz_common::properties::Property* draw_under_property_ = nullptr;
  
  TileClient tile_client_;

  std::unordered_map<TileId, std::future<QImage>> pending_tiles_;
  std::map<TileId, TileObject> tiles_;

  sensor_msgs::msg::NavSatFix::ConstSharedPtr last_fix_;

  static char const MAP_FRAME[];
};

}  // namespace rviz_satellite
