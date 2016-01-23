<?php

/**
 * @file
 * List Transitions hook implementations.
 */

/**
 * Implements hook_list_transitions_criterion().
 */
function list_transitions_list_transitions_criterion($instance) {
  $criterion = array();

  // Adds the author role.
  if ('node' == $instance['entity_type']) {
    $criterion['node:author'] = array(
      'label' => t('user is node author'),
      'group' => t('Node context'),
      'criteria callback' => 'list_transitions_criteria_author',
    );
  }

  // Adds the roles to the criterion.
  foreach (user_roles() as $role) {
    $criterion['role:' . $role] = array(
      'label' => $role,
      'group' => t('User role'),
      'criteria callback' => 'list_transitions_criteria_role',
      'criteria arguments' => array($role),
    );
  }

  return $criterion;
}

/**
 * Implements hook_list_transitions_criterion().
 *
 * Implemented on behalf of the User Reference module.
 */
function user_reference_list_transitions_criterion($instance) {
  $criterion = array();

  // Gets all instances of user_reference fields.
  $types = array('user_reference');
  $instances = list_transitions_get_instances_by_type($instance['entity_type'], $instance['bundle'], $types);
  if ($instances) {
    $criterion['user_reference'] = array(
      'label' => t('user is referenced in field'),
      'group' => t('User reference'),
      'criteria callback' => 'list_transitions_criteria_user_reference',
      'settings callback' => 'list_transitions_settings_user_reference',
      'settings arguments' => array($instances),
    );
  }

  return $criterion;
}

/**
 * Checks whether the entity is authored by the current user.
 */
function list_transitions_criteria_author($entity, $instance, $settings) {
  global $user;
  return (1 == $user->uid || $entity->uid == $user->uid);
}

/**
 * Checks whether the user belongs to the passed role.
 */
function list_transitions_criteria_role($entity, $instance, $settings, $role) {
  global $user;
  $roles = array_flip($user->roles);
  return (1 == $user->uid || isset($roles[$role]));
}

/**
 * Checks whether the user is referenced.
 */
function list_transitions_criteria_user_reference($entity, $instance, $settings) {
  global $user;
  $settings += array('user_reference_fields' => array());
  foreach ($settings['user_reference_fields'] as $field_name) {
    $items = field_get_items($instance['entity_type'], $entity, $field_name);
    foreach ($items as $item) {
      if ($item['uid'] == $user->uid) {
        return TRUE;
      }
    }
  }
}

/**
 * Settings form for the user reference criteria.
 */
function list_transitions_settings_user_reference($instance, $settings, $instances) {
  $form = array();

  $options = array();
  foreach ($instances as $field_name => $reference_instance) {
    $options[$field_name] = $reference_instance['label'];
  }

  $settings += array('user_reference_fields' => array());
  $form['user_reference_fields'] = array(
    '#type' => 'select',
    '#title' => t('User reference fields'),
    '#options' => $options,
    '#multiple' => TRUE,
    '#default_value' => $settings['user_reference_fields'],
  );

  return $form;
}

/**
 * Gets fields that are
 *
 * @param $entity_type
 *   The entity type for which to return fields.
 * @param $bundle_name
 *   The bundle name for which to return fields.
 * @param array $types
 *   An array of field types.
 *
 * @return array
 *   An array of field definitions.
 *
 * @see field_info_field()
 */
function list_transitions_get_instances_by_type($entity_type, $bundle_name, array $types) {
  $return = array();
  $types = drupal_map_assoc($types);
  $instances = field_info_instances($entity_type, $bundle_name);
  foreach ($instances as $field_name => $instance) {
    $field = field_info_field($instance['field_name']);
    if ('user_reference' == $field['type']) {
      $return[$field_name] = $instance;
    }
  }
  return $return;
}
