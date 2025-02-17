//
// Created by heyijia on 19-1-30.
//

#ifndef SLAM_COURSE_EDGE_PRIOR_H
#define SLAM_COURSE_EDGE_PRIOR_H

#include <memory>
#include <string>

#include <Eigen/Dense>

#include "eigen_types.h"
#include "edge.h"

namespace graph_optimization {
    /**
    * EdgeSE3Prior，此边为 1 元边，与之相连的顶点有：Ti
    */
    class EdgeSE3Prior : public Edge {
    public:
        EIGEN_MAKE_ALIGNED_OPERATOR_NEW;

        EdgeSE3Prior(const Vec3 &p, const Qd &q) :
                Edge(6, 1, std::vector<std::string>{"VertexPose"}),
                Pp_(p), Qp_(q) {}

        /// 返回边的类型信息
        std::string type_info() const override { return "EdgeSE3Prior"; }

        /// 计算残差
        void compute_residual() override;

        /// 计算雅可比
        void compute_jacobians() override;


    private:
        Vec3 Pp_;   // pose prior
        Qd   Qp_;   // Rotation prior
    };
}



#endif //SLAM_COURSE_EDGE_PRIOR_H
