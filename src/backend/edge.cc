//
// Created by Cain on 2023/2/20.
//

#include <iostream>

#include "backend/edge.h"
#include "backend/vertex.h"

namespace graph_optimization {
    unsigned long Edge::_global_edge_id = 0;

    Edge::Edge(unsigned long residual_dimension, unsigned long num_vertices, const std::vector<std::string> &vertices_types, unsigned loss_function_type) {
        _residual.resize(residual_dimension, 1);
        _vertices.reserve(num_vertices);
        if (!vertices_types.empty()) {
            _vertices_types = vertices_types;
        }
        _jacobians.resize(num_vertices);
        _id = _global_edge_id++;

        _information.resize(residual_dimension, residual_dimension);
        _information.setIdentity();

        switch (loss_function_type) {
            case 1:
                _loss_function = new HuberLoss(1.);
                break;
            case 2:
                _loss_function = new CauchyLoss(1.);
                break;
            case 3:
                _loss_function = new TukeyLoss(2.);
                break;
            default:
                _loss_function = new TrivialLoss;
                break;
        }
    }

    Edge::~Edge() {
        delete _loss_function;
    }

    void Edge::compute_chi2() {
        _chi2 = _residual.transpose() * _information * _residual;
        _rho = _loss_function->compute(_chi2);
    }

    void Edge::robust_information(double &drho, MatXX &info) const {
        VecX w_e = _information * _residual;

        MatXX robust_info(_information.rows(), _information.cols());
        robust_info.setIdentity();
        robust_info *= _rho[1] * _information;
        if(_rho[1] + 2 * _rho[2] * _chi2 > 0.) {
            robust_info += 2 * _rho[2] * w_e * w_e.transpose();
        }

        info = robust_info;
        drho = _rho[1];
    }

    bool Edge::check_valid() {
        if (!_vertices_types.empty()) {
            // check type info
            for (size_t i = 0; i < _vertices.size(); ++i) {
                if (_vertices_types[i] != _vertices[i]->type_info()) {
                    std::cout << "Vertex type does not match, should be " << _vertices_types[i] <<
                              ", but set to " << _vertices[i]->type_info() << std::endl;
                    return false;
                }
            }
        }
        return true;
    }
}

