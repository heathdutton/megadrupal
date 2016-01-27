<?php
/**
 * @file
 * Setting and administration functions for node_adoption.
 */

/**
 * Configure Node Adoption with these settings.
 */
function node_adoption_settings() {

  $name = db_query(
    'SELECT name FROM {users} WHERE uid = :uid',
    array(':uid' => variable_get('node_adoption_parent', 1))
  )
  ->fetchField();

  $form['node_adoption_parent'] = array(
    '#autocomplete_path'  => 'user/autocomplete',
    '#default_value'      => !empty($name) ? $name : '',
    '#title'              => t('Specify who will adopt nodes created by a deleted user.  Defaults to <em>Anonymous</em>.'),
    '#type'               => 'textfield',
  );

  $form = system_settings_form($form); // convert name to uid.
  $form['#submit'][] = 'node_adoption_settings_form_submit';
  return $form;
}

/**
 * #submit callback for node_adoption_settings().
 */
function node_adoption_settings_form_submit($form, &$form_state) {
  $uid = db_query(
    'SELECT uid FROM {users} WHERE name = :name',
    array(':name' => $form_state['values']['node_adoption_parent'])
  )
  ->fetchField();
  variable_set('node_adoption_parent', $uid);
}

