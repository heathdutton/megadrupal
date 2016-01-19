<?php
/**
 * @file
 * BitTorrent Sync API method(s); Get OS name.
 */

/**
 * Get OS name.
 *
 * Returns OS name where BitTorrent Sync is running.
 */
class BTSyncGetOs extends BTSyncMethod {
  protected $name = 'Get OS name';
  protected $description = 'Returns OS name where BitTorrent Sync is running.';
  protected $method = 'get_os';

  protected $return = array(
    'os' => array(
      'label' => 'Operating system',
      'type' => 'text',
    ),
  );
}
