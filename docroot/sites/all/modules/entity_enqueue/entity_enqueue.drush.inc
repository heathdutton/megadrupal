<?php

/**
 * @file entity_enqueue.drush.inc
 *   Drush integration for Entity Enqueue.
 */

/**
 * Implementation of hook_drush_command().
 */
function entity_enqueue_drush_command() {
  $items = array();

  $data_option = array(
    'description' => dt('Arbitrary data to be associated with the new task in the queue, JSON-encoded.'),
    'example-value' => '{\'parent_nid\':517}',
    'value' => 'required',
  );

  $items['entity-enqueue'] = array(
    'description' => dt('Enqueue an entity into a Drupal queue.'),
    'required-arguments' => TRUE,
    'arguments' => array(
      'queue_name' => dt('Queue name where to enqueue the entity.'),
      'entity_id' => dt('Entity IDs to enqueue. Use \'-\' to read EOL-delimited IDs from STDIN.'),
    ),
    'options' => array(
      'type' => array(
        'description' => dt('Optional. Specify entity type. Default is \'node\'.'),
        'example-value' => 'node,user,file,...',
        'value' => 'required',
      ),
      'data' => $data_option,
    ),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_FULL,
    'aliases' => array('eeq'),
    'examples' => array(
      'drush entity-enqueue my-queue 1' => 'Enqueues entity ID 1 of default entity type (node) into \'my-queue\' queue',
      'drush entity-enqueue my-queue 1 2 3' => 'Enqueues entity IDs 1, 2 and 3 of default entity type (node) into \'my-queue\' queue',
      'drush entity-enqueue my-queue -' => 'Enqueues entity IDs read from STDIN of default entity type (node) into \'my-queue\' queue',
      'drush entity-enqueue my-queue 1 --type=user' => 'Enqueues user 1 into \'my-queue\' queue',
    ),
  );

  $items['entity-enqueue-file'] = array(
    'description' => dt('Enqueue a managed file into a Drupal queue.'),
    'required-arguments' => TRUE,
    'arguments' => array(
      'queue_name' => dt('Queue name where to enqueue the file.'),
      'fid' => dt('File IDs to enqueue. Use \'-\' to read EOL-delimited IDs from STDIN.'),
    ),
    'options' => array(
      'data' => $data_option,
     ),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_FULL,
    'aliases' => array('eeqf'),
    'examples' => array(
      'drush entity-enqueue-file my-queue 1' => 'Enqueues File ID 1 into \'my-queue\' queue.',
    ),
  );

  $items['entity-enqueue-node'] = array(
    'description' => dt('Enqueue a node into a Drupal queue.'),
    'required-arguments' => TRUE,
    'arguments' => array(
      'queue_name' => dt('Queue name where to enqueue the node.'),
      'nid'  => dt('Node IDs to enqueue. Use \'-\' to read EOL-delimited IDs from STDIN.'),
    ),
    'options' => array(
      'data' => $data_option,
    ),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_FULL,
    'aliases' => array('eeqn'),
    'examples' => array(
      'drush entity-enqueue-node my-queue 1' => 'Enqueues Node ID 1 into \'my-queue\' queue.',
    ),
  );

  $items['entity-enqueue-user'] = array(
    'description' => dt('Enqueue a user into a Drupal queue.'),
    'required-arguments' => TRUE,
    'arguments' => array(
      'queue_name' => dt('Queue name where to enqueue the user.'),
      'uid' => dt('User IDs to enqueue.'),
    ),
    'options' => array(
      'data' => $data_option,
    ),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_FULL,
    'aliases' => array('eequ'),
    'examples' => array(
      'drush entity-enqueue-user my-queue 1' => 'Enqueues User ID 1 into \'my-queue\' queue.',
    ),
  );

  return $items;
}

function drush_entity_enqueue($queue_name, $entity_id) {
  $entity_type = drush_get_option('type', 'node');

  _drush_entity_enqueue($queue_name, $entity_id, $entity_type);
}

function drush_entity_enqueue_file($queue_name, $fid) {
  _drush_entity_enqueue($queue_name, $fid, 'file');
}

function drush_entity_enqueue_node($queue_name, $nid) {
  _drush_entity_enqueue($queue_name, $nid, 'node');
}

function drush_entity_enqueue_user($queue_name, $uid) {
  _drush_entity_enqueue($queue_name, $uid, 'user');
}

/**
 * Command argument complete callback. Provides argument
 * values for shell completion.
 *
 * @return
 *  Array of popular fillings.
 */
function entity_enqueue_entity_enqueue_complete() {
  drush_bootstrap(DRUSH_BOOTSTRAP_DRUPAL_FULL);
  return array(
    'values' => array_keys(module_invoke_all('cron_queue_info')),
  );
}

function _drush_entity_enqueue($queue_name, $entity_id, $entity_type) {
  $data = _drush_entity_enqueue_get_data_option();

  // Shift command name
  drush_shift();

  // Shift queue name
  $arg = drush_shift();
  if (is_numeric($arg)) {
    $msg = 'The queue name argument looks like an entity number. ';
    $msg .= 'Perhaps you forgot to specify it. Continue anyway?';
    if (!drush_confirm(dt($msg))) {
      return drush_user_abort();
    }
  }

  if ($entity_id == '-') {
    drush_log(dt('Reading entity IDs from STDIN.'));
    $entity_id_array = explode(PHP_EOL,stream_get_contents(STDIN));
    drush_shift();  // Shift '-' to allow following command line IDs
  }

  while($entity_id = drush_shift()) {
    $entity_id_array[] = $entity_id;
  }

  foreach($entity_id_array as $id) {
    $id = trim($id);
    // Skip empty lines
    if (empty($id)) {
      continue;
    }
    entity_enqueue_entity($queue_name, $id, $entity_type, $data);
  }
}

function _drush_entity_enqueue_get_data_option() {
  $data = drush_get_option('data', array());

  if (!empty($data)) {
    drush_log('Data (string): ' . print_r($data, TRUE), 'notice');

    $data = drush_json_decode($data, TRUE);
    drush_log('Data (decoded):' . PHP_EOL . trim(print_r($data, TRUE)), 'notice');
  }
  return $data;
}
