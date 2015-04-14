#ifndef ASV_CTRL_VO_PLUGIN
#define ASV_CTRL_VO_PLUGIN

#include <Eigen/Dense>

#include "asv_msgs/State.h"
#include "nav_msgs/OccupancyGrid.h"
#include "nav_msgs/Odometry.h"
#include "visualization_msgs/Marker.h"

static const double VELOCITY_OK = 0.0;
static const double VELOCITY_VIOLATES_COLREGS = 0.75;
static const double VELOCITY_NOT_OK = 1.0;
static const int OCCUPIED_TRESH = 50;

class VelocityObstacle
{
 public:
  /// Constructor
  VelocityObstacle();
  /// Destructor
  ~VelocityObstacle();

  /**
   * @brief Initializes the controller.
   *
   * @param obstacles A pointer to a vector of obstacles provided by the ROS 
   * wrapper (by subscribing to an obstacle tracker node).
   * @param map A pointer to the occupancy grid published by the map_server.
   */
  void initialize(std::vector<asv_msgs::State> *obstacles, nav_msgs::OccupancyGrid *map);
  /**
   * @brief Updates the Velocity Obstacle.
   */
  void update();

  /**
   * Initializes the visualization markers used to display the velocity field.
   *
   * @param marker A pointer to a marker message (provided by the ROS wrapper).
   */
  void initializeMarkers(visualization_msgs::Marker *marker);

  /**
   * @brief Called after the velocity field has been updated to get the (u, psi) pair
   * with the lowest cost.
   *
   * @param u_best The reference parameter to store the "best" surge speed.
   * @param psi_best The reference parameter to store the "best" heading.
   */
  void getBestControlInput(double &u_best, double &psi_best);
  
 private:
  void setVelocity(const int &ui, const int &ti, const double &val);
  void updateVelocityGrid();
  void clearVelocityGrid();
  void checkStaticObstacles();
  
  bool inVelocityObstacle(const double &u,
                          const double &theta,
                          const Eigen::Vector2d &lb,
                          const Eigen::Vector2d &rb,
                          const Eigen::Vector2d &vb);
  bool violatesColregs(const double &u,
                       const double &theta,
                       const Eigen::Vector3d &obstacle_pose,
                       const Eigen::Vector2d &vb);
  
  bool inObstacle(double px, double py);

  const double RADIUS_;
  const double MAX_VEL_;
  const double MAX_ANG_;
  const double GRID_RES_;
  const double MIN_DIST_;

  const int EDGE_SAMPLES_;
  const int VEL_SAMPLES_;
  const int ANG_SAMPLES_;

  std::vector<double> vo_grid_;

  double u_d_;
  double psi_d_;

  double costmap_resolution_;

  // ROS API
  // void obstacleSubCallback(const nav_msgs::Odometry::ConstPtr &msg);
  std::vector<asv_msgs::State> *obstacles_;
  costmap_2d::costmap2D *costmap_;
  visualization_msgs::Marker *marker_;
};


#endif