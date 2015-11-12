<?php
/**
 * @file
 * fillpdf_ubercart_starter.features.field_instance.inc
 */

/**
 * Implements hook_field_default_field_instances().
 */
function fillpdf_ubercart_starter_field_default_field_instances() {
  $field_instances = array();

  // Exported field_instance: 'node-product-field_fillpdf_fid'
  $field_instances['node-product-field_fillpdf_fid'] = array(
    'bundle' => 'product',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'settings' => array(),
        'type' => 'hidden',
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
    'field_name' => 'field_fillpdf_fid',
    'label' => 'FillPDF form ID',
    'required' => 0,
    'settings' => array(
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
      'weight' => -2,
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('FillPDF form ID');

  return $field_instances;
}
