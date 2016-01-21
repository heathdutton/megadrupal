<?php
/**
 * @file
 * BitTorrent Sync API method(s); Get files.
 */

/**
 * Get folders.
 *
 * Returns list of files within the specified directory. If a directory is not
 * specified, will return list of files and folders within the root folder.
 */
class BTSyncGetFiles extends BTSyncMethod {
  protected $name = 'Get files';
  protected $description = 'Returns list of files within the specified directory. If a directory is not specified, will return list of files and folders within the root folder.';
  protected $method = 'get_files';

  protected $arguments = array(
    'secret' => array(
      'name' => 'Secret',
      'description' => 'Folder secret.',
      'type' => 'text',
      'required' => TRUE,
    ),
    'path' => array(
      'name' => 'Path',
      'description' => 'Path to a subfolder of the sync folder.',
      'type' => 'text',
      'required' => FALSE,
    ),
  );

  protected $return = array(
    '#list' => TRUE,
    'have_pieces' => array(
      'label' => 'Have pieces',
      'type' => 'decimal',
    ),
    'name' => array(
      'label' => 'Name',
      'type' => 'text',
    ),
    'size' => array(
      'label' => 'Size',
      'type' => 'decimal',
    ),
    'state' => array(
      'label' => 'State',
      'type' => 'text',
    ),
    'total_pieces' => array(
      'label' => 'Total pieces',
      'type' => 'decimal',
    ),
    'type' => array(
      'label' => 'Type',
      'type' => 'text',
    ),
    'download' => array(
      'label' => 'Download',
      'type' => 'boolean',
    ),
  );
}
