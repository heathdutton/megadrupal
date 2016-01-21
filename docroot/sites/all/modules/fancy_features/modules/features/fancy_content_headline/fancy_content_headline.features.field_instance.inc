<?php
/**
 * @file
 * fancy_content_headline.features.field_instance.inc
 */

/**
 * Implements hook_field_default_field_instances().
 */
function fancy_content_headline_field_default_field_instances() {
  $field_instances = array();

  // Exported field_instance: 'fancy_content-headline-field_fancy_style'
  $field_instances['fancy_content-headline-field_fancy_style'] = array(
    'bundle' => 'headline',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'module' => 'field_formatter_css_class',
        'settings' => array(
          'depth' => 0,
          'hidden' => 1,
          'target' => 'entity',
        ),
        'type' => 'cssclass_formatter',
        'weight' => 2,
      ),
      'inline_entity_form_preview' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'fancy_content',
    'field_name' => 'field_fancy_style',
    'label' => 'Style',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(),
      'type' => 'options_buttons',
      'weight' => 3,
    ),
  );

  // Exported field_instance: 'fancy_content-headline-field_fancy_textcolor'
  $field_instances['fancy_content-headline-field_fancy_textcolor'] = array(
    'bundle' => 'headline',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'module' => 'css_field_formatters',
        'settings' => array(
          'css' => '.content-[fancy_content:id] {
  color: [field-value:value];
}',
          'css_selector' => 'body',
          'important' => 0,
          'media_query' => 'all',
          'prefix' => '',
          'property' => 'height',
          'suffix' => '',
        ),
        'type' => 'css_property_formatter',
        'weight' => 1,
      ),
      'inline_entity_form_preview' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'fancy_content',
    'field_name' => 'field_fancy_textcolor',
    'label' => 'Textcolor',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 0,
      'module' => 'color_field',
      'settings' => array(),
      'type' => 'color_field_default_widget',
      'weight' => 2,
    ),
  );

  // Exported field_instance: 'fancy_content-headline-field_headline'
  $field_instances['fancy_content-headline-field_headline'] = array(
    'bundle' => 'headline',
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
      'inline_entity_form_preview' => array(
        'label' => 'hidden',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 0,
      ),
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'fancy_content',
    'field_name' => 'field_headline',
    'label' => 'Headline',
    'required' => 1,
    'settings' => array(
      'text_processing' => 1,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'rows' => 2,
      ),
      'type' => 'text_textarea',
      'weight' => 1,
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('Headline');
  t('Style');
  t('Textcolor');

  return $field_instances;
}
