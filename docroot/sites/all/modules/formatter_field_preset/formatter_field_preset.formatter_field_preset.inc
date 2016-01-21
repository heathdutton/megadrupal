<?php

/**
 * @file
 * Default presets for the module.
 */

/**
 * Implements hook_formatter_field_preset_settings_info().
 *
 * @return array
 *   Array with default exportable objects.
 */
function formatter_field_preset_formatter_field_preset_settings_info() {
  $export = array();

  // Create first example exportable object.
  $formatter_field_preset = new stdClass();
  $formatter_field_preset->api_version = 1;
  $formatter_field_preset->machine_name = 'example';
  $formatter_field_preset->name = 'Example object';
  $formatter_field_preset->field = 'comment_body';
  $formatter_field_preset->format = 'text_trimmed';
  $formatter_field_preset->settings = array(
    'trim_length' => "300",
  );
  $export['example'] = $formatter_field_preset;

  // Create second example exportable object.
  $formatter_field_preset = new stdClass();
  $formatter_field_preset->api_version = 1;
  $formatter_field_preset->machine_name = 'one_more_example';
  $formatter_field_preset->name = 'One more example';
  $formatter_field_preset->field = 'field_image';
  $formatter_field_preset->format = 'image';
  $formatter_field_preset->settings = array(
    'image_style' => 'thumbnail',
    'image_link' => '',
  );
  $export['one_more_example'] = $formatter_field_preset;

  // Return array with default exportable objects.
  return $export;
}
