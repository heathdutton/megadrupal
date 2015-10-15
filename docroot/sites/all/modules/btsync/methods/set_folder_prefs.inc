<?php
/**
 * @file
 * BitTorrent Sync API method(s); Set folder preferences.
 */

/**
 * Set folder preferences.
 *
 * Sets preferences for the specified sync folder.
 */
class BTSyncSetFolderPrefs extends BTSyncMethod {
  protected $name = 'Set folder preferences';
  protected $description = 'Sets preferences for the specified sync folder.';
  protected $method = 'set_folder_prefs';

  protected $arguments = array(
    'secret' => array(
      'name' => 'Secret',
      'description' => 'Folder secret.',
      'type' => 'text',
      'required' => TRUE,
    ),
    'search_lan' => array(
      'name' => 'Search LAN',
      'type' => 'boolean',
    ),
    'use_dht' => array(
      'name' => 'Use DHT',
      'type' => 'boolean',
    ),
    'use_hosts' => array(
      'name' => 'Use hosts',
      'type' => 'boolean',
    ),
    'use_relay_server' => array(
      'name' => 'User relay server',
      'type' => 'boolean',
    ),
    'use_sync_trash' => array(
      'name' => 'Use sync trash',
      'type' => 'boolean',
    ),
    'use_tracker' => array(
      'name' => 'Use tracker',
      'type' => 'boolean',
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
