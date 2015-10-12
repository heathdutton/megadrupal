<?php
/**
 * @file
 * BitTorrent Sync API method(s); Get secrets.
 */

/**
 * Get secrets.
 *
 * Generates read-write, read-only and encryption read-only secrets. If ‘secret’
 * parameter is specified, will return secrets available for sharing under this
 * secret.
 */
class BTSyncGetSecrets extends BTSyncMethod {
  protected $name = 'Get secrets';
  protected $description = 'Generates read-write, read-only and encryption read-only secrets. If ‘secret’ parameter is specified, will return secrets available for sharing under this secret.';
  protected $method = 'get_secrets';

  protected $arguments = array(
    'secret' => array(
      'name' => 'Secret',
      'description' => 'Folder secret.',
      'type' => 'text',
      'required' => FALSE,
    ),
    'type' => array(
      'name' => 'Type',
      'description' => 'If type=encrypted, generate secret with support of encrypted peer',
      'type' => 'text',
      'required' => FALSE,
    ),
  );

  protected $return = array(
    'read_only' => array(
      'label' => 'Read only',
      'type' => 'text',
    ),
    'read_write' => array(
      'label' => 'Read/write',
      'type' => 'text',
    ),
    'encryption' => array(
      'label' => 'Encryption',
      'type' => 'text',
    ),
  );
}
