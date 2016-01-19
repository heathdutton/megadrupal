<?php
/**
 * @file
 * Descriptions of the hooks provided by the Akamai Edge Control module.
 */

/**
 * Alter the max age that Akamai Edge Control will use for the current request.
 *
 * @param array $alter
 *   An associative array with a single key: 'max-age'.
 */
function hook_akamai_edge_control_max_age_alter(&$alter) {
  // Cache the front page for 30 seconds.
  if (drupal_is_front_page()) {
    $alter['max-age'] = 30;
  }
}
