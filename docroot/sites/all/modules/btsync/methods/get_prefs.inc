<?php
/**
 * @file
 * BitTorrent Sync API method(s); Get preferences.
 */

/**
 * Get preferences.
 *
 * Returns BitTorrent Sync preferences. Contains dictionary with advanced
 * preferences. Please see Sync user guide for description of each option.
 */
class BTSyncGetPrefs extends BTSyncMethod {
  protected $name = 'Get preferences';
  protected $description = 'Returns BitTorrent Sync preferences. Contains dictionary with advanced preferences. Please see Sync user guide for description of each option.';
  protected $method = 'get_prefs';

  protected $return = array(
    'device_name' => array(
      'label' => 'Device name',
      'type' => 'text',
    ),
    'disk_low_priority' => array(
      'label' => 'Low disk priority',
      'type' => 'boolean',
    ),
    'download_limit' => array(
      'label' => 'Download limit',
      'type' => 'demical',
    ),
    'folder_rescan_interval' => array(
      'label' => 'Folder rescan interval',
      'type' => 'decimal',
    ),
    'lan_encrypt_data' => array(
      'label' => 'Encrypt data over LAN',
      'type' => 'boolean',
    ),
    'lan_use_tcp' => array(
      'label' => 'User TCP over LAN',
      'type' => 'boolean',
    ),
    'lang' => array(
      'label' => 'Language',
      'type' => 'decimal',
    ),
    'listening_port' => array(
      'label' => 'Listening port',
      'type' => 'decimal',
    ),
    'max_file_size_diff_for_patching' => array(
      'label' => 'Max file size diff for patching',
      'type' => 'decimal',
    ),
    'max_file_size_for_versioning' => array(
      'label' => 'Max file size for versioning',
      'type' => 'decimal',
    ),
    'rate_limit_local_peers' => array(
      'label' => 'Rate limit local peers',
      'type' => 'boolean',
    ),
    'send_buf_size' => array(
      'label' => 'Send buffer size',
      'type' => 'decimal',
    ),
    'sync_max_time_diff' => array(
      'label' => 'Sync maximum time difference',
      'type' => 'decimal',
    ),
    'sync_trash_ttl' => array(
      'label' => 'Sync trash time till deletion',
      'type' => 'decimal',
    ),
    'upload_limit' => array(
      'label' => 'Upload limit',
      'type' => 'decimal',
    ),
    'use_upnp' => array(
      'label' => 'Use UPnP',
      'type' => 'boolean',
    ),
    'recv_buf_size' => array(
      'label' => 'Receive buffer size',
      'type' => 'decimal',
    ),
  );
}
