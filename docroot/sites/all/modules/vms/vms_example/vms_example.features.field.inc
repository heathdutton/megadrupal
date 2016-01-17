<?php
/**
 * @file
 * vms_example.features.field.inc
 */

/**
 * Implements hook_field_default_fields().
 */
function vms_example_field_default_fields() {
  $fields = array();

  // Exported field: 'node-sidebar_message-field_sidebar_message'
  $fields['node-sidebar_message-field_sidebar_message'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_sidebar_message',
      'foreign keys' => array(
        'format' => array(
          'columns' => array(
            'format' => 'format',
          ),
          'table' => 'filter_format',
        ),
      ),
      'indexes' => array(
        'format' => array(
          0 => 'format',
        ),
      ),
      'module' => 'text',
      'settings' => array(),
      'translatable' => '0',
      'type' => 'text_long',
    ),
    'field_instance' => array(
      'bundle' => 'sidebar_message',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'Text entered here will be shown in the sidebar, when the menu items selected below are active.',
      'display' => array(
        'default' => array(
          'label' => 'hidden',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => '0',
        ),
        'teaser' => array(
          'label' => 'hidden',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => '0',
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_sidebar_message',
      'label' => 'Sidebar message',
      'required' => 1,
      'settings' => array(
        'text_processing' => '1',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'rows' => '5',
        ),
        'type' => 'text_textarea',
        'weight' => '-4',
      ),
    ),
  );

  // Exported field: 'node-sidebar_message-field_sidebar_mlid_direct'
  $fields['node-sidebar_message-field_sidebar_mlid_direct'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '-1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_sidebar_mlid_direct',
      'foreign keys' => array(),
      'indexes' => array(),
      'module' => 'number',
      'settings' => array(),
      'translatable' => '0',
      'type' => 'number_integer',
    ),
    'field_instance' => array(
      'bundle' => 'sidebar_message',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'The sidebar message will be shown when viewing the menu items selected here.',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => '1',
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => '2',
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_sidebar_mlid_direct',
      'label' => 'Show on these pages',
      'required' => 0,
      'settings' => array(
        'max' => '',
        'min' => '',
        'prefix' => '',
        'suffix' => '',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'mlidselector',
        'settings' => array(
          'menus' => array(
            'devel' => 0,
            'main-menu' => 'main-menu',
            'management' => 0,
            'navigation' => 'navigation',
            'user-menu' => 0,
          ),
          'show_front' => 0,
        ),
        'type' => 'field_mlid_reference',
        'weight' => '-3',
      ),
    ),
  );

  // Exported field: 'node-sidebar_message-field_sidebar_mlid_sub'
  $fields['node-sidebar_message-field_sidebar_mlid_sub'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '-1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_sidebar_mlid_sub',
      'foreign keys' => array(),
      'indexes' => array(),
      'module' => 'number',
      'settings' => array(),
      'translatable' => '0',
      'type' => 'number_integer',
    ),
    'field_instance' => array(
      'bundle' => 'sidebar_message',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'If the selected menu items are in the active trail, this sidebar message will be shown.',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => '2',
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => '1',
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_sidebar_mlid_sub',
      'label' => 'Show below these pages',
      'required' => 0,
      'settings' => array(
        'max' => '',
        'min' => '',
        'prefix' => '',
        'suffix' => '',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'mlidselector',
        'settings' => array(
          'menus' => array(
            'devel' => 0,
            'main-menu' => 'main-menu',
            'management' => 0,
            'navigation' => 'navigation',
            'user-menu' => 0,
          ),
          'show_front' => 0,
        ),
        'type' => 'field_mlid_reference',
        'weight' => '-2',
      ),
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('If the selected menu items are in the active trail, this sidebar message will be shown.');
  t('Show below these pages');
  t('Show on these pages');
  t('Sidebar message');
  t('Text entered here will be shown in the sidebar, when the menu items selected below are active.');
  t('The sidebar message will be shown when viewing the menu items selected here.');

  return $fields;
}
