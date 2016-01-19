<?php
/**
 * @file
 * BitTorrent Sync API method(s); Get folder hosts.
 */

/**
 * Get folder hosts.
 *
 * Returns list of predefined hosts for the folder, or error code if a secret is
 * not specified.
 */
class BTSyncGetFolderHosts extends BTSyncMethod {
  protected $name = 'Get folder hosts';
  protected $description = 'Returns list of predefined hosts for the folder, or error code if a secret is not specified.';
  protected $method = 'get_folder_hosts';

  protected $arguments = array(
    'secret' => array(
      'name' => 'Secret',
      'description' => 'Folder secret.',
      'type' => 'text',
      'required' => TRUE,
    ),
  );

  protected $return = array(
    'hosts' => array(
      'label' => 'Hosts',
      'type' => 'list<text>',
    ),
  );
}
