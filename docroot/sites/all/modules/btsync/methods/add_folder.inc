<?php
/**
 * @file
 * BitTorrent Sync API method(s); Add folder.
 */

/**
 * Add folder.
 *
 * Adds a folder to Sync. If a secret is not specified, it will be generated
 * automatically. The folder will have to pre-exist on the disk and Sync will
 * add it into a list of syncing folders.
 */
class BTSyncAddFolder extends BTSyncMethod {
  protected $name = 'Add folder';
  protected $description = 'Adds a folder to Sync. If a secret is not specified, it will be generated automatically. The folder will have to pre-exist on the disk and Sync will add it into a list of syncing folders.';
  protected $method = 'add_folder';

  protected $arguments = array(
    'dir' => array(
      'name' => 'Directory',
      'description' => 'Path to the sync folder',
      'type' => 'text',
      'required' => TRUE,
    ),
    'secret' => array(
      'name' => 'Secret',
      'description' => 'Folder secret',
      'type' => 'text',
      'required' => FALSE,
    ),
    'selective_sync' => array(
      'name' => 'Selective sync',
      'type' => 'boolean',
      'required' => FALSE,
    ),
  );

  protected $return = array(
    'message' => array(
      'label' => 'Message',
      'type' => 'text',
    ),
    'result' => array(
      'label' => 'Result',
      'type' => 'decimal',
    ),
  );

  /**
   * Preprocess arguments callback.
   */
  public function preprocessArgument($argument, &$value) {
    if ('dir' == $argument) {
      $value = drupal_realpath($value);
    }
  }
}
