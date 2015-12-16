<?php
/**
 * @file
 * BitTorrent Sync API method(s); Get version.
 */

/**
 * Get version.
 *
 * Returns BitTorrent Sync version.
 */
class BTSyncGetVersion extends BTSyncMethod {
  protected $name = 'Get version';
  protected $description = 'Returns BitTorrent Sync version.';
  protected $method = 'get_version';

  protected $return = array(
    'version' => array(
      'label' => 'Version',
      'type' => 'text',
    ),
  );
}
