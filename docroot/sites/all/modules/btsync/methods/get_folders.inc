<?php
/**
 * @file
 * BitTorrent Sync API method(s); Get folders.
 */

/**
 * Get folders.
 *
 * Returns an array with folders info. If a secret is specified, will return
 * info about the folder with this secret.
 */
class BTSyncGetFolders extends BTSyncMethod {
  protected $name = 'Get folders';
  protected $description = 'Returns an array with folders info. If a secret is specified, will return info about the folder with this secret.';
  protected $method = 'get_folders';

  protected $arguments = array(
    'secret' => array(
      'name' => 'Secret',
      'description' => 'If specified, will return info about the folder with this secret.',
      'type' => 'text',
      'required' => FALSE,
    ),
  );

  protected $return = array(
    '#list' => TRUE,
    'dir' => array(
      'label' => 'Directory',
      'type' => 'text',
    ),
    'secret' => array(
      'label' => 'Secret',
      'type' => 'text',
    ),
    'size' => array(
      'label' => 'Size',
      'type' => 'decimal',
    ),
    'type' => array(
      'label' => 'Type',
      'type' => 'text',
    ),
    'files' => array(
      'label' => 'Files',
      'type' => 'decimal',
    ),
    'error' => array(
      'label' => 'Error',
      'type' => 'decimal',
    ),
    'error' => array(
      'label' => 'Error',
      'type' => 'decimal',
    ),
    'indexing' => array(
      'label' => 'Indexing',
      'type' => 'boolean',
    ),
  );
}
