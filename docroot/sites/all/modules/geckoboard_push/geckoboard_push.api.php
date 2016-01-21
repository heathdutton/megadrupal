<?php
/**
 * @file
 * Exposed hooks in 7.x.
 */

/**
 * Lets modules register their data sending functions with geckoboard_push.
 */
function hook_geckoboard_push_info() {
  return array(
    array(
      'function' => 'geckoboard_push_random_number',
      'name' => t('Geckoboard Push Random Number'),
    ),
  );
}
