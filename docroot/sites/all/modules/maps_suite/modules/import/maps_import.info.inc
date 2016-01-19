<?php

 /**
 * @file
 * Implement Entity API related hooks.
 */

/**
 * Implements hook_entity_property_info_alter().
 */
function maps_import_entity_property_info_alter(&$info) {
  foreach ($info as $entity_type => &$properties) {
    maps_import_entity_property_get_class($properties);

    if (!empty($properties['bundles'])) {
      foreach (array_keys($properties['bundles']) as $bundle) {
        maps_import_entity_property_get_class($properties['bundles'][$bundle]);
      }
    }
  }
}

/**
 * Add the expected Drupal Field class to each existing property.
 */
function maps_import_entity_property_get_class(&$properties) {
  $namespace = 'Drupal\\maps_import\\Mapping\\Target\\Drupal\\Field\\';

  if (!empty($properties['properties'])) {
    foreach ($properties['properties'] as $name => $property) {
      $classname = '';

      if (isset($property['setter callback'])) {
        if (!empty($property['type'])) {
          $type = preg_replace('/^list\<([^>]+)\>$/', '$1', $property['type']);
          $classname = $namespace . maps_suite_drupal2camelcase($type);
        }

        $properties['properties'][$name]['maps_import_handler'] = class_exists($classname) ? $classname : $namespace . 'DefaultField';
      }
    }
  }
}
