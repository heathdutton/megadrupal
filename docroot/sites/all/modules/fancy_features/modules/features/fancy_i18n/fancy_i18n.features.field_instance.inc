<?php
/**
 * @file
 * fancy_i18n.features.field_instance.inc
 */

/**
 * Implements hook_field_default_field_instances().
 */
function fancy_i18n_field_default_field_instances() {
  $field_instances = array();

  // Exported field_instance: 'content_i18n-text_i18n-field_fancy_text_i18n'
  $field_instances['content_i18n-text_i18n-field_fancy_text_i18n'] = array(
    'bundle' => 'text_i18n',
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
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'content_i18n',
    'field_name' => 'field_fancy_text_i18n',
    'label' => 'Text i18n',
    'required' => 1,
    'settings' => array(
      'entity_translation_sync' => FALSE,
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'rows' => 5,
      ),
      'type' => 'text_textarea',
      'weight' => 1,
    ),
  );

  // Exported field_instance: 'fancy_section-responsive_i18n-field_fancy_rel_content_i18n'
  $field_instances['fancy_section-responsive_i18n-field_fancy_rel_content_i18n'] = array(
    'bundle' => 'responsive_i18n',
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
      'view_mode_selector' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'fancy_section',
    'field_name' => 'field_fancy_rel_content_i18n',
    'label' => 'Content i18n',
    'required' => FALSE,
    'settings' => array(
      'entity_translation_sync' => FALSE,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'module' => 'inline_entity_form',
      'settings' => array(
        'fields' => array(),
        'preview' => array(
          'enable' => 0,
          'fallback_view_mode' => 'default',
          'view_mode' => 'inline_entity_form_preview',
        ),
        'type_settings' => array(),
      ),
      'type' => 'inline_entity_form',
      'weight' => 1,
    ),
  );

  // Exported field_instance: 'node-fancy_page_i18n-field_fancy_headline'
  $field_instances['node-fancy_page_i18n-field_fancy_headline'] = array(
    'bundle' => 'fancy_page_i18n',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'Translatable title of the node.',
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
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
    'field_name' => 'field_fancy_headline',
    'label' => 'Headline',
    'required' => 1,
    'settings' => array(
      'entity_translation_sync' => FALSE,
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 1,
    ),
  );

  // Exported field_instance: 'node-fancy_page_i18n-field_fancy_rel_sections_i18n'
  $field_instances['node-fancy_page_i18n-field_fancy_rel_sections_i18n'] = array(
    'bundle' => 'fancy_page_i18n',
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
        'weight' => 1,
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
    'field_name' => 'field_fancy_rel_sections_i18n',
    'label' => 'Sections',
    'required' => 0,
    'settings' => array(
      'entity_translation_sync' => FALSE,
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
          'view_mode' => 'view_mode_selector',
        ),
        'type_settings' => array(
          'allow_existing' => 0,
          'clone_button' => 0,
          'delete_references' => 0,
          'label_plural' => 'nodes',
          'label_singular' => 'node',
          'match_operator' => 'CONTAINS',
          'override_labels' => 0,
        ),
      ),
      'type' => 'inline_entity_form',
      'weight' => 5,
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('Content i18n');
  t('Headline');
  t('Sections');
  t('Text i18n');
  t('Translatable title of the node.');

  return $field_instances;
}
