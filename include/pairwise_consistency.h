// author: Pierre-Yves Lajoie <lajoie.py@gmail.com>

#ifndef PAIRWISE_CONSISTENCY_H
#define PAIRWISE_CONSISTENCY_H

#include "graph_utils.h"
#include "geometry_msgs/PoseWithCovariance.h"

#include <eigen3/Eigen/Geometry>

namespace robust_multirobot_slam {

    /**
     * Class for the computation of the pairwise consistency of loop closure edges
     */ 
    class PairwiseConsistency {
      public:
        /**
         * Constructor
         */
        PairwiseConsistency(const graph_utils::TransformMap& transforms_robot1,
                            const graph_utils::TransformMap& transforms_robot2,
                            const graph_utils::TransformMap& transforms_interrobot,
                            const std::list<std::pair<size_t,size_t>>& loop_closure_list,
                            const graph_utils::Trajectory& trajectory_robot1,
                            const graph_utils::Trajectory& trajectory_robot2):
                            loop_closure_list_(loop_closure_list), transforms_robot1_(transforms_robot1), 
                            transforms_robot2_(transforms_robot2), transforms_interrobot_(transforms_interrobot),
                            trajectory_robot1_(trajectory_robot1), trajectory_robot2_(trajectory_robot2){};

        /**
         * Computation of the consistency matrix
         */ 
        Eigen::MatrixXi computeConsistentMeasurementsMatrix(const double& threshold);

      private:

        geometry_msgs::PoseWithCovariance computeConsistencyPose(const geometry_msgs::PoseWithCovariance& aXij, 
                                                        const geometry_msgs::PoseWithCovariance& bXlk, 
                                                        const geometry_msgs::PoseWithCovariance& abZik, 
                                                        const geometry_msgs::PoseWithCovariance& abZjl);

        double computeSquaredMahalanobisDistance(const geometry_msgs::PoseWithCovariance& pose);

        geometry_msgs::PoseWithCovariance composeOnTrajectory(const size_t& id1, const size_t& id2, const size_t& robot_id);

        graph_utils::Trajectory trajectory_robot1_, trajectory_robot2_;

        std::list<std::pair<size_t,size_t>> loop_closure_list_;

        graph_utils::TransformMap transforms_robot1_, transforms_robot2_, transforms_interrobot_;

    };          

}

#endif