<?php
/**
 * @file
 * panopoly_seo.features.field_instance.inc
 */

/**
 * Implements hook_field_default_field_instances().
 */
function panopoly_seo_field_default_field_instances() {
  $field_instances = array();

  // Exported field_instance: 'user-user-field_google_url'
  $field_instances['user-user-field_google_url'] = array(
    'bundle' => 'user',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'Place your Google+ URL here to provide content authorship for content added to the site. Learn more: <a href="http://www.google.com/insidesearch/features/authorship/">http://www.google.com/insidesearch/features/authorship/</a>',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 3,
      ),
      'featured' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'user',
    'field_name' => 'field_google_url',
    'label' => 'Google+ URL',
    'required' => 0,
    'settings' => array(
      'linkit' => array(
        'enable' => 0,
        'insert_plugin' => '',
      ),
      'text_processing' => 0,
      'user_register_form' => 0,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 41,
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('Google+ URL');
  t('Place your Google+ URL here to provide content authorship for content added to the site. Learn more: <a href="http://www.google.com/insidesearch/features/authorship/">http://www.google.com/insidesearch/features/authorship/</a>');

  return $field_instances;
}
