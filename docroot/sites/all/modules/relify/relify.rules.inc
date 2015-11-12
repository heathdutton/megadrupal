<?php

/**
 * @file
 * Rules integration for the Relify module.
 */


/**
 * Implements hook_rules_action_info().
 */
function relify_rules_action_info() {
  $actions = array();

  // Add an action for each available data set.
  foreach (relify_data_set_load_multiple(relify_client_id()) as $data_set_id => $data_set) {
    $action_name = 'relify_add_' . $data_set_id;

    $actions[$action_name] = array(
      'label' => t('Add data: @name', array('@name' => $data_set['name'])),
      'parameter' => array(),
      'named parameter' => TRUE,
      'group' => t('Relify'),
      'callbacks' => array(
        'execute' => 'relify_rules_create_data',
      ),
      'access callback' => 'relify_rules_create_data_access',
    );

    foreach ($data_set['keys'] as $data_set_key) {
      $actions[$action_name]['parameter'][$data_set_key['name']] = array(
        'type' => ($data_set_key['type'] == 'number') ? 'decimal' : 'text',
        'label' => t('@name (@type)', array('@name' => $data_set_key['name'], '@type' => $data_set_key['type'])),
        'optional' => TRUE,
        'default mode' => 'selector',
      );
    }
  }

  return $actions;
}

/**
 * Determines a user's access to add data to data sets.
 */
function relify_rules_create_data_access() {
  return user_access('administer relify');
}

/**
 * Generic data creation callback for Relify data sets.
 */
function relify_rules_create_data($parameters, $action) {
  // Extract the data set ID from the action name and load the data set.
  $data_set_id = substr($action->getElementName(), 11);
  $data_set = relify_data_set_load($data_set_id);

  // Reduce the parameters array to keys that match data set key names and have
  // non-NULL values.
  $data = array_intersect_key($parameters, $data_set['keys']);
  $data = array_filter($data);

  relify_request_data_create($data_set_id, $data);
}
