<?php
/**
 * @file
 * fancy_simple_page.features.field_instance.inc
 */

/**
 * Implements hook_field_default_field_instances().
 */
function fancy_simple_page_field_default_field_instances() {
  $field_instances = array();

  // Exported field_instance: 'node-fancy_simple_page-field_fancy_rel_contents'
  $field_instances['node-fancy_simple_page-field_fancy_rel_contents'] = array(
    'bundle' => 'fancy_simple_page',
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'module' => 'entityreference',
        'settings' => array(
          'links' => 0,
          'view_mode' => 'default',
        ),
        'type' => 'entityreference_entity_view',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'node',
    'field_name' => 'field_fancy_rel_contents',
    'label' => 'Content',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'inline_entity_form',
      'settings' => array(
        'fields' => array(),
        'preview' => array(
          'enable' => 1,
          'fallback_view_mode' => 'default',
          'view_mode' => 'inline_entity_form_preview',
        ),
        'type_settings' => array(
          'allow_existing' => 0,
          'clone_button' => 1,
          'delete_references' => 0,
          'label_plural' => 'contents',
          'label_singular' => 'content',
          'match_operator' => 'CONTAINS',
          'override_labels' => 1,
        ),
      ),
      'type' => 'inline_entity_form',
      'weight' => 32,
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('Content');

  return $field_instances;
}
