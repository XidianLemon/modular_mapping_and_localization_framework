/*
 * @Description: 实时显示，主要是点云
 * @Created Date: 2020-02-29 03:19:45
 * @Author: Ren Qian
 * -----
 * @Last Modified: 2021-11-26 17:38:00
 * @Modified By: Xiaotao Guo
 */

#ifndef LIDAR_LOCALIZATION_MAPPING_VIEWER_VIEWER_HPP_
#define LIDAR_LOCALIZATION_MAPPING_VIEWER_VIEWER_HPP_

#include <yaml-cpp/yaml.h>

#include <Eigen/Dense>
#include <string>

#include "lidar_localization/models/cloud_filter/voxel_filter.hpp"
#include "lidar_localization/sensor_data/cloud_data.hpp"
#include "lidar_localization/sensor_data/key_frame.hpp"
#include "lidar_localization/sensor_data/pose_data.hpp"

namespace lidar_localization {
class Viewer {
public:
    Viewer();

    bool UpdateWithOptimizedKeyFrames(std::deque<KeyFrame>& optimized_key_frames);
    bool UpdateWithNewKeyFrame(std::deque<KeyFrame>& new_key_frames, PoseData transformed_data, CloudData cloud_data);

    bool SaveMap();
    Eigen::Matrix4f& GetCurrentPose();
    CloudData::Cloud_Ptr& GetCurrentScan();
    bool GetLocalMap(CloudData::Cloud_Ptr& local_map_ptr);
    bool GetGlobalMap(CloudData::Cloud_Ptr& local_map_ptr);
    bool HasNewLocalMap();
    bool HasNewGlobalMap();

private:
    bool InitWithConfig();
    bool InitParam(const YAML::Node& config_node);
    bool InitDataPath(const YAML::Node& config_node);
    bool InitFilter(std::string filter_user,
                    std::shared_ptr<CloudFilterInterface>& filter_ptr,
                    const YAML::Node& config_node);

    bool OptimizeKeyFrames();
    bool JointGlobalMap(CloudData::Cloud_Ptr& global_map_ptr);
    bool JointLocalMap(CloudData::Cloud_Ptr& local_map_ptr);
    bool JointCloudMap(const std::deque<KeyFrame>& key_frames, CloudData::Cloud_Ptr& map_cloud_ptr);

private:
    std::string data_path_ = "";
    int local_frame_num_ = 20;

    std::string key_frames_path_ = "";
    std::string map_path_ = "";

    std::shared_ptr<CloudFilterInterface> frame_filter_ptr_;
    std::shared_ptr<CloudFilterInterface> local_map_filter_ptr_;
    std::shared_ptr<CloudFilterInterface> global_map_filter_ptr_;

    Eigen::Matrix4f pose_to_optimize_ = Eigen::Matrix4f::Identity();
    PoseData optimized_odom_;
    CloudData optimized_cloud_;
    std::deque<KeyFrame> optimized_key_frames_;
    std::deque<KeyFrame> all_key_frames_;

    bool has_new_global_map_ = false;
    bool has_new_local_map_ = false;

    bool debug_info_ = false;
};
}  // namespace lidar_localization

#endif