<?php
/**
 * @file
 * fancy_content_headline.strongarm.inc
 */

/**
 * Implements hook_strongarm().
 */
function fancy_content_headline_strongarm() {
  $export = array();

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'auto_entitylabel_fancy_content_headline';
  $strongarm->value = '1';
  $export['auto_entitylabel_fancy_content_headline'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'auto_entitylabel_pattern_fancy_content_headline';
  $strongarm->value = 'Headline created by [current-user:name] at [current-user:created:short]';
  $export['auto_entitylabel_pattern_fancy_content_headline'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'field_bundle_settings_fancy_content__headline';
  $strongarm->value = array(
    'view_modes' => array(
      'inline_entity_form_preview' => array(
        'custom_settings' => TRUE,
      ),
      'teaser' => array(
        'custom_settings' => FALSE,
      ),
      'token' => array(
        'custom_settings' => FALSE,
      ),
    ),
    'extra_fields' => array(
      'form' => array(),
      'display' => array(),
    ),
  );
  $export['field_bundle_settings_fancy_content__headline'] = $strongarm;

  return $export;
}
