<?php


/**
 * @file
 *   Farmacia cache form table drop and recreate module drush integration.
 */

function drop_cache_form_drush_command() {
  $items = array();

  $items['drop-cache-form'] = array(
    'description' => "Drop and create a new cache_form table.",
    'aliases' => array('dcf'),
  );

  return $items;
}


/**
 *
 */
function drush_drop_cache_form() {
  
  $droptable = db_drop_table('cache_form');

  if($droptable) {

    watchdog('Drop cache form', 'Cache form has been droped', $variables = array(), WATCHDOG_INFO, NULL);

    $schema['cache_form'] = array(
      'description' => 'Generic cache table for caching things not separated out into their own tables. Contributed modules may also use this to store cached items.',
      'fields' => array(
        'cid' => array(
          'description' => 'Primary Key: Unique cache ID.',
          'type' => 'varchar',
          'length' => 255,
          'not null' => TRUE,
          'default' => '',
        ),
        'data' => array(
          'description' => 'A collection of data to cache.',
          'type' => 'blob',
          'not null' => FALSE,
          'size' => 'big',
        ),
        'expire' => array(
          'description' => 'A Unix timestamp indicating when the cache entry should expire, or 0 for never.',
          'type' => 'int',
          'not null' => TRUE,
          'default' => 0,
        ),
        'created' => array(
          'description' => 'A Unix timestamp indicating when the cache entry was created.',
          'type' => 'int',
          'not null' => TRUE,
          'default' => 0,
        ),
        'serialized' => array(
          'description' => 'A flag to indicate whether content is serialized (1) or not (0).',
          'type' => 'int',
          'size' => 'small',
          'not null' => TRUE,
          'default' => 0,
        ),
      ),
      'indexes' => array(
        'expire' => array('expire'),
      ),
      'primary key' => array('cid'),
    );

   $createtable = db_create_table('cache_form', $schema['cache_form']);

    watchdog('Drop cache form', 'Cache form table has been created', $variables = array(), WATCHDOG_INFO, NULL);    

 }else {
    watchdog('Drop cache form', 'Cache form has not been droped', $variables = array(), WATCHDOG_INFO, NULL);  
 }
}

