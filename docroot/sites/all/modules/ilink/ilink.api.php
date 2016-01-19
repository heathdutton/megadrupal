<?php

/**
 * @file
 * Contains API documentation!
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Alter the ilink build array just before it is returned.
 *
 * @param  $element
 *   The render array that you'll want to alter.
 */
function hook_ilink_build_alter(&$element) {
  if ($element['#ilink'] == 'mymodule_closebutton') {
    $element['js events'] = array('mousedown');
  }
}

/**
 * @} End of "addtogroup hooks".
 */
