<?php
/**
 * @file
 * workbench_moderation_profile.features.field_instance.inc
 */

/**
 * Implements hook_field_default_field_instances().
 */
function workbench_moderation_profile_field_default_field_instances() {
  $field_instances = array();

  // Exported field_instance:
  // workbench_moderation_profile-workbench_moderation_profile-field_transitions
  $field_instances['workbench_moderation_profile-workbench_moderation_profile-field_transitions'] = array(
    'bundle' => 'workbench_moderation_profile',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'entityreference',
        'settings' => array(
          'link' => FALSE,
        ),
        'type' => 'entityreference_label',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'workbench_moderation_profile',
    'field_name' => 'field_transitions',
    'label' => 'Transitions',
    'required' => 1,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(),
      'type' => 'options_select',
      'weight' => 1,
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('Transitions');

  return $field_instances;
}
