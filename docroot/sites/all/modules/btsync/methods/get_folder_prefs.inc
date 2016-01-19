<?php
/**
 * @file
 * BitTorrent Sync API method(s); Get folder preferences.
 */

/**
 * Get folder preferences.
 *
 * Returns preferences for the specified sync folder.
 */
class BTSyncGetFolderPrefs extends BTSyncMethod {
  protected $name = 'Get folder preferences';
  protected $description = 'Returns preferences for the specified sync folder.';
  protected $method = 'get_folder_prefs';

  protected $arguments = array(
    'secret' => array(
      'name' => 'Secret',
      'description' => 'Folder secret.',
      'type' => 'text',
      'required' => TRUE,
    ),
  );

  protected $return = array(
    'search_lan' => array(
      'label' => 'Search LAN',
      'type' => 'boolean',
    ),
    'use_dht' => array(
      'label' => 'Use DHT',
      'type' => 'boolean',
    ),
    'use_hosts' => array(
      'label' => 'Use hosts',
      'type' => 'boolean',
    ),
    'use_relay_server' => array(
      'label' => 'User relay server',
      'type' => 'boolean',
    ),
    'use_sync_trash' => array(
      'label' => 'Use sync trash',
      'type' => 'boolean',
    ),
    'use_tracker' => array(
      'label' => 'Use tracker',
      'type' => 'boolean',
    ),
  );
}
