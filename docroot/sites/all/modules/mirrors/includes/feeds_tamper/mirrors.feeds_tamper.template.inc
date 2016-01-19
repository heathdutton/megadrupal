<?php

/**
 * @file
 * Feeds Default Template
 *
 * @todo: this is a template for 'explode' operation
 * @todo: make this callable from field-plugin definition
 */

/**
 * Returns Feeds Importer object.
 *
 * @param string $name
 *   Name of importer.
 *
 * @param string $field_id
 *   Field Id (field_product, field_terms, etc.).
 */
function mirrors_feeds_tamper_default_template($name, $field_id) {

  $feeds_tamper = new stdClass();
  $feeds_tamper->disabled = FALSE; /* Edit this to true to make a default feeds_tamper disabled initially */
  $feeds_tamper->api_version = 2;
  $feeds_tamper->id = $name . '-' . $field_id . '-explode';
  $feeds_tamper->importer = $name;
  $feeds_tamper->source = $field_id;
  $feeds_tamper->plugin_id = 'explode';
  $feeds_tamper->settings = array(
    'separator' => MIRRORS_DEFAULT_SEPARATOR,
    'limit' => '',
    'real_separator' => ',',
  );
  $feeds_tamper->weight = 0;
  $feeds_tamper->description = 'Explode ' . $field_id;

  return $feeds_tamper;
}
