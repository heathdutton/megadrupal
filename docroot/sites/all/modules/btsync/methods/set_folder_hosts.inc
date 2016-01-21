<?php
/**
 * @file
 * BitTorrent Sync API method(s); Set folder hosts.
 */

/**
 * Set folder hosts.
 *
 * Sets one or several predefined hosts for the specified sync folder. Existing
 * list of hosts will be replaced.
 */
class BTSyncSetFolderHosts extends BTSyncMethod {
  protected $name = 'Set folder hosts';
  protected $description = 'Sets one or several predefined hosts for the specified sync folder. Existing list of hosts will be replaced.';
  protected $method = 'set_folder_hosts';

  protected $arguments = array(
    'secret' => array(
      'name' => 'Secret',
      'description' => 'Folder secret.',
      'type' => 'text',
      'required' => TRUE,
    ),
    'hosts' => array(
      'name' => 'Hosts',
      'description' => 'enter list of hosts separated by comma. Host should be represented as “[address]:[port]”.',
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
