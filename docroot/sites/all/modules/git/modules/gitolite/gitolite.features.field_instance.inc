<?php
/**
 * @file
 * gitolite.features.field_instance.inc
 */

/**
 * Implements hook_field_default_field_instances().
 */
function gitolite_field_default_field_instances() {
  $field_instances = array();

  // Exported field_instance: 'git_server-gitolite-field_admin_users'
  $field_instances['git_server-gitolite-field_admin_users'] = array(
    'bundle' => 'gitolite',
    'default_value' => array(
      0 => array(
        'value' => 'admin',
      ),
    ),
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'git_server',
    'field_name' => 'field_admin_users',
    'label' => 'Admin User(s)',
    'required' => 1,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 30,
      ),
      'type' => 'text_textfield',
      'weight' => 1,
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('Admin User(s)');

  return $field_instances;
}
