<?php

/**
 * @file
 * Provides VBO action to grant access to nodes for user roles.
 */

/**
 * Implements hook_action_info().
 */
function aul_roles_action_info() {
  return array(
    'aul_roles_add_grants_action' => array(
      'type' => 'node',
      'label' => t('Add grants for role'),
      'behavior' => array('changes_property'),
      'configurable' => TRUE,
      'vbo_configurable' => TRUE,
      'triggers' => array('any'),
    ),
    'aul_roles_remove_grants_action' => array(
      'type' => 'node',
      'label' => t('Remove grants for role'),
      'behavior' => array('changes_property'),
      'configurable' => FALSE,
      'vbo_configurable' => TRUE,
      'triggers' => array('any'),
    ),
  );
}

/**
 * Actions callback. Add grants to node for special users roles.
 */
function aul_roles_add_grants_action(&$node, $context) {
  
  $aul_name = AUL_DEFAULT_SOURCE;
  
  // Get users list.
  $role_names = array();
  $role_values = $context['roles'];
  
  foreach ($role_values as $rid) {
    if (!$rid || !$role = user_role_load($rid)) {
      continue;
    }
    $role_names[] = $role->name;
    
    // Add grants.
    aul_add_aul($role->rid, $node->nid, $context['grants'], $aul_name, 'rid');
  }
  
  // Collect users names for message.
  $grant_names = array();
  foreach ($context['grants'] as $grant_name) {
    if($grant_name) {
      $grant_names[] = $grant_name;
    }
  }

  // Rebuild node grants.
  node_access_acquire_grants($node);

  // Show success message.
  if(count($role_names) == 1) {
    $text = '%roles has got permission to %grants "%title".';
  }
  else {
    $text = '%roles have got permission to %grants "%title".';
  }
  
  $message = t($text, array(
    '%roles' => implode(', ', $role_names),
    '%title' => $node->title,
    '%grants' => implode(', ', $grant_names),
  ));

  drupal_set_message($message);
}


/**
 * Actions callback. Remove grants to node for special users list.
 */
function aul_roles_remove_grants_action(&$node, $context) {
  
  $aul_name = AUL_DEFAULT_SOURCE;
  
  // Get users list.
  $role_names = array();
  $role_values = $context['roles'];
  
  foreach ($role_values as $rid) {
    if (!$rid || !$role = user_role_load($rid)) {
      continue;
    }
    $role_names[] = $role->name;
    
    // Remove grants.
    aul_remove_aul($role->rid, $node->nid, $context['grants'], $aul_name, 'rid');
  }
  
  // Collect users names for message.
  $grant_names = array();
  foreach ($context['grants'] as $grant_name) {
    if($grant_name) {
      $grant_names[] = $grant_name;
    }
  }

  // Rebuild node grants.
  node_access_acquire_grants($node);

  // Show success message.
  $message = t('Access to %grants "%title" has been successfully removed for %roles.', array(
    '%roles' => implode(', ', $role_names),
    '%title' => $node->title,
    '%grants' => implode(', ', $grant_names),
  ));

  drupal_set_message($message);
}
 
/**
 * Settings form for AUL VBO action.
 */
function aul_roles_remove_grants_action_views_bulk_operations_form($settings, $entityType, $settings_dom_id) {

  return _aul_roles_grants_action_views_bulk_operations_form($settings, $entityType, $settings_dom_id);
}

/**
 * Builds form for removing grants to node for special users roles.
 */
function aul_roles_remove_grants_action_form($settings) {

  return _aul_roles_grants_action_form($settings);
}

/**
 * Form callback for removing grants to node for special users roles.
 */
function aul_roles_remove_grants_action_submit($form, $form_state) {
  
  return _aul_roles_grants_action_submit($form, $form_state);
}

/**
 * Settings form for AUL VBO action.
 */
function aul_roles_add_grants_action_views_bulk_operations_form($settings, $entityType, $settings_dom_id) {

  return _aul_roles_grants_action_views_bulk_operations_form($settings, $entityType, $settings_dom_id);
}

/**
 * Builds form for adding grants to node for special users list.
 */
function aul_roles_add_grants_action_form($settings) {

  return _aul_roles_grants_action_form($settings);
}

/**
 * Form callback for adding grants to node for special users roles.
 */
function aul_roles_add_grants_action_submit($form, $form_state) {
  
  return _aul_roles_grants_action_submit($form, $form_state);
}

/**
 * Helper function. Builds form for grants to node for special roles.
 */
function _aul_roles_grants_action_form($settings) {

  $form['grants'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Permissions'),
    '#options' => array(
      'view' => t('View'),
      'update' => t('Update'),
      'delete' => t('Delete'),
    ),
    '#required' => TRUE,
    '#default_value' => isset($settings['settings']['grants']) ? $settings['settings']['grants'] : array(),
  );
  
  $roles = user_roles();
  
  $form['roles'] = array(
    '#title' => t('Roles'),
    '#type' => 'checkboxes',
    '#options' => $roles,
    '#required' => TRUE,
    '#default_value' => isset($settings['settings']['roles']) ? $settings['settings']['roles'] : array(),
  );

  return $form;
}

/**
 * Helper function. Builds settings form for AUL VBO action.
 */
function _aul_roles_grants_action_views_bulk_operations_form($settings, $entityType, $settings_dom_id) {

  $form['aul_name'] = array(
    '#title' => t('AUL module'),
    '#type' => 'textfield',
    '#required' => TRUE,
    '#default_value' => !empty($settings['aul_name']) ? $settings['aul_name'] : AUL_DEFAULT_SOURCE,
    '#description' => t('Name of AUL group. Default value is "@name"', array(
      '@name' => AUL_DEFAULT_SOURCE,
    )),
  );
  return $form;
}

/**
 * Helper form callback for adding grants to node for special users roles.
 */
function _aul_roles_grants_action_submit($form, $form_state) {
  $return = array();
  $return['grants'] = $form_state['values']['grants'];
  $return['roles'] = $form_state['values']['roles'];
  return $return;
}
