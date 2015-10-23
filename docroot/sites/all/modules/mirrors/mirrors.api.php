<?php

/**
 * @file
 * Documentation of Mirrors hooks.
 *
 * DEPRECATED: REFACTORED TO CTOOLS PLUGINS
 *
 * @todo update API documentation
 * @see: mirrors/contrib for examples
 */

/**
 * Mirrors offers several hooks to extend supported entities, fields and
 * properties.
 *
 * This module exposes 3 hooks:
 *  - hook_mirrors_entity_types_alter
 *  - hook_mirrors_field_types_alter
 *  - hook_mirrors_property_types_alter
 *
 * Mirrors uses the entity_api to read out which entities/bundles are active,
 * but only outputs Views & Feeds that are supported (thus declared).
 *
 * Mirrors configurations act as a mapping-functionality, as a bridge between
 * Feeds and Views.
 *
 * Examples:
 *  - import a taxonomy term entity with 'FeedsTermProcessor' class.
 *  - output a date-property (post-date of node) as timestamp in format 'U'.
 *  - output a decimal-field without thousand separator.
 *
 * Currently is doesn't use CTools plugin API. Therefore you must ensure
 * yourself to load your files in your module.
 */

/**
 * Extend and alter entity_types.
 *
 * hook_mirrors_entity_types_alter()
 * @see: mirrors.defaults.inc
 */
function MY_MODULE_mirrors_entity_types_alter(&$entity_types) {
  // Add your custom entity type
  // i.e.: node, taxonomy_term.
  $entity_types['MY_CUSTOM_ENTITY_TYPE'];

  // Add Views configuration
  // supported: base_table, filters, relationships, sorts
  // @see mirrors.views.inc
  // @see complex example: entity/taxonomy_term.inc
  $entity_types['MY_CUSTOM_ENTITY_TYPE']['views'] = array(
    // Required
    'base_table' => 'taxonomy_term_data',
    // Highly recommended
    'filters' => array(
      'machine_name' => 'taxonomy_vocabulary',
    ),
    // Optional, useful when sequence of entities is important while importing.
    // i.e.: first parent terms, then child terms (taxonomy, menu).
    // i.e.: first product-variations, then referencing product-displays.
    'relationships' => array(
      'parent' => 'taxonomy_term_hierarchy',
    ),
    'sorts' => array(
      'tid' => 'taxonomy_term_data',
    ),
  );

  // Add Feeds configuration
  // supported: processor
  $entity_types['MY_CUSTOM_ENTITY_TYPE']['feeds'] = array(
    // Required
    'processor' => 'FeedsTermProcessor',
  );

  // Add property configuration.
  // Because we cannot trust on internal Drupal functions to get the 'field-type' of a property these are declared.
  // i.e. 'status' is of type 'boolean', 'nid' is of type 'number'.
  //
  // One property must be declared UNIQUE, this is required for feeds to function.
  //
  // Properties can be extended with special settings for views rendedering.
  // i.e. 'link_to_node' = FALSE
  $entity_types['MY_CUSTOM_ENTITY_TYPE']['properties'] = array(
    // Each property has its own array.
    'name' => array(
      // required
      'type' => 'text',
      // required for a least one time.
      'unique' => TRUE,
    ),
    // This property is a special case, it has some extra settings for displaying results in Views.
    // These 'views' settings will be blindly passed to Views.
    'parent' => array(
      'type' => 'integer',
      'views' => array(
        'field' => 'name',
        'relationship' => 'parent',
      ),
    ),
    'uid' => array(
      'type' => 'text',
      'views' => array(
        'link_to_user' => FALSE,
      ),
    ),
  );
}

/**
 * Extend and alter field_types.
 *
 * hook_mirrors_field_types_alter() *
 * @see: mirrors.defaults.inc
 */
function MY_MODULE_mirrors_field_types_alter(&$field_types) {
  // Add your custom field type
  // i.e.: text, price, entity_reference.
  $field_types['MY_CUSTOM_FIELD_TYPE'] = array();

  // The configuration contains a Views and a Feeds part. Both are required but can be left empty.
  // @see: field/boolean.inc for a complex example
  // @see: field/text.inc for the most easy example
  $field_types['MY_CUSTOM_FIELD_TYPE']['views'] = array(
    // note: this is the actual boolean field views configuration
    // @see: field/boolean.inc
    'views' => array(
      'type' => 'boolean_yes_no',
      'settings' => array(
        'format' => 'boolean',
        'custom_on' => '',
        'custom_off' => '',
        'reverse' => 0,
      ),
    ),
  );
  $field_types['MY_CUSTOM_FIELD_TYPE']['feeds'] = array(
    // note: this is the actual taxonomy_term field feeds configuration
    // @see: field/taxonomy_term.inc
    'feeds' => array(
      'term_search' => '0',
      'autocreate' => '1',
    ),
  );

}

/**
 * Extend and alter property_types.
 *
 * hook_mirrors_property_types_alter()
 * @see: mirrors.defaults.inc
 */
function MY_MODULE_mirrors_property_types_alter(&$property_types) {
  // Almost identical to field-declarations. It separated cause especialiy in Views settings can be different.

  // Add your custom field type
  // i.e.: text, price, entity_reference.
  $property_types['MY_CUSTOM_FIELD_TYPE'] = array();

  // The configuration contains a Views and a Feeds part. Both are required but can be left empty.
  // @see: property/date.inc a complex example
  // @see: property/text.inc for the most easy example
  $property_types['MY_CUSTOM_FIELD_TYPE']['views'] = array(
    // note: this is the actual date property views configuration
    // @see: property/date.inc
    'views' => array(
      'date_format' => 'custom',
      'custom_date_format' => 'U',
    ),
  );
  $property_types['MY_CUSTOM_FIELD_TYPE']['feeds'] = array(
    // Required, but can be left empty.
    // Settings will be passed blindy. Alter and save a feed or view to see what configuration you need.
    'feeds' => array(),
  );
}
