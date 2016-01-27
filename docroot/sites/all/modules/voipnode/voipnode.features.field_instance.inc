<?php
/**
 * @file
 * voipnode.features.field_instance.inc
 */

/**
 * Implements hook_field_default_field_instances().
 */
function voipnode_field_default_field_instances() {
  $field_instances = array();

  // Exported field_instance: 'node-voipnode-body'
  $field_instances['node-voipnode-body'] = array(
    'bundle' => 'voipnode',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'hidden',
        'module' => 'text',
        'settings' => array(
          'trim_length' => 600,
        ),
        'type' => 'text_summary_or_trimmed',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'body',
    'label' => 'Body',
    'required' => FALSE,
    'settings' => array(
      'display_summary' => TRUE,
      'text_processing' => 1,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'module' => 'text',
      'settings' => array(
        'rows' => 20,
        'summary_rows' => 5,
      ),
      'type' => 'text_textarea_with_summary',
      'weight' => 1,
    ),
  );

  // Exported field_instance: 'node-voipnode-field_audio'
  $field_instances['node-voipnode-field_voipextension_audio'] = array(
    'bundle' => 'voipnode',
    'deleted' => 0,
    'description' => 'Audio file to be played in place of reading out the text of the body.',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'file',
        'settings' => array(),
        'type' => 'file_default',
        'weight' => 2,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_voipextension_audio',
    'label' => 'Audio',
    'required' => 0,
    'settings' => array(
      'description_field' => 0,
      'file_directory' => 'voipnode',
      'file_extensions' => 'mp3 wav',
      'max_filesize' => '',
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'file',
      'settings' => array(
        'progress_indicator' => 'throbber',
      ),
      'type' => 'file_generic',
      'weight' => 2,
    ),
  );

  // Exported field_instance: 'node-voipnode-field_extension'
  $field_instances['node-voipnode-field_voipextension'] = array(
    'bundle' => 'voipnode',
    'default_value' => array(
      0 => array(
        'value' => '',
        'enabled' => 1,
        'script' => 'voipnode_basic_script',
      ),
    ),
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'voipextension',
        'settings' => array(
          'prefix_suffix' => TRUE,
          'status_display' => 0,
        ),
        'type' => 'voipextension_default',
        'weight' => 3,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_voipextension',
    'label' => 'Extension',
    'required' => 0,
    'settings' => array(
      'max' => 5000,
      'min' => 1000,
      'prefix' => '',
      'script' => '',
      'script_selectable' => 0,
      'suffix' => '',
      'toggle_enabled' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 0,
      'module' => 'voipextension',
      'settings' => array(),
      'type' => 'voipextension',
      'weight' => 3,
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('Audio');
  t('Audio file to be played in place of reading out the text of the body.');
  t('Body');
  t('Extension');

  return $field_instances;
}
