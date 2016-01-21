<?php
/**
 * @file
 * cod_basic_page.features.field_instance.inc
 */

/**
 * Implements hook_field_default_field_instances().
 */
function cod_basic_page_field_default_field_instances() {
  $field_instances = array();

  // Exported field_instance: 'node-page-og_group_ref'
  $field_instances['node-page-og_group_ref'] = array(
    'bundle' => 'page',
    'default_value' => NULL,
    'default_value_function' => 'entityreference_prepopulate_field_default_value',
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
        'weight' => 1,
      ),
      'listing' => array(
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
    'entity_type' => 'node',
    'field_name' => 'og_group_ref',
    'label' => 'Conference',
    'required' => 0,
    'settings' => array(
      'behaviors' => array(
        'prepopulate' => array(
          'action' => 'hide',
          'action_on_edit' => 1,
          'fallback' => 'redirect',
          'providers' => array(
            'og_context' => 1,
            'url' => 1,
          ),
          'skip_perm' => 'administer nodes',
          'status' => 1,
        ),
      ),
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(),
      'type' => 'options_select',
      'weight' => 3,
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('Conference');

  return $field_instances;
}
