<?php
/**
 * @file
 * BitTorrent Sync API method(s); Set preferences.
 */

/**
 * Set preferences.
 *
 * Sets BitTorrent Sync preferences.
 */
class BTSyncSetPrefs extends BTSyncMethod {
  protected $name = 'Set preferences';
  protected $description = 'Sets BitTorrent Sync preferences.';
  protected $method = 'set_prefs';

  protected $arguments = array(
    'device_name' => array(
      'name' => 'Device name',
      'type' => 'text',
    ),
    'disk_low_priority' => array(
      'name' => 'Low disk priority',
      'type' => 'boolean',
    ),
    'download_limit' => array(
      'name' => 'Download limit',
      'type' => 'demical',
    ),
    'folder_rescan_interval' => array(
      'name' => 'Folder rescan interval',
      'type' => 'decimal',
    ),
    'lan_encrypt_data' => array(
      'name' => 'Encrypt data over LAN',
      'type' => 'boolean',
    ),
    'lan_use_tcp' => array(
      'name' => 'User TCP over LAN',
      'type' => 'boolean',
    ),
    'lang' => array(
      'name' => 'Language',
      'type' => 'decimal',
    ),
    'listening_port' => array(
      'name' => 'Listening port',
      'type' => 'decimal',
    ),
    'max_file_size_diff_for_patching' => array(
      'name' => 'Max file size diff for patching',
      'type' => 'decimal',
    ),
    'max_file_size_for_versioning' => array(
      'name' => 'Max file size for versioning',
      'type' => 'decimal',
    ),
    'rate_limit_local_peers' => array(
      'name' => 'Rate limit local peers',
      'type' => 'boolean',
    ),
    'send_buf_size' => array(
      'name' => 'Send buffer size',
      'type' => 'decimal',
    ),
    'sync_max_time_diff' => array(
      'name' => 'Sync maximum time difference',
      'type' => 'decimal',
    ),
    'sync_trash_ttl' => array(
      'name' => 'Sync trash time till deletion',
      'type' => 'decimal',
    ),
    'upload_limit' => array(
      'name' => 'Upload limit',
      'type' => 'decimal',
    ),
    'use_upnp' => array(
      'name' => 'Use UPnP',
      'type' => 'boolean',
    ),
    'recv_buf_size' => array(
      'name' => 'Receive buffer size',
      'type' => 'decimal',
    ),
  );

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
