<?php

/**
 * @file
 * Hooks provided by the Space Launches module.
 */

/**
 * Imports launches from a data source and returns an array of launch data.
 *
 * @return array
 *   Array of launch data arrays.
 */
function hook_space_launches_import() {
  $launches = array();

  $launches[] = array(
    // The source of the launch data.
    'source' => 'Kerbal Space Program',
    // The unique ID assigned to this launch by the source.
    'source_uid' => 9999,
    // The Unix timestamp when this launch was last updated by the source.
    'source_updated' => 1437146272,
    // The title of this launch.
    'title' => 'Exciting Rocket Launch',
    // The description of this launch.
    'description' => 'Putting stuff in space.',
    // The URL of this launch.
    'url' => 'http://example.com/',
    // The Unix timestamp of this launch.
    'time' => 1439722800,
    // Boolean indicating whether the launch time is exact.
    'time_is_exact' => 1,
  );

  return $launches;
}
