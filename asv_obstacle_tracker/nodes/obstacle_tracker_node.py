#!/usr/bin/env python

import rospy

from nav_msgs.msg import Odometry
from asv_msgs.msg import State, StateArray

import numpy as np

def quat2yaw(q):
    return np.arctan2(2*(q[1]*q[2] + q[3]*q[0]), 1 - 2*(q[2]**2 + q[3]**2))


def obstacleCallback(data, num):
    q = (data.pose.pose.orientation.x,
         data.pose.pose.orientation.y,
         data.pose.pose.orientation.z,
         data.pose.pose.orientation.w)

    statearray.states[num].x = data.pose.pose.position.x
    statearray.states[num].y = data.pose.pose.position.y
    statearray.states[num].psi = quat2yaw(q)
    statearray.states[num].u = data.twist.twist.linear.x
    statearray.states[num].v = data.twist.twist.linear.y
    statearray.states[num].r = data.twist.twist.angular.z

    pub.publish(statearray)

if __name__ == "__main__":
    rospy.init_node("obstacle_tracker_node")

    ships = rospy.get_param("/obstacles")


    statearray = StateArray()

    subscriber_list = []
    num = 0

    for ship in ships:
        statearray.states.append(State())
        subscriber_list.append(rospy.Subscriber("/obstacles/" + str(ship) + "/state", Odometry, obstacleCallback, num))
        statearray.states[num].header.id = num
        statearray.states[num].header.name = str(ship)
        statearray.states[num].header.radius = ships[str(ship)][str(ship)]['radius']
        num += 1

    pub = rospy.Publisher("/obstacle_states", StateArray, queue_size=1)

    rospy.spin()
