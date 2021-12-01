/*
 * @Description: 前端里程计算法
 * @Author: Ren Qian
 * @Date: 2020-02-04 18:52:45
 */
#ifndef MAPPING_LOCALIZATION_MAPPING_FRONT_END_FRONT_END_HPP_
#define MAPPING_LOCALIZATION_MAPPING_FRONT_END_FRONT_END_HPP_

#include <yaml-cpp/yaml.h>

#include <Eigen/Dense>
#include <deque>

#include "mapping_localization/models/cloud_filter/cloud_filter_interface.hpp"
#include "mapping_localization/models/registration/registration_interface.hpp"
#include "mapping_localization/sensor_data/cloud_data.hpp"
#include "mapping_localization/tools/tic_toc.hpp"

namespace mapping_localization {
class FrontEnd {
public:
    struct Frame {
        Eigen::Matrix4f pose = Eigen::Matrix4f::Identity();
        CloudData cloud_data;
    };

public:
    FrontEnd(const YAML::Node& global_node, const YAML::Node& config_node);

    bool Update(const CloudData& cloud_data, Eigen::Matrix4f& cloud_pose);
    bool SetInitPose(const Eigen::Matrix4f& init_pose);

private:
    bool InitWithConfig();
    bool InitParam(const YAML::Node& config_node);
    bool InitRegistration(std::shared_ptr<RegistrationInterface>& registration_ptr, const YAML::Node& config_node);
    bool InitFilter(std::string filter_user,
                    std::shared_ptr<CloudFilterInterface>& filter_ptr,
                    const YAML::Node& config_node);
    bool UpdateWithNewFrame(const Frame& new_key_frame);

private:
    std::string data_path_ = "";

    std::shared_ptr<CloudFilterInterface> frame_filter_ptr_;
    std::shared_ptr<CloudFilterInterface> local_map_filter_ptr_;
    std::shared_ptr<RegistrationInterface> registration_ptr_;

    std::deque<Frame> local_map_frames_;

    CloudData::Cloud_Ptr local_map_ptr_;
    Frame current_frame_;

    Eigen::Matrix4f init_pose_ = Eigen::Matrix4f::Identity();

    float key_frame_distance_ = 2.0;
    int local_frame_num_ = 20;

    Timer matching_timer_ = Timer("Scan Matching");
};
}  // namespace mapping_localization

#endif