<?php
/**
 * @file
 * BitTorrent Sync API method(s); Get folder peers.
 */

/**
 * Get folder peers.
 *
 * Returns list of peers connected to the specified folder.
 */
class BTSyncGetFolderPeers extends BTSyncMethod {
  protected $name = 'Get folder peers';
  protected $description = 'Returns an array with folders info. If a secret is specified, will return info about the folder with this secret.';
  protected $method = 'get_folder_peers';

  protected $arguments = array(
    'secret' => array(
      'name' => 'Secret',
      'description' => 'Folder secret.',
      'type' => 'text',
      'required' => TRUE,
    ),
  );

  protected $return = array(
    '#list' => TRUE,
    'id' => array(
      'label' => 'ID',
      'type' => 'text',
    ),
    'connection' => array(
      'label' => 'Connection',
      'type' => 'text',
    ),
    'name' => array(
      'label' => 'Name',
      'type' => 'text',
    ),
    'synced' => array(
      'label' => 'Synced',
      'type' => 'date',
    ),
    'download' => array(
      'label' => 'Download',
      'type' => 'decimal',
    ),
    'upload' => array(
      'label' => 'Upload',
      'type' => 'decimal',
    ),
  );
}
