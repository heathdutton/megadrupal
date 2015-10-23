<?php
/**
 * @file
 * BitTorrent Sync API method(s); Shutdown.
 */

/**
 * Shutdown.
 *
 * Removes folder from Sync while leaving actual folder and files on disk. It
 * will remove a folder from the Sync list of folders and does not touch any
 * files or folders on disk.
 */
class BTSyncShutdown extends BTSyncMethod {
  protected $name = 'Shutdown';
  protected $description = 'Gracefully stops Sync.';
  protected $method = 'shutdown';

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
