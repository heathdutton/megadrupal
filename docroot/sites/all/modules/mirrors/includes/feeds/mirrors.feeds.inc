<?php

/**
 * @file
 * Feeds Related
 */

/**
 * Creates a Feed object (the importer).
 */
function mirrors_feeds_create($name, $entity_type, $entity_id, $mirrors_entity) {
  module_load_include('inc', 'mirrors', 'includes/feeds/mirrors.feeds.template');

  $processor = $mirrors_entity['feeds']['processor'];
  $config = mirrors_feeds_default_template($name, $entity_type, $entity_id, $processor);
  $mapping = mirrors_feeds_mapping_create($mirrors_entity);

  $config['processor']['config']['mappings'] = $mapping;

  $feeds_importer = new stdClass();
  $feeds_importer->disabled = FALSE; /* Edit this to true to make a default feeds_importer disabled initially */
  $feeds_importer->api_version = 1;
  $feeds_importer->id = $name;

  $feeds_importer->config = $config;

  return $feeds_importer;
}

/**
 * Returns feeds field mapping.
 */
function mirrors_feeds_mapping_create($mirrors_entity) {
  $mapping = array();

  // We do properties and fields in one shot.
  $mappers = array_merge(
    $mirrors_entity['properties'],
    $mirrors_entity['fields']
  );

  foreach ($mappers as $key => $property) {

    $mapper = array();
    $mapper['source'] = $key;
    $mapper['target'] = $key;

    // Now enable specific feeds overrides.
    if (isset($property['feeds'])) {
      foreach ((array) $property['feeds'] as $override => $value) {
        // Parse key override supporter (with '<$key>:extension' structure).
        mirrors_feeds_mapping_extend_token($key, $override, $value);
        $mapper[$override] = $value;
      }
    }

    // Set GUID field.
    if (isset($property['feeds']['unique']) && $property['feeds']['unique'] == 1) {
      $mapper['unique'] = $property['feeds']['unique'];
    }
    else {
      $mapper['unique'] = 0;
    }

    $mapping[] = $mapper;
  }

  return $mapping;
}

/**
 * Helper function to extend a normal key.
 *
 * This function is used by used in Feeds when a target is extended with a
 * postfix. For and example check mirrors_commerce where this function is used
 * to extend the commerce_product field.
 * @2do: needs more descriptive parameters (key, override).
 */
function mirrors_feeds_mapping_extend_token($key, $override, &$value) {
  // If "SELF" is passed, replace it with the key.
  $value = preg_replace('/SELF/', $key, $value, 1);

  $key_length = strlen($override);
  // Check if we are dealing with a list.
  if (substr($value, 0, $key_length + 2) == '<' . $override . '>') {
    $extension = substr($value, $key_length + 2);
    dpm($extension);
    $value = $key . $extension;
    // Extract field_type "list<$field_type>".
  }
}
