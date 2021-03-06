// Copyright (C) 2018 by Pierre-Yves Lajoie <lajoie.py@gmail.com>

#include "graph_utils/graph_utils_functions.h"
#include "pairwise_consistency/pairwise_consistency.h"
#include "robot_local_map/robot_local_map.h"
#include "global_map_solver/global_map_solver.h"
#include "findClique.h"

#include <string>
#include <iostream>
#include <eigen3/Eigen/Geometry>
#include <chrono>

#define THRESHOLD 1.635

const std::string CONSISTENCY_MATRIX_FILE_NAME = "consistency_matrix.clq.mtx";

int main(int argc, char* argv[])
{
  std::cout << "---------------------------------------------------------" << std::endl;
  // Parse arguments
  std::string robot1_file_name, robot2_file_name, interrobot_file_name;
  if (argc < 4) {
    std::cout << "Not enough arguments, please specify at least 3 input files. (format supported : .g2o)" << std::endl;
    return -1;
  } else {
    robot1_file_name = argv[1];
    robot2_file_name = argv[2];
    interrobot_file_name = argv[3];
  }

  std::cout << "Construction of local maps from the following files : " << robot1_file_name << ", " << std::endl << robot2_file_name << ", " << std::endl << interrobot_file_name;
  auto start = std::chrono::high_resolution_clock::now();

  //--- Map construction
  auto robot1_local_map = robot_local_map::RobotLocalMap(robot1_file_name);
  auto robot2_local_map = robot_local_map::RobotLocalMap(robot2_file_name);
  auto interrobot_measurements = robot_local_map::RobotMeasurements(interrobot_file_name, true);
  //---
  
  auto finish = std::chrono::high_resolution_clock::now();
  auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(finish-start);
  std::cout << " | Completed (" << milliseconds.count() << "ms)" << std::endl;
  
  std::cout << "Solving global map." ;
  start = std::chrono::high_resolution_clock::now();

  //--- Solve global map
  auto solver = global_map_solver::GlobalMapSolver(robot1_local_map, robot2_local_map, interrobot_measurements); 
  int max_clique_size = solver.solveGlobalMap();
  //---

  finish = std::chrono::high_resolution_clock::now();
  milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(finish-start);
  std::cout << " | Completed (" << milliseconds.count() << "ms)" << std::endl;
  std::cout << "Maximum clique size = " << max_clique_size << std::endl;

  return 0;
}