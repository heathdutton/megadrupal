<?php
/**
 * @file
 * makefile.features.field_instance.inc
 */

/**
 * Implements hook_field_default_field_instances().
 */
function makefile_field_default_field_instances() {
  $field_instances = array();

  // Exported field_instance: 'file-drush_make-field_drush_api_version'
  $field_instances['file-drush_make-field_drush_api_version'] = array(
    'bundle' => 'drush_make',
    'default_value' => array(
      0 => array(
        'value' => 2,
      ),
    ),
    'deleted' => 0,
    'description' => 'API version of the make file.',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'number',
        'settings' => array(
          'decimal_separator' => '.',
          'prefix_suffix' => TRUE,
          'scale' => 0,
          'thousand_separator' => ' ',
        ),
        'type' => 'number_integer',
        'weight' => 0,
      ),
      'preview' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'file',
    'field_name' => 'field_drush_api_version',
    'label' => 'Drush Make API Version',
    'required' => 0,
    'settings' => array(
      'max' => '',
      'min' => 1,
      'prefix' => 'api = ',
      'suffix' => '',
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 0,
      'module' => 'number',
      'settings' => array(),
      'type' => 'number',
      'weight' => 3,
    ),
  );

  // Exported field_instance: 'file-drush_make-field_drush_core_api_version'
  $field_instances['file-drush_make-field_drush_core_api_version'] = array(
    'bundle' => 'drush_make',
    'default_value' => array(
      0 => array(
        'value' => '7.x',
      ),
    ),
    'deleted' => 0,
    'description' => 'Drupal Core API Version (i.e. 7.x, 8.x)',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 1,
      ),
      'preview' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'file',
    'field_name' => 'field_drush_core_api_version',
    'label' => 'Drupal Core API Version',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 12,
      ),
      'type' => 'text_textfield',
      'weight' => 4,
    ),
  );

  // Exported field_instance: 'file-drush_make-field_drush_make_name'
  $field_instances['file-drush_make-field_drush_make_name'] = array(
    'bundle' => 'drush_make',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'A human-readable name for this make file.',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'safeword',
        'settings' => array(),
        'type' => 'safeword_human',
        'weight' => 2,
      ),
      'preview' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'file',
    'field_name' => 'field_drush_make_name',
    'label' => 'Name',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 0,
      'module' => 'safeword',
      'settings' => array(),
      'type' => 'safeword_machine_name',
      'weight' => 0,
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('A human-readable name for this make file.');
  t('API version of the make file.');
  t('Drupal Core API Version');
  t('Drupal Core API Version (i.e. 7.x, 8.x)');
  t('Drush Make API Version');
  t('Name');

  return $field_instances;
}
