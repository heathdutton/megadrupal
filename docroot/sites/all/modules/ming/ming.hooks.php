<?php
/**
 * @file
 * Hook information for Ming
 */

/**
 * Declare connection information for Ming
 *
 * Allows modules to provide alternate connections to Ming for their own use.
 *
 * @return array
 *  An array of connection settings
 */
function hook_ming_settings() {

  $settings['my_connection_alias'] = array(
    'mongo_host' => 'localhost',
    'mongo_port' => '27017',
    'mongo_user' => 'my_user_name',
    'mongo_pass' => 'f@ncy_p@ssw0rd',
    'mongo_db' => 'my_database',
  );

  return $settings;
}
