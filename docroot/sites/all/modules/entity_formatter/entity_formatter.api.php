<?php

/**
 * @file
 * API documentation for Entity Formatter.
 */

/**
 * Allows modules to use custom formatter classes for specific field-types.
 * Custom classes should subclass EfFieldFormatter or a subclass of it.
 *
 * NOTE: You need to re-generate your classes, after you implemented this hook.
 *
 * To use specific classes for individual fields, you can consider manually
 * modifying the generated EfEntityFormatter class because existent methods are
 * not overwritten the next time the class is generated.
 *
 * @param array $field_type_mapping
 *    Array which maps field-types to specific formatter classes.
 */
function hook_entity_formatter_field_type_mapping_alter(&$field_type_mapping) {
  // For fields of type "link_field" use "EfBetterLinkFieldFormatter" class.
  $field_type_mapping['link_field'] = 'EfBetterLinkFieldFormatter';
}