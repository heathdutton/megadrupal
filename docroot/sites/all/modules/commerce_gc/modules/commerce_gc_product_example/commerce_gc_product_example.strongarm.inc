<?php
/**
 * @file
 * commerce_gc_product_example.strongarm.inc
 */

/**
 * Implements hook_strongarm().
 */
function commerce_gc_product_example_strongarm() {
  $export = array();

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_anonymous_giftcard_product_display';
  $strongarm->value = 0;
  $export['comment_anonymous_giftcard_product_display'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_mode_giftcard_product_display';
  $strongarm->value = 1;
  $export['comment_default_mode_giftcard_product_display'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_per_page_giftcard_product_display';
  $strongarm->value = '50';
  $export['comment_default_per_page_giftcard_product_display'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_form_location_giftcard_product_display';
  $strongarm->value = 1;
  $export['comment_form_location_giftcard_product_display'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_giftcard_product_display';
  $strongarm->value = '1';
  $export['comment_giftcard_product_display'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_preview_giftcard_product_display';
  $strongarm->value = '1';
  $export['comment_preview_giftcard_product_display'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_subject_field_giftcard_product_display';
  $strongarm->value = 1;
  $export['comment_subject_field_giftcard_product_display'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'field_bundle_settings_node__giftcard_product_display';
  $strongarm->value = array(
    'view_modes' => array(),
    'extra_fields' => array(
      'form' => array(),
      'display' => array(
        'product:sku' => array(
          'teaser' => array(
            'weight' => '4',
            'visible' => FALSE,
          ),
        ),
        'product:title' => array(
          'teaser' => array(
            'weight' => '5',
            'visible' => FALSE,
          ),
        ),
        'product:status' => array(
          'teaser' => array(
            'weight' => '6',
            'visible' => FALSE,
          ),
        ),
        'product:commerce_gc_value' => array(
          'teaser' => array(
            'weight' => '2',
            'visible' => TRUE,
          ),
        ),
        'product:commerce_price' => array(
          'teaser' => array(
            'weight' => '1',
            'visible' => TRUE,
          ),
        ),
      ),
    ),
  );
  $export['field_bundle_settings_node__giftcard_product_display'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_options_giftcard_product_display';
  $strongarm->value = array(
    0 => 'main-menu',
  );
  $export['menu_options_giftcard_product_display'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_parent_giftcard_product_display';
  $strongarm->value = 'main-menu:0';
  $export['menu_parent_giftcard_product_display'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_options_giftcard_product_display';
  $strongarm->value = array(
    0 => 'status',
    1 => 'promote',
  );
  $export['node_options_giftcard_product_display'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_preview_giftcard_product_display';
  $strongarm->value = '1';
  $export['node_preview_giftcard_product_display'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_submitted_giftcard_product_display';
  $strongarm->value = 0;
  $export['node_submitted_giftcard_product_display'] = $strongarm;

  return $export;
}
