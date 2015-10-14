<?php

/**
 * @file
 * Documentation for unity_webapp API.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Implement this hook add links to unity HUD.
 *
 * Other modules may implement hook_unity_webapp_addlinks()
 * to add links to unity HUD.
 *
 * @return array
 *  An array containing pairs of linknames and links.
 */
function hook_unity_webapp_addlinks() {
  return array(
    array(
      'name' => 'Administration',
      'href' => 'admin',
    ),
    array(
      'name' => 'Tasks',
      'href' => 'admin/tasks',
    ),
  );
}

/**
 * @} End of "addtogroup hooks".
 */
