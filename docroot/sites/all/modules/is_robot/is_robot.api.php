<?php

/**
 * @file
 * Hooks provided by the Is Robot? module.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Manipulate or replace the list of robots to check for.
 *
 * This hook is used to modify the array of robots that will be checked when is_robots()
 * is called. This hook should handle the $append flag accordingly as this is not done on
 * the module end. When set to TRUE the hook should replace &$robots with it's new array.
 * When set to false, it should append the data to the end of the array using $robots[]
 *
 * @param &$robots
 *  An array representing the robots to be appended or created anew.
 *    - user_agent - The user agent of the robot to be hooked
 *    - robot_id - A machine name for the robot listed in the user agent.
 * @param &$replace
 *   An array with the same associations as &$robots. This will be added to a replacement
 *   array to be assigned to $robots once all hooks are called.
 */

function hook_is_robots_robots($robots, &$replace) {
  // In this example we're assigning the same to both. The hook handler will determine if
  // $robots or $replace is used based on is_robot settings
  $robots[] = array(
    'user_agent' => 'My User Agent String',
    'robot-id' => 'my_user_agent_string',
  );
  $replace[] = array(
    'user_agent' => 'My User Agent String',
    'robot-id' => 'my_user_agent_string',
  );
}

/**
 * @} End of "addtogroup hooks".
 */
