<?php
/**
 * @file
 * BitTorrent Sync API method(s); Set file preferences.
 */

/**
 * Set file preferences.
 *
 * Selects file for download for selective sync folders. Returns file
 * information with applied preferences.
 */
class BTSyncSetFilePrefs extends BTSyncMethod {
  protected $name = 'Set file preferences';
  protected $description = 'Selects file for download for selective sync folders. Returns file information with applied preferences.';
  protected $method = 'set_file_prefs';

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
      'required' => TRUE,
    ),
    'download' => array(
      'name' => 'Download',
      'description' => 'Should be downloaded?',
      'type' => 'boolean',
      'required' => TRUE,
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
