<?php
/**
 * @file
 * ns_form.features.field.inc
 */

/**
 * Implements hook_defaultconfig_field_default_fields().
 */
function ns_form_defaultconfig_field_default_fields() {
  $fields = array();

  // Exported field: 'node-ns_article-field_ns_form_ns_article_form'
  $fields['node-ns_article-field_ns_form_ns_article_form'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '-1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_ns_form_ns_article_form',
      'foreign keys' => array(),
      'indexes' => array(
        'target_entity' => array(
          0 => 'target_id',
        ),
      ),
      'module' => 'entityreference',
      'settings' => array(
        'handler' => 'base',
        'handler_settings' => array(
          'target_bundles' => array(
            'ns_form' => 'ns_form',
          ),
        ),
        'handler_submit' => 'Change handler',
        'target_type' => 'node',
      ),
      'translatable' => '1',
      'type' => 'entityreference',
    ),
    'field_instance' => array(
      'bundle' => 'ns_article',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'entityreference',
          'settings' => array(
            'link' => FALSE,
          ),
          'type' => 'entityreference_label',
          'weight' => 16,
        ),
        'search_index' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
        'search_result' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => '0',
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_ns_form_ns_article_form',
      'label' => 'Forms',
      'required' => 0,
      'settings' => array(
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'entityreference',
        'settings' => array(
          'match_operator' => 'CONTAINS',
          'path' => '',
          'references_dialog_add' => 1,
          'references_dialog_edit' => 1,
          'references_dialog_search' => 1,
          'references_dialog_search_view' => '',
          'size' => '60',
        ),
        'type' => 'entityreference_autocomplete',
        'weight' => '12',
      ),
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('Forms');

  return $fields;
}
