<?php

/**
 * @file
 * Provides API documentation for the Conditional Flags module.
 */

/**
 * Implementation of hook_conditional_flags_conditions().
 *
 * This hook allows other modules to create additional custom conditions
 * for each flag.
 *
 * @see conditional_flags_get()
 */
function hook_conditional_flags_conditions() {
  return array(
    'x_flag' => array(
      'flag' => array('y_flag' => 'unflag'),
    ),
    'y_flag' => array(
      'flag' => array('x_flag' => 'unflag'),
    ),
  );
}