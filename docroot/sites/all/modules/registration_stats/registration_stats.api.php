<?php
/**
 * @file
 * Provides API documentation.
 */

/**
 * Implements hook_registration_stats().
 *
 * @param string $type
 *   The type of stats.
 *
 * @return array
 *   An array of social networks stats. Each menu item has a key corresponding to
 *   the social network name. The corresponding array value is an associative array
 *   that contain the period of time as a key and count of registrations as a value.
 *
 * @see registration_stats_all_page()
 * @see registration_stats_year_page()
 * @see registration_stats_month_page()
 */

function hook_registration_stats($type) {
  switch ($type) {
    case 'all':
      return array(
        'pornhub' => array(
          '2001' => 2,
          '2002' => 8,
          '2003' => 16,
        ),
      );

    case 'year':
      return array(
        'pornhub' => array(
          '2015 January'  => 32,
          '2015 February' => 64,
          '2015 March'    => 64,
        ),
      );

    case 'month':
      return array(
        'pornhub' => array(
          '2015-06-13' => 128,
          '2015-06-14' => 256,
          '2015-06-15' => 512,
        ),
      );
  }
}
