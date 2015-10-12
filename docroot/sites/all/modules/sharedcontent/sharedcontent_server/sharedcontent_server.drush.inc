<?php

/**
 * @file
 * Drush commands for administer Shared Content server.
 */

/**
 * Implements hook_drush_command().
 */
function sharedcontent_server_drush_command() {
  $items = array();

  $items['sharedcontent-server-keywords'] = array(
    'description' => 'Manage taxonomy fields used when building index keywords.',
    'aliases' => array('sc-keywords'),
    'examples' => array(
      'drush sc-keywords add field_tags article' => dt('Use the field field_tags on articles.'),
      'drush sc-keywords remove field_tags basic_page node' => dt('Do no longer use the field field_tags on articles.'),
    ),
    'arguments' => array(
      'action' => dt('action to be done. Can be add or remove.'),
      'field' => dt('The field the action should be applied.'),
      'bundle' => dt('The bundle the action should be applied.'),
      'entity_type' => dt('The entity the action should be applied. If left blank defaults to node'),
    ),
//    'drupal dependencies' => array('sharedcontent_server'),
  );

  return $items;
}

/**
 * Mark or unmark a taxonomy field to be used for index keywords.
 *
 * @param $action
 *   Action to be taken. Can be either add or remove.
 * @param $field
 *   The field in question.
 * @param $bundle
 *   The bundle in question.
 * @param $entity_type
 *   The entity type in question. Defaults to node.
 */
function drush_sharedcontent_server_keywords($action, $field, $bundle, $entity_type = 'node') {
  if (!in_array($action, array('add', 'remove'))) {
    drush_log(dt("Invalid action. Use either add or remove."), 'error');
    return;
  }

  $info = entity_get_info($entity_type);

  if (empty($info)) {
    drush_log(dt("Invalid entity type !type given.", array('!type' => $entity_type)), 'error');
    return;
  }

  if (!array_key_exists($bundle, $info['bundles'])) {
    drush_log(dt("Invalid bundle !bundle given.", array('!bundle' => $bundle)), 'error');
    return;
  }

  $fields = variable_get('sharedcontent_keyword_fields', array());
  switch ($action) {
    case 'add':
      $fields[$entity_type][$bundle][$field] = $field;
      break;
    case 'remove':
      unset($fields[$entity_type][$bundle][$field]);
      if (empty($fields[$entity_type][$bundle])) {
        unset($fields[$entity_type][$bundle]);
      }
      if (empty($fields[$entity_type])) {
        unset($fields[$entity_type]);
      }
      break;
  }
  if (empty($fields)) {
    variable_del('sharedcontent_keyword_fields');
  }
  else {
    variable_set('sharedcontent_keyword_fields', $fields);
  }
}
