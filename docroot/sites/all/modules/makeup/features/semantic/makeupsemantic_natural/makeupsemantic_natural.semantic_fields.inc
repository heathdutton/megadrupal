<?php
/**
 * @file
 * makeupsemantic_natural.semantic_fields.inc
 */

/**
 * Implements hook_default_semantic_fields_preset().
 */
function makeupsemantic_natural_default_semantic_fields_preset() {
  $export = array();

  $preset = new stdClass();
  $preset->disabled = FALSE; /* Edit this to true to make a default preset disabled initially */
  $preset->api_version = 1;
  $preset->name = 'h1';
  $preset->admin_title = 'H1';
  $preset->description = '';
  $preset->data = array(
    'field_element' => '',
    'field_class' => '',
    'field_prefix' => '',
    'field_suffix' => '',
    'label_element_above' => 'h1',
    'label_class_above' => '',
    'label_suffix_above' => '',
    'label_element_inline' => 'span',
    'label_class_inline' => '',
    'label_suffix_inline' => ':',
    'multiple_items_element' => 'div',
    'multiple_items_class' => '',
    'multiple_item_element' => 'h1',
    'multiple_item_class' => '',
    'multiple_item_separator' => '',
    'use_singlevalue_settings' => 0,
    'single_item_element' => 'h1',
    'single_item_class' => '',
    'last_every_nth' => '0',
    'first_class' => 'first',
    'last_class' => 'last',
    'striping_classes' => 'odd even',
  );
  $export['h1'] = $preset;

  $preset = new stdClass();
  $preset->disabled = FALSE; /* Edit this to true to make a default preset disabled initially */
  $preset->api_version = 1;
  $preset->name = 'h2';
  $preset->admin_title = 'H2';
  $preset->description = '';
  $preset->data = array(
    'field_element' => '',
    'field_class' => '',
    'field_prefix' => '',
    'field_suffix' => '',
    'label_element_above' => 'h2',
    'label_class_above' => '',
    'label_suffix_above' => '',
    'label_element_inline' => 'span',
    'label_class_inline' => '',
    'label_suffix_inline' => ':',
    'multiple_items_element' => 'div',
    'multiple_items_class' => '',
    'multiple_item_element' => 'h2',
    'multiple_item_class' => '',
    'multiple_item_separator' => '',
    'use_singlevalue_settings' => 0,
    'single_item_element' => 'h2',
    'single_item_class' => '',
    'last_every_nth' => '0',
    'first_class' => 'first',
    'last_class' => 'last',
    'striping_classes' => 'odd even',
  );
  $export['h2'] = $preset;

  $preset = new stdClass();
  $preset->disabled = FALSE; /* Edit this to true to make a default preset disabled initially */
  $preset->api_version = 1;
  $preset->name = 'h3';
  $preset->admin_title = 'H3';
  $preset->description = '';
  $preset->data = array(
    'field_element' => '',
    'field_class' => '',
    'field_prefix' => '',
    'field_suffix' => '',
    'label_element_above' => 'h3',
    'label_class_above' => '',
    'label_suffix_above' => '',
    'label_element_inline' => 'span',
    'label_class_inline' => '',
    'label_suffix_inline' => ':',
    'multiple_items_element' => 'div',
    'multiple_items_class' => '',
    'multiple_item_element' => 'h3',
    'multiple_item_class' => '',
    'multiple_item_separator' => '',
    'use_singlevalue_settings' => 0,
    'single_item_element' => 'h3',
    'single_item_class' => '',
    'last_every_nth' => '0',
    'first_class' => 'first',
    'last_class' => 'last',
    'striping_classes' => 'odd even',
  );
  $export['h3'] = $preset;

  $preset = new stdClass();
  $preset->disabled = FALSE; /* Edit this to true to make a default preset disabled initially */
  $preset->api_version = 1;
  $preset->name = 'h4';
  $preset->admin_title = 'H4';
  $preset->description = '';
  $preset->data = array(
    'field_element' => '',
    'field_class' => '',
    'field_prefix' => '',
    'field_suffix' => '',
    'label_element_above' => 'h4',
    'label_class_above' => '',
    'label_suffix_above' => '',
    'label_element_inline' => 'span',
    'label_class_inline' => '',
    'label_suffix_inline' => ':',
    'multiple_items_element' => 'div',
    'multiple_items_class' => '',
    'multiple_item_element' => 'h4',
    'multiple_item_class' => '',
    'multiple_item_separator' => '',
    'use_singlevalue_settings' => 0,
    'single_item_element' => 'h4',
    'single_item_class' => '',
    'last_every_nth' => '0',
    'first_class' => 'first',
    'last_class' => 'last',
    'striping_classes' => 'odd even',
  );
  $export['h4'] = $preset;

  $preset = new stdClass();
  $preset->disabled = FALSE; /* Edit this to true to make a default preset disabled initially */
  $preset->api_version = 1;
  $preset->name = 'h5';
  $preset->admin_title = 'H5';
  $preset->description = '';
  $preset->data = array(
    'field_element' => '',
    'field_class' => '',
    'field_prefix' => '',
    'field_suffix' => '',
    'label_element_above' => 'h5',
    'label_class_above' => '',
    'label_suffix_above' => '',
    'label_element_inline' => 'span',
    'label_class_inline' => '',
    'label_suffix_inline' => ':',
    'multiple_items_element' => 'div',
    'multiple_items_class' => '',
    'multiple_item_element' => 'H5',
    'multiple_item_class' => '',
    'multiple_item_separator' => '',
    'use_singlevalue_settings' => 0,
    'single_item_element' => 'h5',
    'single_item_class' => '',
    'last_every_nth' => '0',
    'first_class' => 'first',
    'last_class' => 'last',
    'striping_classes' => 'odd even',
  );
  $export['h5'] = $preset;

  $preset = new stdClass();
  $preset->disabled = FALSE; /* Edit this to true to make a default preset disabled initially */
  $preset->api_version = 1;
  $preset->name = 'h6';
  $preset->admin_title = 'H6';
  $preset->description = '';
  $preset->data = array(
    'field_element' => '',
    'field_class' => '',
    'field_prefix' => '',
    'field_suffix' => '',
    'label_element_above' => 'h6',
    'label_class_above' => '',
    'label_suffix_above' => '',
    'label_element_inline' => 'span',
    'label_class_inline' => '',
    'label_suffix_inline' => ':',
    'multiple_items_element' => 'div',
    'multiple_items_class' => '',
    'multiple_item_element' => 'h6',
    'multiple_item_class' => '',
    'multiple_item_separator' => '',
    'use_singlevalue_settings' => 0,
    'single_item_element' => 'h6',
    'single_item_class' => '',
    'last_every_nth' => '0',
    'first_class' => 'first',
    'last_class' => 'last',
    'striping_classes' => 'odd even',
  );
  $export['h6'] = $preset;

  $preset = new stdClass();
  $preset->disabled = FALSE; /* Edit this to true to make a default preset disabled initially */
  $preset->api_version = 1;
  $preset->name = 'h7';
  $preset->admin_title = 'H7';
  $preset->description = '';
  $preset->data = array(
    'field_element' => '',
    'field_class' => '',
    'field_prefix' => '',
    'field_suffix' => '',
    'label_element_above' => 'h7',
    'label_class_above' => '',
    'label_suffix_above' => '',
    'label_element_inline' => 'span',
    'label_class_inline' => '',
    'label_suffix_inline' => ':',
    'multiple_items_element' => 'div',
    'multiple_items_class' => '',
    'multiple_item_element' => 'h7',
    'multiple_item_class' => '',
    'multiple_item_separator' => '',
    'use_singlevalue_settings' => 0,
    'single_item_element' => 'h7',
    'single_item_class' => '',
    'last_every_nth' => '0',
    'first_class' => 'first',
    'last_class' => 'last',
    'striping_classes' => 'odd even',
  );
  $export['h7'] = $preset;

  $preset = new stdClass();
  $preset->disabled = FALSE; /* Edit this to true to make a default preset disabled initially */
  $preset->api_version = 1;
  $preset->name = 'none';
  $preset->admin_title = 'None';
  $preset->description = 'The field, and nothing else.';
  $preset->data = array(
    'field_element' => '',
    'field_class' => '',
    'field_prefix' => '',
    'field_suffix' => '',
    'label_element_above' => 'div',
    'label_class_above' => '',
    'label_suffix_above' => '',
    'label_element_inline' => 'span',
    'label_class_inline' => '',
    'label_suffix_inline' => ':',
    'multiple_items_element' => '',
    'multiple_items_class' => '',
    'multiple_item_element' => '',
    'multiple_item_class' => '',
    'multiple_item_separator' => '',
    'use_singlevalue_settings' => 0,
    'single_item_element' => '',
    'single_item_class' => '',
    'last_every_nth' => '0',
    'first_class' => 'first',
    'last_class' => 'last',
    'striping_classes' => 'odd even',
  );
  $export['none'] = $preset;

  return $export;
}
