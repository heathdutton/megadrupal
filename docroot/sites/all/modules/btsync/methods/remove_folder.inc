<?php
/**
 * @file
 * BitTorrent Sync API method(s); Remove folder.
 */

/**
 * Remove folder.
 *
 * Removes folder from Sync while leaving actual folder and files on disk. It
 * will remove a folder from the Sync list of folders and does not touch any
 * files or folders on disk.
 */
class BTSyncRemoveFolder extends BTSyncMethod {
  protected $name = 'Remove folder';
  protected $description = 'Removes folder from Sync while leaving actual folder and files on disk. It will remove a folder from the Sync list of folders and does not touch any files or folders on disk.';
  protected $method = 'remove_folder';

  protected $arguments = array(
    'secret' => array(
      'name' => 'Secret',
      'description' => 'Folder secret',
      'type' => 'text',
      'required' => TRUE,
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
}
