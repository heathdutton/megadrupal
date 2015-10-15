<?php
/**
 * @file
 * amazon_examples.strongarm.inc
 */

/**
 * Implementation of hook_strongarm().
 */
function amazon_examples_strongarm() {
  $export = array();

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_amazon_example';
  $strongarm->value = '0';
  $export['comment_amazon_example'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_anonymous_amazon_example';
  $strongarm->value = 0;
  $export['comment_anonymous_amazon_example'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_mode_amazon_example';
  $strongarm->value = 1;
  $export['comment_default_mode_amazon_example'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_per_page_amazon_example';
  $strongarm->value = '50';
  $export['comment_default_per_page_amazon_example'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_form_location_amazon_example';
  $strongarm->value = 1;
  $export['comment_form_location_amazon_example'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_preview_amazon_example';
  $strongarm->value = '1';
  $export['comment_preview_amazon_example'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_subject_field_amazon_example';
  $strongarm->value = 1;
  $export['comment_subject_field_amazon_example'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_options_amazon_example';
  $strongarm->value = array(
    0 => 'status',
    1 => 'promote',
  );
  $export['node_options_amazon_example'] = $strongarm;

  return $export;
}
