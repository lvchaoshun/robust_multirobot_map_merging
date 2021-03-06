// Copyright (C) 2018 by Pierre-Yves Lajoie <lajoie.py@gmail.com>

#include "global_map_solver/global_map_solver.h"
#include "findClique.h"

namespace global_map_solver {

const std::string GlobalMapSolver::CONSISTENCY_MATRIX_FILE_NAME = std::string("consistency_matrix.clq.mtx");
const std::string GlobalMapSolver::CONSISTENCY_LOOP_CLOSURES_FILE_NAME = std::string("consistent_loop_closures.txt");

GlobalMapSolver::GlobalMapSolver(const robot_local_map::RobotLocalMap& robot1_local_map,
                const robot_local_map::RobotLocalMap& robot2_local_map,
                const robot_local_map::RobotMeasurements& interrobot_measurements): 
                pairwise_consistency_(robot1_local_map.getTransforms(), robot2_local_map.getTransforms(), 
                            interrobot_measurements.getTransforms(), interrobot_measurements.getLoopClosures(),
                            robot1_local_map.getTrajectory(), robot2_local_map.getTrajectory(),
                            robot1_local_map.getNbDegreeFreedom()){}

int GlobalMapSolver::solveGlobalMap() {
    // Compute consistency matrix
    Eigen::MatrixXi consistency_matrix = pairwise_consistency_.computeConsistentMeasurementsMatrix();
    graph_utils::printConsistencyGraph(consistency_matrix, CONSISTENCY_MATRIX_FILE_NAME);
    
    // Compute maximum clique
    FMC::CGraphIO gio;
    gio.readGraph(CONSISTENCY_MATRIX_FILE_NAME);
    int max_clique_size = 0;
    std::vector<int> max_clique_data;
    max_clique_size = FMC::maxClique(gio, max_clique_size, max_clique_data);

    // Print results
    graph_utils::printConsistentLoopClosures(pairwise_consistency_.getLoopClosures(), max_clique_data, CONSISTENCY_LOOP_CLOSURES_FILE_NAME);

    // Clean up
    max_clique_data.clear();

    return max_clique_size;
}

}