<?php
/**
 * @file
 * BitTorrent Sync API method(s); Get speed.
 */

/**
 * Get speed.
 *
 * Returns current upload and download speed.
 */
class BTSyncGetSpeed extends BTSyncMethod {
  protected $name = 'Get speed';
  protected $description = 'Returns current upload and download speed.';
  protected $method = 'get_speed';

  protected $return = array(
    'download' => array(
      'label' => 'Download speed',
      'type' => 'text',
    ),
    'upload' => array(
      'label' => 'Upload speed',
      'type' => 'text',
    ),
  );
}
