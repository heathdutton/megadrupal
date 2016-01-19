<?php
/**
 * @file
 * DSB config file.
 */

$config['build tag'] = 'sharedcontent';

$config['domain'] = 'sharedcontent.dev';

$config['makefile'] = 'dsb.sharedcontent.make';

/**
 * Simple development setup.
 *
 * Creates an instance of each SC node type.
 */
$config['scenarios']['dev'] = array(
  'custom variables' => array(
    'SC_SERVERS' => 's1,sc1',
    'SC_CLIENTS' => 'c1,sc1',
  ),
  'post install script' => array('./scripts/sync.sh'),
  'cleanup script' => array('./scripts/cleanup.sh'),
  'hosts' => array(
    's1' => array(
      'setup script' => array(
        './scripts/setup/common.sh',
        './scripts/setup/server.sh',
      ),
    ),
    'sc1' => array(
      'setup script' => array(
        './scripts/setup/common.sh',
        './scripts/setup/server.sh',
        './scripts/setup/client.sh',
      ),
    ),
    'c1' => array(
      'setup script' => array(
        './scripts/setup/common.sh',
        './scripts/setup/client.sh',
      ),
    ),
  ),
);

/**
 * Automated tests.
 */
$config['scenarios']['test'] = array(
  'cleanup script' => array('./scripts/cleanup.sh'),
  'hosts' => array(
    'test' => array(),
  ),
  'tests' => array(
    'all' => array(
      'test objects' => array('"Shared Content"'),
    ),
  ),
);

// Include local config file with config overrides.
if (file_exists(dirname(__FILE__) . '/local.dsb.config.php')) {
  include dirname(__FILE__) . '/local.dsb.config.php';
}
